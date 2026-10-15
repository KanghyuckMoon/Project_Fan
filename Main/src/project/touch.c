#include "touch.h"
#include "tft.h"
#include "debug.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


volatile uint16_t x, y;
volatile uint8_t zone;

// ========================================
// Touch Software SPI Write
// ========================================

static void TOUCH_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // DIN
        if (data & 0x80)
        {
            PORTA |= (1 << TOUCH_DIN);
        }
        else
        {
            PORTA &= ~(1 << TOUCH_DIN);
        }

        // Clock
        PORTA |= (1 << TOUCH_CLK);
        PORTA &= ~(1 << TOUCH_CLK);

        data <<= 1;
    }
}


// ========================================
// Touch Software SPI Read
// ========================================

static uint8_t TOUCH_ReadByte(void)
{
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        PORTA |= (1 << TOUCH_CLK);

        data <<= 1;

        if (PINA & (1 << TOUCH_DO))
        {
            data |= 1;
        }

        PORTA &= ~(1 << TOUCH_CLK);
    }

    return data;
}


// ========================================
// XPT2046 ADC Read
// ========================================

static uint16_t TOUCH_ReadADC(uint8_t command)
{
    uint16_t value;


    // TFT 통신 해제
    CS_HIGH();

    // Touch 선택
    TOUCH_CS_LOW();


    // Command 전송
    TOUCH_WriteByte(command);


    // 16bit 데이터 읽기
    value = (uint16_t)TOUCH_ReadByte() << 8;
    value |= TOUCH_ReadByte();


    // Touch 선택 해제
    TOUCH_CS_HIGH();


    // XPT2046 12bit ADC
    value >>= 3;

    return value & 0x0FFF;
}


// ========================================
// Touch Init
// ========================================

void TOUCH_Init(void)
{
    // 출력
    DDRA |= (1 << TOUCH_CS)
          | (1 << TOUCH_CLK)
          | (1 << TOUCH_DIN);


    // 입력
    DDRA &= ~(
        (1 << TOUCH_DO)
        |
        (1 << TOUCH_IRQ)
    );


    // 기본 상태
    TOUCH_CS_HIGH();

    PORTA &= ~(1 << TOUCH_CLK);
    PORTA &= ~(1 << TOUCH_DIN);


    // IRQ Pull-up
    PORTA |= (1 << TOUCH_IRQ);
}


// ========================================
// Touch Press Check
// ========================================

uint8_t TOUCH_IsPressed(void)
{
    // IRQ LOW = pressed
    return !(PINA & (1 << TOUCH_IRQ));
}


// ========================================
// Read X
// ========================================

uint16_t TOUCH_ReadX(void)
{
    return TOUCH_ReadADC(0xD0);
}


// ========================================
// Read Y
// ========================================

uint16_t TOUCH_ReadY(void)
{
    return TOUCH_ReadADC(0x90);
}


// ========================================
// Read X/Y
// ========================================

uint8_t TOUCH_GetXY(
    uint16_t *x,
    uint16_t *y
)
{
    DebugLog("Return 0\n");
    if (!TOUCH_IsPressed())
    {
        return 0;
    }
    DebugLog("Return 1\n");


    // 간단한 debounce
    _delay_ms(10);


    if (!TOUCH_IsPressed())
    {
        return 0;
    }
    DebugLog("Return 2\n");


    *x = TOUCH_ReadX();
    *y = TOUCH_ReadY();


    return 1;
}