#include "tft.h"

void SPI_Write(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
            TFT_PORT |= (1 << TFT_MOSI);
        else
            TFT_PORT &= ~(1 << TFT_MOSI);

        TFT_PORT |= (1 << TFT_SCK);
        TFT_PORT &= ~(1 << TFT_SCK);

        data <<= 1;
    }
}

void TFT_Command(uint8_t cmd)
{
    CS_LOW();
    DC_LOW();

    SPI_Write(cmd);

    CS_HIGH();
}

void TFT_Data(uint8_t data)
{
    CS_LOW();
    DC_HIGH();

    SPI_Write(data);

    CS_HIGH();
}

void TFT_Init(void)
{
    DDRC |= (1 << PC0) |
        (1 << PC1) |
        (1 << PC2) |
        (1 << PC3) |
        (1 << PC4) |
        (1 << PC5);

    DDRC &= ~((1 << PC6) | (1 << PC7));

    TFT_DDR |= (1 << TFT_CS)
            |  (1 << TFT_RST)
            |  (1 << TFT_DC)
            |  (1 << TFT_MOSI)
            |  (1 << TFT_SCK);
            

    CS_HIGH();
    TFT_PORT &= ~(1 << TFT_SCK);

    // Hardware reset
    RST_LOW();
    _delay_ms(20);

    RST_HIGH();
    _delay_ms(150);

    // Software reset
    TFT_Command(0x01);
    _delay_ms(150);

    // Sleep out
    TFT_Command(0x11);
    _delay_ms(150);

    // 16-bit RGB565
    TFT_Command(0x3A);
    TFT_Data(0x55);

    // Memory Access Control
    TFT_Command(0x36);
    TFT_Data(0x48);

    // Display ON
    TFT_Command(0x29);
    _delay_ms(20);
}

void TFT_SetWindow(uint16_t x0, uint16_t y0,
                   uint16_t x1, uint16_t y1)
{
    TFT_Command(0x2A);

    TFT_Data(x0 >> 8);
    TFT_Data(x0 & 0xFF);
    TFT_Data(x1 >> 8);
    TFT_Data(x1 & 0xFF);

    TFT_Command(0x2B);

    TFT_Data(y0 >> 8);
    TFT_Data(y0 & 0xFF);
    TFT_Data(y1 >> 8);
    TFT_Data(y1 & 0xFF);

    TFT_Command(0x2C);
}

void TFT_FillScreen(uint16_t color)
{
    TFT_SetWindow(0, 0, 239, 319);

    CS_LOW();
    DC_HIGH();

    for (uint32_t i = 0; i < 240UL * 320UL; i++)
    {
        SPI_Write(color >> 8);
        SPI_Write(color & 0xFF);
    }

    CS_HIGH();
}

uint8_t Touch_SPI_Transfer(uint8_t data)
{
    uint8_t rx = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
            TOUCH_MOSI_HIGH();
        else
            TOUCH_MOSI_LOW();

        data <<= 1;

        TOUCH_SCK_HIGH();

        rx <<= 1;

        if (TOUCH_DO_PIN & (1 << TOUCH_DO_BIT))
            rx |= 1;

        TOUCH_SCK_LOW();
    }

    return rx;
}

uint16_t Touch_ReadRaw(uint8_t command)
{
    uint16_t value;

    TOUCH_CS_LOW();

    Touch_SPI_Transfer(command);

    value = ((uint16_t)Touch_SPI_Transfer(0x00) << 8);
    value |= Touch_SPI_Transfer(0x00);

    TOUCH_CS_HIGH();

    // 12-bit 데이터 추출
    value >>= 3;

    return value & 0x0FFF;
}

uint16_t Touch_ReadX(void)
{
    return Touch_ReadRaw(0xD0);
}

uint16_t Touch_ReadY(void)
{
    return Touch_ReadRaw(0x90);
}

void TouchTest(void)
{
    if (TOUCH_IRQ_PIN & (1 << TOUCH_IRQ_BIT))
    {
        DebugLog("IRQ HIGH\r\n");
        return;
    }

    DebugLog("IRQ LOW - Touch detected\r\n");

    uint16_t x = Touch_ReadX();
    uint16_t y = Touch_ReadY();

    DebugLog("Touch X:%u Y:%u\r\n", x, y);
}