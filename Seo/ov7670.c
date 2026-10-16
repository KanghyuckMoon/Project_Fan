#include "ov7670.h"
#include "uart0.h"

// 핀 매핑 정의 (PA0: SIOC/SCL, PA1: SIOD/SDA)
#define SCCB_PORT   PORTA
#define SCCB_PIN    PINA
#define SCCB_DDR    DDRA
#define SIOC_BIT    PA0
#define SIOD_BIT    PA1

#define CAM_CTRL_PORT   PORTE
#define CAM_CTRL_PIN    PINE
#define CAM_CTRL_DDR    DDRE

#define VSYNC_BIT       PE7
#define HREF_BIT        PE5
#define PCLK_BIT        PE3

// 8bit Data port (Port C)
#define CAM_DATA_PIN    PINC
#define CAM_DATA_DDR    DDRC


// 레지스터 설정 구조체
typedef struct {
    uint8_t addr;
    uint8_t val;
} regval_list;

// // QQVGA (160x120) 최적화 설정 배열
// const regval_list ov7670_qqvga_cfg[] = {
//     { 0x12, 0x80 }, // COM7: Soft Reset
//     { 0x11, 0x01 }, // CLKRC: Internal clock prescaler
//     { 0x12, 0x06 }, // COM7: QQVGA
//     { 0x0c, 0x00 }, // COM3: Scaling enabled
//     { 0x3e, 0x00 }, // COM14
//     { 0x70, 0x3a }, // SCALING_XSC
//     { 0x71, 0x35 }, // SCALING_YSC
//     { 0x72, 0x11 }, // SCALING_DCWCTR
//     { 0x73, 0xf0 }, // SCALING_PCLK_DIV
//     { 0xa2, 0x02 }, // SCALING_PCLK_DELAY
//     { 0xff, 0xff }, // Terminator
// };

// QQVGA (160x120) Grayscale 최적화 설정 배열
const regval_list ov7670_qqvga_cfg[] = {
    { 0x12, 0x80 }, // COM7: Soft Reset
    { 0x11, 0x01 }, // CLKRC: Internal clock prescaler
    { 0x12, 0x06 }, // COM7: QQVGA 해상도 선택
    { 0x0c, 0x00 }, // COM3: Scaling enabled
    
    // Grayscale(흑백) 출력 활성화를 위한 COM14 및 Color Matrix 설정
    { 0x3e, 0x00 }, // COM14
    { 0x70, 0x3a }, // SCALING_XSC
    { 0x71, 0x35 }, // SCALING_YSC
    { 0x72, 0x11 }, // SCALING_DCWCTR
    { 0x73, 0xf0 }, // SCALING_PCLK_DIV
    { 0xa2, 0x02 }, // SCALING_PCLK_DELAY

    // 흑백(Grayscale) 모드 설정 (COM13[6]: Gamma enable, COM13[5]: UV saturation / Grayscale)
    { 0xb0, 0x84 }, // Reserved or Color matrix settings
    { 0x81, 0x3f }, // 흑백 출력을 위한 AWB/Color 매개변수 조정
    { 0x28, 0x00 }, // COM13: UV saturation 저하를 통한 Grayscale 유도 (또는 0x40)
    
    { 0xff, 0xff }, // Terminator
};

// --- SCCB 저수준 통신 함수들을 먼저 정의합니다 ---
static void SCCB_Delay(void)
{
    _delay_us(5); // 안정성 확보를 위한 딜레이
}

void SCCB_Init(void)
{
    SCCB_DDR |= (1 << SIOC_BIT) | (1 << SIOD_BIT);
    SCCB_PORT |= (1 << SIOC_BIT) | (1 << SIOD_BIT);
}

static void SCCB_Start(void)
{
    SCCB_DDR |= (1 << SIOD_BIT);
    SCCB_PORT |= (1 << SIOD_BIT);
    SCCB_PORT |= (1 << SIOC_BIT);
    SCCB_Delay();
    SCCB_PORT &= ~(1 << SIOD_BIT); 
    SCCB_Delay();
    SCCB_PORT &= ~(1 << SIOC_BIT);
}

