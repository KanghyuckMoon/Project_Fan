#include "touch.h"
#include "tft.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// 터치스크린 핀: ATmega128A PORTA
#define TOUCH_CS  PA3
#define TOUCH_CLK PA4
#define TOUCH_DIN PA5
#define TOUCH_DO  PA6
#define TOUCH_IRQ PA7

#define TOUCH_CS_LOW()  (PORTA &= ~(1 << TOUCH_CS))
#define TOUCH_CS_HIGH() (PORTA |=  (1 << TOUCH_CS))

// 터치 컨트롤러로 1바이트 전송
static void TOUCH_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
            PORTA |= (1 << TOUCH_DIN);
        else
            PORTA &= ~(1 << TOUCH_DIN);

        PORTA |= (1 << TOUCH_CLK);
        PORTA &= ~(1 << TOUCH_CLK);

        data <<= 1;
    }
}

// 터치 컨트롤러에서 1바이트 수신
static uint8_t TOUCH_ReadByte(void)
{
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        PORTA |= (1 << TOUCH_CLK);

        data <<= 1;

        if (PINA & (1 << TOUCH_DO))
            data |= 1;

        PORTA &= ~(1 << TOUCH_CLK);
    }

    return data;
}

// 터치 ADC 데이터 읽기
static uint16_t TOUCH_ReadADC(uint8_t command)
{
    uint16_t value = 0;

    // TFT 선택 해제
    CS_HIGH();

    // 터치 컨트롤러 선택
    TOUCH_CS_LOW();

    // ADC 명령 전송
    TOUCH_WriteByte(command);

    // 16비트 수신
    value = (uint16_t)TOUCH_ReadByte() << 8;
    value |= TOUCH_ReadByte();

    // 터치 컨트롤러 선택 해제
    TOUCH_CS_HIGH();

    // 12비트 ADC 값 추출
    return (value >> 3) & 0x0FFF;
}

// 터치 초기화
void TOUCH_Init(void)
{
    DDRA |= (1 << TOUCH_CS)
          | (1 << TOUCH_CLK)
          | (1 << TOUCH_DIN);

    DDRA &= ~((1 << TOUCH_DO) | (1 << TOUCH_IRQ));

    TOUCH_CS_HIGH();

    PORTA &= ~(1 << TOUCH_CLK);
    PORTA &= ~(1 << TOUCH_DIN);

    // PA7 내부 풀업 활성화
    PORTA |= (1 << TOUCH_IRQ);
}

// 터치 여부 확인
// 일반 입력 방식: IRQ가 LOW이면 눌린 것으로 판단
uint8_t TOUCH_IsPressed(void)
{
    return !(PINA & (1 << TOUCH_IRQ));
}

// X축 ADC 값 읽기
uint16_t TOUCH_ReadX(void)
{
    return TOUCH_ReadADC(0xD0);
}

// Y축 ADC 값 읽기
uint16_t TOUCH_ReadY(void)
{
    return TOUCH_ReadADC(0x90);
}

// 터치 좌표 읽기
uint8_t TOUCH_GetXY(uint16_t *x, uint16_t *y)
{
    if (!TOUCH_IsPressed())
        return 0;

    _delay_ms(10);

    if (!TOUCH_IsPressed())
        return 0;

    *x = TOUCH_ReadX();
    *y = TOUCH_ReadY();

    return 1;
}