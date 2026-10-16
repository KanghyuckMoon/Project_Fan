#include "GUI.h"
#include "debug.h"
#include "tft.h"

typedef struct
{
    Canvas *canvas;
    bool previous;
} CanvasScene;

static void PaintScene(void *context)
{
    const CanvasScene *scene = context;
    DrawUI();
    // 항상 동일한 순서로 합성하여 이동/숨김/겹침도 배경까지 복원한다.
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
        Button *button = scene->canvas->buttonIntance[i];
        if (button == NULL) continue;
        if (scene->previous)
        {
            if (button->rendered_valid)
                DrawButtonAppearance(&button->rendered);
        }
        else
            DrawButton(button);
    }
}

static void SaveScene(Canvas *canvas)
{
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
        Button *button = canvas->buttonIntance[i];
        if (button == NULL) continue;
        GetButtonAppearance(button, &button->rendered);
        button->rendered_valid = true;
        button->dirty = false;
    }
}

static void IncludeBounds(const ButtonAppearance *appearance,
                          uint16_t *left, uint16_t *top, uint16_t *right, uint16_t *bottom)
{
    const Bound *b = &appearance->bound;
    if (!appearance->visible || b->width == 0 || b->height == 0 ||
        b->x >= SCREEN_WIDTH || b->y >= SCREEN_HEIGHT) return;
    uint32_t end_x = (uint32_t)b->x + b->width;
    uint32_t end_y = (uint32_t)b->y + b->height;
    if (end_x > SCREEN_WIDTH) end_x = SCREEN_WIDTH;
    if (end_y > SCREEN_HEIGHT) end_y = SCREEN_HEIGHT;
    if (b->x < *left) *left = b->x;
    if (b->y < *top) *top = b->y;
    if (end_x > *right) *right = end_x;
    if (end_y > *bottom) *bottom = end_y;
}

static void IncludeTextBand(const ButtonAppearance *appearance, uint16_t offset, uint16_t height,
                            uint16_t *left, uint16_t *top, uint16_t *right, uint16_t *bottom)
{
    ButtonAppearance band = {0};
    band.visible = true;
    band.bound = appearance->bound;
    band.bound.y += offset;
    band.bound.height = height;
    IncludeBounds(&band, left, top, right, bottom);
}

static void IncludeChanges(const ButtonAppearance *previous, const ButtonAppearance *current,
                           uint16_t *left, uint16_t *top, uint16_t *right, uint16_t *bottom)
{
    const Bound *b = &current->bound;
    // 스타일/표시/위치가 바뀌면 이전 위치와 새 위치 모두 비교한다.
    if (memcmp(&previous->bound, b, sizeof(*b)) != 0 ||
        previous->visible != current->visible || previous->active != current->active ||
        previous->color != current->color || previous->text_color != current->text_color ||
        previous->accent != current->accent || previous->icon != current->icon ||
        b->width < 80 || b->height < 92 ||
        (uint32_t)b->x + b->width > SCREEN_WIDTH || (uint32_t)b->y + b->height > SCREEN_HEIGHT)
    {
        IncludeBounds(previous, left, top, right, bottom);
        IncludeBounds(current, left, top, right, bottom);
        return;
    }
    if (!current->visible) return;
    if (strcmp(previous->label, current->label) != 0)
        IncludeTextBand(current, 58, 14, left, top, right, bottom);
    if (strcmp(previous->title, current->title) != 0)
        IncludeTextBand(current, 43, 7, left, top, right, bottom);
    if (strcmp(previous->hint, current->hint) != 0 || previous->level != current->level)
        IncludeTextBand(current, b->height - 20, 7, left, top, right, bottom);
}

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
            button->rendered_valid = false;
            InvalidateButton(button);
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
    CanvasScene scene = {canvas, false};
    TFT_RenderRegion(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GUI_BG, PaintScene, &scene);
    SaveScene(canvas);
}

bool DrawDirtyCanvas(Canvas* canvas)
{
    uint16_t left = SCREEN_WIDTH, top = SCREEN_HEIGHT, right = 0, bottom = 0;
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
        Button *button = canvas->buttonIntance[i];
        if (button == NULL) continue;
        ButtonAppearance current;
        GetButtonAppearance(button, &current);
        if (!button->rendered_valid || memcmp(&current, &button->rendered, sizeof(current)) != 0)
        {
            if (button->rendered_valid)
                IncludeChanges(&button->rendered, &current, &left, &top, &right, &bottom);
            else
                IncludeBounds(&current, &left, &top, &right, &bottom);
        }
    }

    bool transferred = false;
    if (left < right && top < bottom)
    {
        CanvasScene previous = {canvas, true};
        CanvasScene current = {canvas, false};
        transferred = TFT_RenderChangedRegion(left, top, right, bottom,
                                              GUI_BG, PaintScene, &previous, &current);
    }
    SaveScene(canvas);
    return transferred;
}


Button* GetButton(Canvas* canvas, uint8_t index)
{
    if (index >= MAX_BUTTONS)
    {
        return NULL;
    }

    return (Button*)(canvas->buttonIntance[index]);
}