static void SCCB_Stop(void)
{
    SCCB_DDR |= (1 << SIOD_BIT);
    SCCB_PORT &= ~(1 << SIOD_BIT);
    SCCB_Delay();
    SCCB_PORT |= (1 << SIOC_BIT);
    SCCB_Delay();
    SCCB_PORT |= (1 << SIOD_BIT); 
    SCCB_Delay();
}

static uint8_t SCCB_SendByte(uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (data & 0x80)
            SCCB_PORT |= (1 << SIOD_BIT);
        else
            SCCB_PORT &= ~(1 << SIOD_BIT);
        
        data <<= 1;
        SCCB_Delay();
        SCCB_PORT |= (1 << SIOC_BIT);
        SCCB_Delay();
        SCCB_PORT &= ~(1 << SIOC_BIT);
    }
    
    SCCB_DDR &= ~(1 << SIOD_BIT); 
    SCCB_PORT |= (1 << SIOD_BIT); 
    SCCB_Delay();
    SCCB_PORT |= (1 << SIOC_BIT);
    SCCB_Delay();
    
    uint8_t ack = (SCCB_PIN & (1 << SIOD_BIT)); 
    
    SCCB_PORT &= ~(1 << SIOC_BIT);
    SCCB_DDR |= (1 << SIOD_BIT); 
    
    return (ack == 0);
}

static uint8_t SCCB_ReceiveByte(void)
{
    uint8_t i, data = 0;
    
    SCCB_DDR &= ~(1 << SIOD_BIT); 
    SCCB_PORT |= (1 << SIOD_BIT); 
    
    for (i = 0; i < 8; i++)
    {
        SCCB_Delay();
        SCCB_PORT |= (1 << SIOC_BIT);
        data <<= 1;
        if (SCCB_PIN & (1 << SIOD_BIT))
            data |= 0x01;
        
        SCCB_Delay();
        SCCB_PORT &= ~(1 << SIOC_BIT);
    }
    SCCB_DDR |= (1 << SIOD_BIT); 
    return data;
}

// --- 상위 레지스터 제어 함수들을 아래에 배치합니다 ---

uint8_t OV7670_WriteReg(uint8_t regAddr, uint8_t data)
{
    SCCB_Start();
    if (!SCCB_SendByte(OV7670_I2C_ADDR)) { SCCB_Stop(); return 0; }
    if (!SCCB_SendByte(regAddr))        { SCCB_Stop(); return 0; }
    if (!SCCB_SendByte(data))           { SCCB_Stop(); return 0; }
    SCCB_Stop();
    _delay_ms(1);
    return 1;
}

uint8_t OV7670_ReadReg(uint8_t regAddr)
{
    uint8_t val = 0;

    SCCB_Start();
    if (!SCCB_SendByte(OV7670_I2C_ADDR)) { SCCB_Stop(); return 0xFF; }
    if (!SCCB_SendByte(regAddr))        { SCCB_Stop(); return 0xFF; }
    SCCB_Stop();

    _delay_us(200); 

    SCCB_Start();
    if (!SCCB_SendByte(OV7670_I2C_ADDR | 0x01)) { SCCB_Stop(); return 0xFF; }
    val = SCCB_ReceiveByte();
    SCCB_Stop();

    return val;
}

void OV7670_Init(void)
{
    uint16_t i = 0;
    while (ov7670_qqvga_cfg[i].addr != 0xff)
    {
        OV7670_WriteReg(ov7670_qqvga_cfg[i].addr, ov7670_qqvga_cfg[i].val);
        i++;
    }
}

void Camera_Sync_Init(void)
{
    // 1. 제어 신호 핀들을 입력 모드로 설정
    CAM_CTRL_DDR &= ~((1<<VSYNC_BIT) | (1<<HREF_BIT) | (1<<PCLK_BIT));

    // 내부 풀업 저항 활성화
    CAM_CTRL_PORT |= (1<<VSYNC_BIT) | (1<<HREF_BIT) | (1<<PCLK_BIT);

    // 2. 8 bit data bus portc
    CAM_DATA_DDR = 0x00;
}

// void OV7670_Capture_Frame(void)
// {
//     // 1. VSYNC가 High가 될 때까지 대기
//     while (!(CAM_CTRL_PIN & (1<<VSYNC_BIT)));

//     // 2. VSYNC가 Low로 떨어질 때까지 대기 (새로운 프레임 시작)
//     while (CAM_CTRL_PIN & (1<<VSYNC_BIT));

