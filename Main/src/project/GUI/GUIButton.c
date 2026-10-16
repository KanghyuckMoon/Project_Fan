#include "GUIButton.h"
#include "FontAndColor.h"
#include "tft.h"


void SetButtonEnable(Button* button, bool enable)
{
    if (button == NULL)
    {
        return;
    }

    if (button->enable != enable)
    {
        button->enable = enable;
        InvalidateButton(button);
    }
}
void SetLabel(Button* button, const char* label)
{
    if (button == NULL)
        return;

    if (button->label == NULL || label == NULL ||
        strcmp(button->label, label) != 0)
    {
        InvalidateButton(button);
    }
    button->label = label;
}

void InvalidateButton(Button* button)
{
    if (button != NULL)
        button->dirty = true;
}

void SetButtonActive(Button* button, bool active)
{
    if (button != NULL && button->active != active)
    {
        button->active = active;
        InvalidateButton(button);
    }
}

void SetButtonLevel(Button* button, uint8_t level)
{
    if (level > 3) level = 3;
    if (button != NULL && button->level != level)
    {
        button->level = level;
        InvalidateButton(button);
    }
}

#include <avr/pgmspace.h>

// 16x16 단색 기능 아이콘. RGB 이미지 버퍼 없이 플래시에서 읽는다.
static const uint16_t icons[4][16] PROGMEM = {
    {0x0180, 0x0180, 0x0990, 0x1998, 0x318C, 0x318C, 0x6186, 0x6186, 0x6006, 0x6006, 0x300C, 0x300C, 0x1C38, 0x0FF0, 0x03C0, 0x0000},
    {0x0000, 0x03C0, 0x0FF2, 0x0C3A, 0x000E, 0x000E, 0x003E, 0x0000, 0x0000, 0x7C00, 0x7000, 0x7000, 0x5C30, 0x4FF0, 0x03C0, 0x0000},
    {0x0000, 0x001C, 0x001C, 0x001C, 0x001C, 0x001C, 0x039C, 0x039C, 0x039C, 0x039C, 0x739C, 0x739C, 0x739C, 0x739C, 0x739C, 0x0000},
    {0x0000, 0x0180, 0x0180, 0x0180, 0x0180, 0x03C0, 0x07E0, 0x7FFE, 0x7FFE, 0x07E0, 0x03C0, 0x0180, 0x0180, 0x0180, 0x0180, 0x0000},
};

static void DrawIcon(uint16_t x, uint16_t y, ButtonIcon icon, uint16_t color)
{
    if (icon < ICON_POWER || icon > ICON_AUTO) return;
    for (uint8_t row = 0; row < 16; row++)
    {
        uint16_t bits = pgm_read_word(&icons[icon - ICON_POWER][row]);
        for (uint8_t col = 0; col < 16; col++)
            if (bits & ((uint16_t)1 << (15 - col)))
                DrawRect(x + col, y + row, x + col + 1, y + row + 1, color);
    }
}

void DrawButton(Button* button)
{
    if (button == NULL) return;
    const Bound *bound = &button->bound;
    uint16_t x = bound->x, y = bound->y;
    uint16_t w = bound->width, h = bound->height;
    if (w == 0 || h == 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    uint16_t right = (uint32_t)x + w > SCREEN_WIDTH ? SCREEN_WIDTH : x + w;
    uint16_t bottom = (uint32_t)y + h > SCREEN_HEIGHT ? SCREEN_HEIGHT : y + h;
    w = right - x;
    h = bottom - y;
    uint16_t accent = button->accent ? button->accent : GUI_TEAL;
    uint16_t surface = button->active ? GUI_SURFACE_ACTIVE : button->color;
    uint16_t border = button->active ? accent : GUI_BORDER;

    // 모서리와 그림자를 포함한 영역을 먼저 지워 이전 상태를 남기지 않는다.
    DrawRect(x, y, right, bottom, GUI_BG);
    if (w < 80 || h < 92)
    {
        DrawRoundRect(x, y, right, bottom, surface);
        if (w >= 16 && h >= 14)
            DrawString(x + w / 2, y + (h - 7) / 2, button->label, 1,
                       button->text_color, surface);
        return;
    }

    DrawRoundRect(x, y + 3, right, bottom, GUI_SHADOW);
    DrawRoundRect(x, y, right, bottom - 3, border);
    DrawRoundRect(x + 1, y + 1, right - 1, bottom - 4, surface);
    DrawIcon(x + 14, y + 15, button->icon, button->active ? accent : GUI_MUTED);
    DrawRect(right - 21, y + 20, right - 15, y + 23,
             button->active ? accent : GUI_BORDER);

    DrawString(x + w / 2, y + 43, button->title, 1, GUI_MUTED, surface);
    uint16_t length = button->label ? strlen(button->label) : 0;
    uint8_t scale = (uint32_t)length * 12 > w - 12U ? 1 : 2;
    DrawString(x + w / 2, y + 58, button->label, scale,
               button->active ? accent : button->text_color, surface);

    if (button->icon == ICON_SPEED)
    {
        uint16_t start = x + (w - 68) / 2;
        for (uint8_t i = 0; i < 3; i++)
            DrawRect(start + i * 24, bottom - 20, start + i * 24 + 20, bottom - 16,
                     i < button->level ? accent : GUI_BORDER);
    }
    else
        DrawString(x + w / 2, bottom - 20, button->hint, 1, GUI_MUTED, surface);
}
