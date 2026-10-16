#include "ap.h"
#include "ov7670.h"

char rxBuff[100] = {0,};
volatile uint8_t rxFlag = 0;

// 스트리밍 상태 저장을 위한 토글 플래그 (0: 정지 상태, 1: 연속 전송 상태)
volatile uint8_t isStreaming = 0;

int uartPutchar(char c, FILE *stream)
{
    if (c == '\n')
        uartPutchar('\r', stream);
    
    while (!(UCSR0A & (1 << UDRE0))); 
    UDR0 = c;                         
    return 0;
}

ISR(USART0_RX_vect)
{    
    uint8_t rxData = UDR0;

    // 's'나 'x'가 들어오면 엔터 입력 없이 즉시 스트리밍 제어
    if (rxData == 's' || rxData == 'S')
    {
        isStreaming = 1;
        LCD_WriteStringXY(1, 0, "Stream: On     ");
    }
    else if (rxData == 'x' || rxData == 'X')
    {
        isStreaming = 0;
        LCD_WriteStringXY(1, 0, "Stream: OFF    ");
    }

    // 기존 문자열 명령어('p', 'c' 등) 처리를 위한 로직 유지
    static uint8_t rxHead = 0;
    if(rxData == '\n' || rxData == '\r')
    {
        rxBuff[rxHead] = '\0';
        rxHead = 0;
        rxFlag = 1;       
    }
    else
    {
        if(rxHead < sizeof(rxBuff) - 1)
        {
            rxBuff[rxHead++] = rxData;
        }
    }
}

FILE OUTPUT = FDEV_SETUP_STREAM(uartPutchar, NULL, _FDEV_SETUP_WRITE);

void apInit()
{
    uartInit();
    LCD_Init();

    // 타이머 1 레지스터 초기화
    TCCR1A = 0;
    TCCR1B = 0;
    
    // PB6을 출력으로 설정
    DDRB |= (1 << PB6);
    PORTB &= ~(1 << PB6);

    // CTC 모드 (WGM12 = 1) 및 Toggle OC1B on Compare Match (COM1B0 = 1)
    // 분주비 1 (CS10 = 1) -> F_CPU / (2 * (1 + OCR1A))
    TCCR1A = (1 << COM1B0);                    
    TCCR1B = (1 << WGM12) | (1 << CS10);       
    OCR1A = 0; // OCR1A = 0 이면 16MHz / (2 * 1) = 8MHz 구형파 출력
    
    // 2. 카메라 제어 핀(RESET: PA2, PWDN: PA3) 설정 및 리셋 시퀀스
    DDRA |= (1 << PA2) | (1 << PA3);           
    PORTA &= ~(1 << PA3);                      // PWDN = Low (절전 모드 해제)
    PORTA &= ~(1 << PA2);                      // RESET = Low (리셋 상태 유지)
    _delay_ms(20);
    PORTA |= (1 << PA2);                       // RESET = High (카메라 동작 시작)
    _delay_ms(50);                             // 카메라 안정화 대기

    // 3. SCCB(I2C) 통신 초기화
    SCCB_Init();
    Camera_Sync_Init();
    OV7670_Init(); // 카메라 레지스터 설정 추가

    stdout = &OUTPUT;
}

void apMain()
{
    sei();

    // 부팅 직후 핀 전압이 완전히 안정되도록 대기
    _delay_ms(100);

    printf("\r\n=== System Initialized ===\r\n");
    printf("Commands:\r\n");
    printf(" 'p' : Read OV7670 PID\r\n");
    printf(" 'c' : Capture 1 Frame & Check Pixels\r\n");
    printf(" 's' : Start Camera Stream\r\n");
    printf(" 'x' : Stop Camera Stream\r\n");
    _delay_ms(1000);

    // 부팅 직후 자동 PID 검증 수행
    printf("Reading OV7670 Product ID (Reg: 0x0A)...\r\n");
    uint8_t pid = OV7670_ReadReg(REG_PID);
    printf("OV7670 PID Result: 0x%02X\r\n", pid);

    if (pid == 0x76)
    {
        printf("[SUCCESS] Camera PID matched (0x76)!\r\n");
        LCD_WriteStringXY(0, 0, "OV7670 OK (0x76)");
    }
    else
    {
        printf("[FAIL] Check I2C wiring or pull-up resistors. (Read: 0x%02X)\r\n", pid);
        LCD_WriteStringXY(0, 0, "OV7670 FAIL");
    }   

    // 초기 상태: 스트리밍 대기 (정지) 상태
    isStreaming = 0;
    LCD_WriteStringXY(1, 0, "Stream: OFF    ");

    while(1)
    {          
        // 1. 기존 UART 수신 처리 유지
        if(rxFlag == 1)
        {
            rxFlag = 0;           

            if(strcmp(rxBuff, "p") == 0)     
            {
                printf("Re-reading OV7670 Product ID...\r\n");
                pid = OV7670_ReadReg(REG_PID);
                printf("OV7670 PID Result: 0x%02X\r\n", pid);
            }            
            else if(strcmp(rxBuff, "c") == 0)
            {
                char lcd_buf[17];
                printf("Capturing camera frame...\r\n");                

                // Frame capture
                OV7670_Capture_Frame();
                
                // Result
                printf("Capture Done ! Non-zero pixels: %u, Sample Pixel: %02X\r\n", non_zero_count, sample_pixel);

                // LCD에 유효 픽셀 수 표시
                sprintf(lcd_buf, "Pixels: %u", non_zero_count);
                LCD_WriteStringXY(1, 0, lcd_buf);
            }
            else if(strcmp(rxBuff, "s") == 0)
            {
                isStreaming = 1;
                printf("Starting camera stream to PC...\r\n");           
                LCD_WriteStringXY(1, 0, "Stream: On     ");
            }
            else if(strcmp(rxBuff, "x") == 0)
            {
                isStreaming = 0;
                printf("Camera stream stopped.\r\n");           
                LCD_WriteStringXY(1, 0, "Stream: OFF    ");
            }
        }

        // 2. 스트리밍 상태가 On일 경우 지속적으로 프레임 전송 수행
        if(isStreaming)
        {
            OV7670_Stream_Frame_UART();
        }
    }    
}