//     // 3. QQVGA 기준 120 line 반복
//     for(uint16_t h=0; h<120; h++)
//     {
//         // HREF가 High 가 될 때까지 대기 (유효한 행 시작)
//         while(!(CAM_CTRL_PIN & (1<<HREF_BIT)));

//         // QQVGA 기준 160 픽셀 반복
//         for(uint16_t w=0; w<160; w++)
//         {
//             // PCLK 가 Low일 때 대기
//             while(CAM_CTRL_PIN & (1<<PCLK_BIT));

//             // Pixel data 읽기 (PORTC에서 8비트 데이터 수신)
//             uint8_t pixel = CAM_DATA_PIN;
            
//             // Todo: 수신된 pixel 데이터를 LCD나 UART로 전송/처리

//             // PCLK가 High에서 Low로 떨어질 때까지 대기
//             while(CAM_CTRL_PIN & (1<<PCLK_BIT));
//         }
//         // HREF가 Low가 될 때까지 대기 (행 종료)
//         while(CAM_CTRL_PIN & (1<<HREF_BIT));
//     }
// }

// 디버깅을 위한 결과값 저장 변수 (외부나 메인에서 참조 가능)
uint8_t sample_pixel = 0;
uint16_t non_zero_count = 0;

void OV7670_Capture_Frame(void)
{
    // 1. VSYNC가 High가 될 때까지 대기
    while (!(CAM_CTRL_PIN & (1<<VSYNC_BIT)));

    // 2. VSYNC가 Low로 떨어질 때까지 대기 (새로운 프레임 시작)
    while (CAM_CTRL_PIN & (1<<VSYNC_BIT));

    non_zero_count = 0; // 프레임 시작 시 카운터 초기화

    // 3. QQVGA 기준 120 line 반복
    for(uint16_t h=0; h<120; h++)
    {
        // HREF가 High가 될 때까지 대기 (유효한 행 시작)
        while(!(CAM_CTRL_PIN & (1<<HREF_BIT)));

        // QQVGA 기준 160 픽셀 반복
        for(uint16_t w=0; w<160; w++)
        {
            // PCLK가 Low일 때 대기
            while(CAM_CTRL_PIN & (1<<PCLK_BIT));

            // Pixel data 읽기 (PORTC에서 8비트 데이터 수신)
            uint8_t pixel = CAM_DATA_PIN;
            
            // 첫 번째 라인의 첫 번째 픽셀 샘플링 저장
            if (h == 0 && w == 0) {
                sample_pixel = pixel;
            }

            // 0이 아닌 유효 데이터 카운트
            if (pixel != 0x00 && pixel != 0xFF) {
                non_zero_count++;
            }

            // PCLK가 High에서 Low로 떨어질 때까지 대기
            while(CAM_CTRL_PIN & (1<<PCLK_BIT));
        }
        // HREF가 Low가 될 때까지 대기 (행 종료)
        while(CAM_CTRL_PIN & (1<<HREF_BIT));
    }
}

void OV7670_Stream_Frame_UART(void)
{
    // 1. VSYNC 대기 및 프레임 시작 동기화
    while (!(CAM_CTRL_PIN & (1<<VSYNC_BIT)));
    while (CAM_CTRL_PIN & (1<<VSYNC_BIT));

    // 2. PC 측에서 프레임의 시작을 알 수 있도록 고유한 헤더 전송
    uartTransmit(0xFF);
    uartTransmit(0x00);
    uartTransmit(0xFF);
    uartTransmit(0xAA);

    // 3. QQVGA 기준 120 line 반복
    for(uint16_t h=0; h<120; h++)
    {
        while(!(CAM_CTRL_PIN & (1<<HREF_BIT)));

        for(uint16_t w=0; w<160; w++)
        {
            while(CAM_CTRL_PIN & (1<<PCLK_BIT));

            // Pixel data read
            uint8_t pixel = CAM_DATA_PIN;

            // UART를 통해 PC로 바이트 전송
            uartTransmit(pixel);

            while(CAM_CTRL_PIN & (1<<PCLK_BIT));
        }
        while(!(CAM_CTRL_PIN & (1<<HREF_BIT)));
    }
}