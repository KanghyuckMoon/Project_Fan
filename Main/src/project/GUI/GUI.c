#include "GUI.h"
#include "debug.h"

uint8_t GUIMain(Canvas* canvas, uint16_t x, uint16_t y)
{
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
        Button* button = (Button*)(canvas->buttonIntance[i]);

        if (button == NULL)
        {
            continue;
        }

        if (!button->enable)
        {
            continue;
        }

        if (Bound_ContainsPoint(&button->bound, x, y))
        {
            if (button->OnClick != NULL)
            {
                button->OnClick();
                if (button->OnLabelUpdate != NULL)
                {
                    button->OnLabelUpdate(button);
                }
                return RESULT_CHANGE;
            }

            // 겹쳐 있는 버튼 여러 개가 동시에 실행되지 않도록
            return RESULT_NONE;
        }
    }
    return RESULT_NONE;
}

bool AddButton(Canvas* canvas, Button* button)
{
    if (button == NULL)
    {
        return false;
    }

    // 이미 등록되어 있는지 확인
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
        if (canvas->buttonIntance[i] == button)
        {
            return false;
        }
    }

    // 빈 슬롯 탐색
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
        if (canvas->buttonIntance[i] == NULL)
        {
            canvas->buttonIntance[i] = button;
            if (button->OnLabelUpdate != NULL)
            {
                button->OnLabelUpdate(button);
            }
            return true;
        }
    }

    // 최대 버튼 개수 초과
    return false;
}

void DrawCanvas(Canvas* canvas)
{
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
        Button* button = (Button*)(canvas->buttonIntance[i]);

        if (button == NULL)
        {
            continue;
        }

        if (!button->enable)
        {
            continue;
        }
        DrawButton(button);
    }
}


Button* GetButton(Canvas* canvas, uint8_t index)
{
    if (index >= MAX_BUTTONS)
    {
        return NULL;
    }

    return (Button*)(canvas->buttonIntance[index]);
}

