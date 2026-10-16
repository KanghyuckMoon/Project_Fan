#include "GUIButton.h"

#include "FontAndColor.h"
#include "GUIButton.h"
#include "GUIBounds.h"
#include "tft.h"
#include "touch.h"


void SetButtonEnable(Button* button, bool enable)
{
    if (button == NULL)
    {
        return;
    }

    button->enable = enable;
}
void SetLabel(Button* button, char* label)
{
    button->label = label;
}

// // 사각형 그리기: x1, y1은 포함하지 않는 끝 좌표
// void DrawRect(uint16_t x0, uint16_t y0,
//               uint16_t x1, uint16_t y1,
//               uint16_t color)
// {
//     TFT_SetWindow(x0, y0, x1 - 1, y1 - 1);

//     for (uint32_t i = 0;
//          i < (uint32_t)(x1 - x0) * (y1 - y0);
//          i++)
//     {
//         TFT_Data(color >> 8);
//         TFT_Data(color & 0xFF);
//     }
// }

// // 글자 하나 그리기
// void DrawChar(uint16_t x, uint16_t y, char c,
//               uint8_t scale, uint16_t fg, uint16_t bg)
// {
//     uint8_t index = FontIndex(c);

//     TFT_SetWindow(x, y,
//                   x + 5 * scale - 1,
//                   y + 7 * scale - 1);

//     for (uint8_t row = 0; row < 7; row++)
//     {
//         for (uint8_t sy = 0; sy < scale; sy++)
//         {
//             for (uint8_t col = 0; col < 5; col++)
//             {
//                 uint16_t color =
//                     (font[index][row] & (1 << (4 - col))) ? fg : bg;

//                 for (uint8_t sx = 0; sx < scale; sx++)
//                 {
//                     TFT_Data(color >> 8);
//                     TFT_Data(color & 0xFF);
//                 }
//             }
//         }
//     }
// }

// // 문자열을 가로 가운데에 배치
// void DrawString(uint16_t center_x, uint16_t y,
//                 const char *str, uint8_t scale,
//                 uint16_t fg, uint16_t bg)
// {
//     uint16_t len = 0;
//     while (str[len] != '\0')
//         len++;

//     uint16_t width = len * 6 * scale - scale;
//     uint16_t x = center_x - width / 2;

//     while (*str)
//     {
//         DrawChar(x, y, *str, scale, fg, bg);
//         x += 6 * scale;
//         str++;
//     }
// }


//Draw
void DrawButton(Button* button)
{
    Bound bound = button->bound; 
    uint16_t x = bound.x;
    uint16_t y = bound.y;

    // 버튼 테두리: 검은색
    DrawRect(x - 2, y - 2, x + bound.width + 2, y + bound.height + 2, BLACK);

    // 버튼 배경: 흰색
    DrawRect(x + 2, y + 2, x + bound.width - 2, y + bound.height - 2, WHITE);

    // 버튼 글자: 검은색
    DrawString(x + bound.width * 0.5, y + bound.height * 0.5, button->label, 2, BLACK, WHITE);
}

