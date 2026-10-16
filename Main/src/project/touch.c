#include "touch.h"
#include "tft.h"
#include "debug.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


volatile uint16_t x, y;
volatile uint8_t zone;


static uint16_t MapValue(
    uint16_t value,
    uint16_t in_min,
    uint16_t in_max,
    uint16_t out_min,
    uint16_t out_max
)
{
    if (value < in_min)
        value = in_min;

    if (value > in_max)
        value = in_max;

    return (uint32_t)(value - in_min) *
           (out_max - out_min) /
           (in_max - in_min) +
           out_min;
}

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
            TOUCH_PORT |= (1 << TOUCH_DIN);
        }
        else
        {
            TOUCH_PORT &= ~(1 << TOUCH_DIN);
        }

        // Clock
        TOUCH_PORT |= (1 << TOUCH_CLK);
        TOUCH_PORT &= ~(1 << TOUCH_CLK);

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
        TOUCH_PORT |= (1 << TOUCH_CLK);

        data <<= 1;

        if (TOUCH_PIN & (1 << TOUCH_DO))
        {
            data |= 1;
        }

        TOUCH_PORT &= ~(1 << TOUCH_CLK);
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
    TOUCH_PORT_DDR |= (1 << TOUCH_CS) | (1 << TOUCH_CLK) | (1 << TOUCH_DIN);


    // 입력
    TOUCH_PORT_DDR &= ~((1 << TOUCH_DO) | (1 << TOUCH_IRQ));

    // 기본 상태
    TOUCH_CS_HIGH();

    TOUCH_PORT &= ~(1 << TOUCH_CLK);
    TOUCH_PORT &= ~(1 << TOUCH_DIN);


    // IRQ Pull-up
    TOUCH_PORT |= (1 << TOUCH_IRQ);
}


// ========================================
// Touch Press Check
// ========================================

uint8_t TOUCH_IsPressed(void)
{
    // IRQ LOW = pressed
    return !(TOUCH_PIN & (1 << TOUCH_IRQ));
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

uint8_t TOUCH_GetXY(uint16_t *x, uint16_t *y)
{
    static uint8_t debounce_count = 0;

    if (!TOUCH_IsPressed())
    {
        debounce_count = 0;
        return 0;
    }

    if (debounce_count < 10)
    {
        debounce_count++;
        return 0;
    }

    uint16_t rawX = TOUCH_ReadX();
    uint16_t rawY = TOUCH_ReadY();

    uint16_t mappedX = MapValue(
        rawX,
        TOUCH_X_MIN,
        TOUCH_X_MAX,
        0,
        TFT_WIDTH - 1
    );

    uint16_t mappedY = MapValue(
        rawY,
        TOUCH_Y_MIN,
        TOUCH_Y_MAX,
        0,
        TFT_HEIGHT - 1
    );

    // 터치 좌표계 -> TFT 좌표계 변환
    *x = (TFT_WIDTH  - 1) - mappedX;
    *y = (TFT_HEIGHT - 1) - mappedY;

    return 1;
}