#ifndef GUI_H_
#define GUI_H_

#include "def.h"
#include "GUIButton.h"

#define MAX_BUTTONS 10

#define RESULT_NONE 0
#define RESULT_CHANGE 1

typedef struct GUICanvas
{
    Button* buttonIntance[MAX_BUTTONS];
} Canvas;

uint8_t GUIMain(Canvas* canvas, uint16_t x, uint16_t y);
void DrawCanvas(Canvas* canvas);
// 변경된 버튼만 갱신한다. 직접 필드를 수정하면 InvalidateButton을 호출한다.
bool DrawDirtyCanvas(Canvas* canvas);
bool AddButton(Canvas* canvas, Button* button);
Button* GetButton(Canvas* canvas, uint8_t index);

#endif
