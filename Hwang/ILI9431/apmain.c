#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "tft.h"
#include "touch.h"

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

// 상태 변수
uint8_t motor_on = 0;
uint8_t motor_level = 1;
uint8_t rotation_on = 0;

// 글꼴: A, E, F, L, N, O, R, T, U, V, 1, 2, 3, /, 공백
static const uint8_t font[15][7] = {
    {14,17,17,31,17,17,17}, // A
    {31,16,16,30,16,16,31}, // E
    {31,16,16,30,16,16,16}, // F
    {16,16,16,16,16,16,31}, // L
    {17,25,25,21,19,19,17}, // N
    {14,17,17,17,17,17,14}, // O
    {30,17,17,30,20,18,17}, // R
    {31,4,4,4,4,4,4},       // T
    {17,17,17,17,17,17,14}, // U
    {17,17,17,17,17,10,4},  // V
    {4,12,4,4,4,4,14},      // 1
    {14,17,1,2,4,8,31},     // 2
    {30,1,1,14,1,1,30},     // 3
    {1,2,2,4,8,8,16},       // /
    {0,0,0,0,0,0,0}         // 공백
};

static uint8_t FontIndex(char c)
{
    switch (c)
    {
        case 'A': return 0;
        case 'E': return 1;
        case 'F': return 2;
        case 'L': return 3;
        case 'N': return 4;
        case 'O': return 5;
        case 'R': return 6;
        case 'T': return 7;
        case 'U': return 8;
        case 'V': return 9;
        case '1': return 10;
        case '2': return 11;
        case '3': return 12;
        case '/': return 13;
        case ' ': return 14;
        default:  return 14;
    }
}

// 사각형 그리기: x1, y1은 포함하지 않는 끝 좌표
void DrawRect(uint16_t x0, uint16_t y0,
              uint16_t x1, uint16_t y1,
              uint16_t color)
{
    TFT_SetWindow(x0, y0, x1 - 1, y1 - 1);

    for (uint32_t i = 0;
         i < (uint32_t)(x1 - x0) * (y1 - y0);
         i++)
    {
        TFT_Data(color >> 8);
        TFT_Data(color & 0xFF);
    }
}

// 글자 하나 그리기
void DrawChar(uint16_t x, uint16_t y, char c,
              uint8_t scale, uint16_t fg, uint16_t bg)
{
    uint8_t index = FontIndex(c);

    TFT_SetWindow(x, y,
                  x + 5 * scale - 1,
                  y + 7 * scale - 1);

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
                    TFT_Data(color >> 8);
                    TFT_Data(color & 0xFF);
                }
            }
        }
    }
}

// 문자열을 가로 가운데에 배치
void DrawString(uint16_t center_x, uint16_t y,
                const char *str, uint8_t scale,
                uint16_t fg, uint16_t bg)
{
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
void DrawButton(uint8_t zone, uint16_t color,
                const char *label, uint16_t text_color)
{
    uint16_t x = (zone % 2) * 120;
    uint16_t y = (zone / 2) * 160;

    // 버튼 배경: 흰색
    DrawRect(x, y, x + 120, y + 160, WHITE);

    // 버튼 테두리: 검은색
    DrawRect(x, y, x + 120, y + 2, BLACK);
    DrawRect(x, y + 158, x + 120, y + 160, BLACK);
    DrawRect(x, y, x + 2, y + 160, BLACK);
    DrawRect(x + 118, y, x + 120, y + 160, BLACK);

    // 버튼 글자: 검은색
    DrawString(x + 60, y + 76, label, 2, BLACK, WHITE);
}

// 현재 상태에 맞춰 버튼 4개 표시
void DrawUI(void)
{
    DrawButton(0, motor_on ? GREEN : RED,
               motor_on ? "ON" : "OFF", WHITE);

    if (motor_level == 1)
        DrawButton(1, BLUE, "LEVEL 1", WHITE);
    else if (motor_level == 2)
        DrawButton(1, BLUE, "LEVEL 2", WHITE);
    else
        DrawButton(1, BLUE, "LEVEL 3", WHITE);

    DrawButton(2, rotation_on ? GREEN : RED,
               rotation_on ? "ROT ON" : "ROT OFF", WHITE);

    DrawButton(3, 0xFFE0, "AUTO", BLACK);

    // 화면 중앙 구분선
    DrawRect(119, 0, 121, SCREEN_HEIGHT, WHITE);
    DrawRect(0, 159, SCREEN_WIDTH, 161, WHITE);
}

int p_main(void)
{
    uint16_t x, y;
    uint8_t zone;

    // TFT 초기화
    TFT_Init();

    // 터치 초기화
    TOUCH_Init();

    // 초기 화면
    DrawUI();

    while (1)
    {
        if (TOUCH_GetXY(&x, &y))
        {
            /*
             * 현재 사용 중인 터치 방향 보정 유지
             * 좌표 중앙값 2048을 기준으로 구역 분리
             */
            if (x >= 2048 && y >= 2048)
                zone = 0;  // 1번: 왼쪽 위
            else if (x < 2048 && y >= 2048)
                zone = 1;  // 2번: 오른쪽 위
            else if (x >= 2048 && y < 2048)
                zone = 2;  // 3번: 왼쪽 아래
            else
                zone = 3;  // 4번: 오른쪽 아래

            switch (zone)
            {
                case 0:
                    // ON / OFF 전환
                    motor_on = !motor_on;
                    break;

                case 1:
                    // 강도 1 → 2 → 3 → 1
                    motor_level++;

                    if (motor_level > 3)
                        motor_level = 1;
                    break;

                case 2:
                    // 회전 ON / OFF 전환
                    rotation_on = !rotation_on;
                    break;

                case 3:
                    // AUTO: 현재는 글자만 표시
                    break;
            }

            // 상태 변경 후 화면 갱신
            DrawUI();

            // 손을 뗄 때까지 대기
            while (TOUCH_IsPressed())
                _delay_ms(10);

            _delay_ms(100);
        }
    }

    return 0;
}