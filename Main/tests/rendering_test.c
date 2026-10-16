#include <assert.h>
#include "tft.h"
#include "GUI.h"

uint8_t PORTA, PORTB, DDRA, DDRB;
static uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];
static uint16_t expected[SCREEN_HEIGHT][SCREEN_WIDTH];
static uint8_t writes[SCREEN_HEIGHT][SCREEN_WIDTH];
static uint8_t command, args[4], arg_count, high, half;
static uint16_t x0, y0, x1, y1, px, py;
static unsigned long pixels, bytes, windows;
static bool painting;

// Decode the actual CASET/PASET/RAMWR byte stream, including RGB565 order.
void SPI_Write(uint8_t value)
{
    assert(!painting);  // Composition must stay in RAM until the stripe is ready.
    assert(!(PORTB & (1 << TFT_CS)));
    bytes++;
    if (!(PORTA & (1 << TFT_DC)))
    {
        assert(!half);
        command = value;
        arg_count = 0;
        if (command == 0x2C) { px = x0; py = y0; windows++; }
        return;
    }
    if (command == 0x2A || command == 0x2B)
    {
        assert(arg_count < 4);
        args[arg_count++] = value;
        if (arg_count == 4)
        {
            uint16_t a = ((uint16_t)args[0] << 8) | args[1];
            uint16_t b = ((uint16_t)args[2] << 8) | args[3];
            assert(a <= b);
            if (command == 0x2A) { x0 = a; x1 = b; assert(b < SCREEN_WIDTH); }
            else { y0 = a; y1 = b; assert(b < SCREEN_HEIGHT); }
        }
        return;
    }
    assert(command == 0x2C);
    if (!half) { high = value; half = 1; return; }
    assert(px <= x1 && py <= y1);
    screen[py][px] = ((uint16_t)high << 8) | value;
    writes[py][px]++;
    pixels++;
    half = 0;
    if (++px > x1) { px = x0; py++; }
}

static void Reset(void)
{
    memset(screen, 0x5A, sizeof(screen));
    memset(writes, 0, sizeof(writes));
    pixels = bytes = windows = 0;
}

static Button *clicked;
static void Click(void) { SetLabel(clicked, "OFF"); }

static void TestCanvas(void)
{
    Canvas canvas = {0};
    Button buttons[4] = {0};
    const char *labels[] = {"ON", "ROT OFF", "LEVEL 2", "AUTO"};
    for (unsigned i = 0; i < 4; i++)
    {
        buttons[i].bound = (Bound){.x=(i % 2)*120, .y=(i / 2)*160,
                                   .width=120, .height=160};
        buttons[i].enable = true;
        buttons[i].color = WHITE;
        buttons[i].text_color = BLACK;
        buttons[i].label = labels[i];
        buttons[i].OnClick = Click;
        assert(AddButton(&canvas, &buttons[i]));
    }
    // The direct renderer is the visual oracle for the buffered renderer.
    Reset();
    for (unsigned i = 0; i < 4; i++) DrawButton(&buttons[i]);
    DrawUI();
    memcpy(expected, screen, sizeof(screen));
    Reset();
    DrawCanvas(&canvas);
    assert(memcmp(expected, screen, sizeof(screen)) == 0);
    assert(pixels == 76800 && windows == 160);
    for (unsigned y = 0; y < SCREEN_HEIGHT; y++)
        for (unsigned x = 0; x < SCREEN_WIDTH; x++)
            assert(writes[y][x] == 1);
    unsigned long before = bytes;
    assert(!DrawDirtyCanvas(&canvas) && bytes == before);
    SetLabel(&buttons[0], "ON");
    assert(!DrawDirtyCanvas(&canvas));
    clicked = &buttons[0];
    assert(GUIMain(&canvas, 20, 20) == RESULT_CHANGE);
    unsigned long before_pixels = pixels;
    assert(DrawDirtyCanvas(&canvas));
    assert(pixels - before_pixels == 19200);
    assert(!buttons[1].dirty && !buttons[2].dirty && !buttons[3].dirty);
    SetLabel(&buttons[0], "ON");
    assert(DrawDirtyCanvas(&canvas));
    assert(memcmp(expected, screen, sizeof(screen)) == 0); // Erase longer OFF label.
    SetButtonEnable(&buttons[0], false);
    before = bytes;
    assert(!DrawDirtyCanvas(&canvas) && bytes == before);
    SetButtonEnable(&buttons[0], true);
    assert(DrawDirtyCanvas(&canvas));
    puts("PASS: canvas pixels, single writes, dirty updates, shorter labels, enable");
}

static uint8_t scale;
static void PaintPattern(void *context)
{
    assert(context == &scale);
    DrawRect(1, 1, 238, 319, GREEN);
    DrawRect(3, 4, 38, 41, RED);
    DrawChar(5, 6, 'A', scale, BLUE, WHITE);
    DrawChar(232, 313, 'R', 1, WHITE, RED);
    DrawString(120, 40, "LEVEL 3", scale, BLACK, GREEN);
    DrawString(120, 50, "", 2, BLACK, WHITE);
    DrawString(120, 50, NULL, 2, BLACK, WHITE);
    DrawUI();
}

static void BufferedPattern(void *context)
{
    painting = true;
    PaintPattern(context);
    painting = false;
}

static void TestRegion(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
    Reset();
    DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, YELLOW);
    PaintPattern(&scale);
    memcpy(expected, screen, sizeof(screen));
    Reset();
    TFT_RenderRegion(left, top, right, bottom, YELLOW, BufferedPattern, &scale);
    uint16_t end_x = right > SCREEN_WIDTH ? SCREEN_WIDTH : right;
    uint16_t end_y = bottom > SCREEN_HEIGHT ? SCREEN_HEIGHT : bottom;
    assert(pixels == (unsigned long)(end_x - left) * (end_y - top));
    for (unsigned y = 0; y < SCREEN_HEIGHT; y++)
        for (unsigned x = 0; x < SCREEN_WIDTH; x++)
        {
            bool inside = x >= left && x < end_x && y >= top && y < end_y;
            assert(writes[y][x] == (inside ? 1 : 0));
            assert(screen[y][x] == (inside ? expected[y][x] : 0x5A5A));
        }
    assert(!half && (PORTB & (1 << TFT_CS)));
}

int main(void)
{
    TestCanvas();
    for (scale = 1; scale <= 3; scale++)
    {
        TestRegion(0, 0, 240, 320);
        TestRegion(7, 9, 120, 72); // Partial glyphs and an incomplete final stripe.
        TestRegion(232, 313, 300, 400); // Right/bottom clipping.
        TestRegion(5, 6, 6, 31); // Narrow region and one stripe.
    }
    unsigned long before = bytes;
    TFT_RenderRegion(1, 1, 1, 2, BLACK, BufferedPattern, &scale);
    TFT_RenderRegion(2, 2, 1, 1, BLACK, BufferedPattern, &scale);
    TFT_RenderRegion(240, 0, 241, 1, BLACK, BufferedPattern, &scale);
    TFT_RenderRegion(0, 0, 1, 1, BLACK, NULL, &scale);
    assert(bytes == before);
    puts("PASS: alternating stripes, clipping, scaling, final stripe, invalid regions");
    return 0;
}
