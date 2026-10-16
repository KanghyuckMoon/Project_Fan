#include "tft.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


// 상태 변수
volatile uint8_t motor_on = 0;
volatile uint8_t motor_level = 1;
volatile uint8_t rotation_on = 0;

static uint16_t render_buffers[2][TFT_BUFFER_PIXELS];
static uint16_t *render_target;
static uint16_t render_x0, render_y0, render_x1, render_y1;

static void FlushBuffer(const uint16_t *pixels, uint16_t x0, uint16_t y0,
                        uint16_t width, uint16_t rows)
{
    TFT_SetWindow(x0, y0, x0 + width - 1, y0 + rows - 1);
    uint16_t count = width * rows;
    CS_LOW();
    DC_HIGH();
    while (count--)
    {
        uint16_t color = *pixels++;
        SPI_Write(color >> 8);
        SPI_Write(color & 0xFF);
    }
    CS_HIGH();
}

void TFT_RenderRegion(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                      uint16_t clear_color, TFT_Painter painter, void *context)
{
    if (painter == NULL || x0 >= SCREEN_WIDTH || y0 >= SCREEN_HEIGHT)
        return;
    if (x1 > SCREEN_WIDTH)
        x1 = SCREEN_WIDTH;
    if (y1 > SCREEN_HEIGHT)
        y1 = SCREEN_HEIGHT;
    if (x0 >= x1 || y0 >= y1)
        return;

    // 재진입은 거부하여 현재 버퍼 상태를 보존한다.
    if (render_target != NULL)
        return;

    uint16_t width = x1 - x0;
    uint16_t stripe_rows = TFT_BUFFER_PIXELS / width;
    uint16_t pending_y = 0, pending_rows = 0;
    uint8_t back = 0;

    for (uint16_t top = y0; top < y1; )
    {
        uint16_t rows = y1 - top;
        if (rows > stripe_rows)
            rows = stripe_rows;

        render_target = render_buffers[back];
        render_x0 = x0;
        render_y0 = top;
        render_x1 = x1;
        render_y1 = top + rows;
        uint16_t count = width * rows;
        for (uint16_t i = 0; i < count; i++)
            render_target[i] = clear_color;

        // 배경, 글자, 구분선을 합성하는 동안에는 TFT에 쓰지 않는다.
        painter(context);
        render_target = NULL;

        // 이전 front를 끝까지 보낸 뒤 역할을 바꾼다.
        // 소프트웨어 SPI이므로 합성과 전송은 순차 실행된다.
        if (pending_rows != 0)
            FlushBuffer(render_buffers[back ^ 1], x0, pending_y, width, pending_rows);
        pending_y = top;
        pending_rows = rows;
        back ^= 1;
        top += rows;
    }

    FlushBuffer(render_buffers[back ^ 1], x0, pending_y, width, pending_rows);
}



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
    // PORTB
    // CS / MOSI / SCK 출력
    TFT_DDR |= (1 << TFT_CS) |  (1 << TFT_MOSI) |  (1 << TFT_SCK);

    // PORTA
    // RESET / DC 출력
    TFT_RST_DDR |= (1 << TFT_RST);
    TFT_DC_DDR  |= (1 << TFT_DC);


    // 초기 상태
    CS_HIGH();
    DC_HIGH();
    RST_HIGH();

    TFT_PORT &= ~(1 << TFT_SCK);


    // ========================
    // Hardware Reset
    // ========================

    RST_LOW();
    _delay_ms(20);

    RST_HIGH();
    _delay_ms(150);


    // ========================
    // Software Reset
    // ========================

    TFT_Command(0x01);
    _delay_ms(150);


    // Sleep Out
    TFT_Command(0x11);
    _delay_ms(150);


    // RGB565
    // 16bit color
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
    DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

// 사각형 그리기: x1, y1은 포함하지 않는 끝 좌표
void DrawRect(uint16_t x0, uint16_t y0,
              uint16_t x1, uint16_t y1,
              uint16_t color)
{
    if (x0 >= SCREEN_WIDTH || y0 >= SCREEN_HEIGHT)
        return;
    if (x1 > SCREEN_WIDTH)
        x1 = SCREEN_WIDTH;
    if (y1 > SCREEN_HEIGHT)
        y1 = SCREEN_HEIGHT;
    if (x0 >= x1 || y0 >= y1)
        return;

    if (render_target != NULL)
    {
        if (x0 < render_x0) x0 = render_x0;
        if (y0 < render_y0) y0 = render_y0;
        if (x1 > render_x1) x1 = render_x1;
        if (y1 > render_y1) y1 = render_y1;
        if (x0 >= x1 || y0 >= y1)
            return;

        uint16_t stride = render_x1 - render_x0;
        uint16_t *line = render_target + (y0 - render_y0) * stride + x0 - render_x0;
        for (uint16_t row = y0; row < y1; row++)
        {
            for (uint16_t col = 0; col < x1 - x0; col++)
                line[col] = color;
            if (row + 1 < y1)
                line += stride;
        }
        return;
    }

    TFT_SetWindow(x0, y0, x1 - 1, y1 - 1);

    const uint8_t high = color >> 8;
    const uint8_t low = color & 0xFF;
    const uint16_t width = x1 - x0;

    // 픽셀 데이터 전체를 하나의 전송으로 유지한다.
    CS_LOW();
    DC_HIGH();
    for (uint16_t row = y0; row < y1; row++)
    {
        for (uint16_t col = 0; col < width; col++)
        {
            SPI_Write(high);
            SPI_Write(low);
        }
    }
    CS_HIGH();
}

