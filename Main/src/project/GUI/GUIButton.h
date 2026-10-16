#ifndef GUIBUTTON_H_
#define GUIBUTTON_H_

#include "GUIBounds.h"

typedef struct GUIButton Button;

struct GUIButton
{
    Bound bound;
    bool enable;
    uint16_t color;
    char *label;
    uint16_t text_color;


    void (*OnClick)();
    void (*OnLabelUpdate)(Button* btn);

};


void SetButtonEnable(Button* button, bool enable);
void SetLabel(Button* button, char* label);
void DrawButton(Button* button);

#endif