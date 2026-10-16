#ifndef GUIBUTTON_H_
#define GUIBUTTON_H_

#include "GUIBounds.h"

typedef struct GUIButton Button;

typedef enum { ICON_NONE, ICON_POWER, ICON_SWING, ICON_SPEED, ICON_AUTO } ButtonIcon;

// 카드 문자열은 최대 15자까지 표시한다. 이전 문자열도 복사하여 보관한다.
#define BUTTON_TEXT_CAPACITY 16
typedef struct
{
    Bound bound;
    uint16_t color, text_color, accent;
    ButtonIcon icon;
    bool active, visible;
    uint8_t level;
    char label[BUTTON_TEXT_CAPACITY];
    char title[BUTTON_TEXT_CAPACITY];
    char hint[BUTTON_TEXT_CAPACITY];
} ButtonAppearance;

struct GUIButton
{
    Bound bound;
    bool enable;
    uint16_t color;
    const char *label;
    uint16_t text_color;


    void (*OnClick)();
    void (*OnLabelUpdate)(Button* btn);

    // 상태 변경 후 다음 부분 갱신에서 다시 그릴지 여부
    bool dirty;
    const char *title;
    const char *hint;
    uint16_t accent;
    ButtonIcon icon;
    bool active;
    uint8_t level;
    ButtonAppearance rendered;
    bool rendered_valid;
};


void SetButtonEnable(Button* button, bool enable);
void SetLabel(Button* button, const char* label);
void DrawButton(Button* button);
void InvalidateButton(Button* button);
void SetButtonActive(Button* button, bool active);
void SetButtonLevel(Button* button, uint8_t level);
void GetButtonAppearance(const Button *button, ButtonAppearance *appearance);
void DrawButtonAppearance(const ButtonAppearance *appearance);

#endif