// 글자 하나 그리기
void DrawChar(uint16_t x, uint16_t y, char c,
              uint8_t scale, uint16_t fg, uint16_t bg)
{
    if (scale == 0)
        return;

    if (render_target != NULL)
    {
        uint32_t right = (uint32_t)x + 5U * scale;
        uint32_t bottom = (uint32_t)y + 7U * scale;
        uint16_t left = x < render_x0 ? render_x0 : x;
        uint16_t top = y < render_y0 ? render_y0 : y;
        uint16_t end_x = right > render_x1 ? render_x1 : right;
        uint16_t end_y = bottom > render_y1 ? render_y1 : bottom;
        if (left >= end_x || top >= end_y)
            return;

        uint8_t index = FontIndex(c);
        uint16_t stride = render_x1 - render_x0;
        uint16_t *line = render_target + (top - render_y0) * stride + left - render_x0;
        for (uint16_t py = top; py < end_y; py++)
        {
            uint8_t bits = font[index][(py - y) / scale];
            for (uint16_t px = left; px < end_x; px++)
                line[px - left] = (bits & (1 << (4 - (px - x) / scale))) ? fg : bg;
            if (py + 1 < end_y)
                line += stride;
        }
        return;
    }

    uint8_t index = FontIndex(c);

    TFT_SetWindow(x, y,
                  x + 5 * scale - 1,
                  y + 7 * scale - 1);

    CS_LOW();
    DC_HIGH();
    for (uint8_t row = 0; row < 7; row++)
    {
        for (uint8_t sy = 0; sy < scale; sy++)
        {
            for (uint8_t col = 0; col < 5; col++)
            {
                uint16_t color =
                    (font[index][row] & (1 << (4 - col))) ? fg : bg;

                for (uint8_t sx = 0; sx < scale; sx++)
                {
                    SPI_Write(color >> 8);
                    SPI_Write(color & 0xFF);
                }
            }
        }
    }
    CS_HIGH();
}

// 문자열을 가로 가운데에 배치
void DrawString(uint16_t center_x, uint16_t y,
                const char *str, uint8_t scale,
                uint16_t fg, uint16_t bg)
{
    if (str == NULL || *str == '\0' || scale == 0)
        return;

    uint16_t len = 0;
    while (str[len] != '\0')
        len++;

    uint16_t width = len * 6 * scale - scale;
    uint16_t x = center_x - width / 2;

    while (*str)
    {
        DrawChar(x, y, *str, scale, fg, bg);
        x += 6 * scale;
        str++;
    }
}

// 버튼 하나 그리기
// void DrawButton(uint8_t zone, uint16_t color,
//                 const char *label, uint16_t text_color)
// {
//     uint16_t x = (zone % 2) * 120;
//     uint16_t y = (zone / 2) * 160;

//     // 버튼 배경: 흰색
//     DrawRect(x, y, x + 120, y + 160, WHITE);

//     // 버튼 테두리: 검은색
//     DrawRect(x, y, x + 120, y + 2, BLACK);
//     DrawRect(x, y + 158, x + 120, y + 160, BLACK);
//     DrawRect(x, y, x + 2, y + 160, BLACK);
//     DrawRect(x + 118, y, x + 120, y + 160, BLACK);

//     // 버튼 글자: 검은색
//     DrawString(x + 60, y + 76, label, 2, BLACK, WHITE);
// }


// 현재 상태에 맞춰 버튼 4개 표시
void DrawUI(void)
{
    // DrawButton(0, motor_on ? GREEN : RED,
    //            motor_on ? "ON" : "OFF", WHITE);

    // if (motor_level == 1)
    //     DrawButton(1, BLUE, "LEVEL 1", WHITE);
    // else if (motor_level == 2)
    //     DrawButton(1, BLUE, "LEVEL 2", WHITE);
    // else
    //     DrawButton(1, BLUE, "LEVEL 3", WHITE);

    // DrawButton(2, rotation_on ? GREEN : RED,
    //            rotation_on ? "ROT ON" : "ROT OFF", WHITE);

    // DrawButton(3, 0xFFE0, "AUTO", BLACK);

    // 화면 중앙 구분선
    DrawRect(119, 0, 121, SCREEN_HEIGHT, WHITE);
    DrawRect(0, 159, SCREEN_WIDTH, 161, WHITE);
}
