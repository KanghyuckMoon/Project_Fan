#ifndef GUIBUTTON_H_
#define GUIBUTTON_H_

#include "GUIBounds.h"

typedef struct GUIButton Button;

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
};


void SetButtonEnable(Button* button, bool enable);
void SetLabel(Button* button, const char* label);
void DrawButton(Button* button);
void InvalidateButton(Button* button);

#endif
