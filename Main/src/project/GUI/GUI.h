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
// 이전 표시 상태와 비교하여 달라진 픽셀만 전송한다. 직접 필드/문자열 변경도 감지한다.
bool DrawDirtyCanvas(Canvas* canvas);
bool AddButton(Canvas* canvas, Button* button);
Button* GetButton(Canvas* canvas, uint8_t index);

#endif
