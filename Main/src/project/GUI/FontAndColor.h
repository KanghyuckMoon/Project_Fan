#ifndef FONT_AND_COLOR_H_
#define FONT_AND_COLOR_H_

#include "def.h"

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define WHITE   0xFFFF
#define BLACK   0x0000
#define YELLOW  0xFFE0

// 공통 테마: RGB888 색상을 TFT RGB565로 변환한다.
#define RGB565(r, g, b) ((((uint16_t)(r) & 0xF8) << 8) | \
                         (((uint16_t)(g) & 0xFC) << 3) | ((uint16_t)(b) >> 3))
#define GUI_BG             RGB565(10, 17, 28)
#define GUI_SURFACE        RGB565(20, 31, 46)
#define GUI_SURFACE_ACTIVE RGB565(17, 43, 47)
#define GUI_BORDER         RGB565(41, 57, 76)
#define GUI_TEXT           RGB565(235, 242, 248)
#define GUI_MUTED          RGB565(126, 147, 167)
#define GUI_TEAL           RGB565(69, 221, 188)
#define GUI_BLUE           RGB565(106, 168, 255)
#define GUI_AMBER          RGB565(249, 193, 95)
#define GUI_PURPLE         RGB565(177, 154, 255)
#define GUI_SHADOW         RGB565(4, 9, 16)

uint8_t FontIndex(char c);
uint8_t FontRow(uint8_t index, uint8_t row);

#endif
