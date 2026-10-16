#include <assert.h>
#include "tft.h"
#include "GUI.h"
#include "ap.h"
#include <setjmp.h>

uint8_t PORTA, PORTB, DDRA, DDRB;
volatile uint8_t SPCR, SPSR, SPDR;
static uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];
static uint16_t expected[SCREEN_HEIGHT][SCREEN_WIDTH];
static uint16_t before_screen[SCREEN_HEIGHT][SCREEN_WIDTH];
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
    if (command != 0x2C) return; // Initialization registers do not write pixels.
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

static void BeginDiff(void)
{
    memcpy(before_screen, screen, sizeof(screen));
    memset(writes, 0, sizeof(writes));
}

static unsigned long AssertDiffOnly(void)
{
    unsigned long changed = 0;
    for (unsigned y = 0; y < SCREEN_HEIGHT; y++)
        for (unsigned x = 0; x < SCREEN_WIDTH; x++)
        {
            bool different = before_screen[y][x] != screen[y][x];
            assert(writes[y][x] == (different ? 1 : 0));
            changed += different;
        }
    return changed;
}

static void AssertCanvasMatches(Canvas *canvas)
{
    memcpy(expected, screen, sizeof(screen));
    DrawUI();
    for (unsigned i = 0; i < MAX_BUTTONS; i++)
        if (canvas->buttonIntance[i] != NULL) DrawButton(canvas->buttonIntance[i]);
    assert(memcmp(expected, screen, sizeof(screen)) == 0);
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
    DrawUI();
    for (unsigned i = 0; i < 4; i++) DrawButton(&buttons[i]);
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
    BeginDiff();
    unsigned long before_pixels = pixels;
    assert(DrawDirtyCanvas(&canvas));
    unsigned long changed = AssertDiffOnly();
    assert(changed > 0 && changed < 19200 && pixels - before_pixels == changed);
    printf("label-only changed pixels: %lu (formerly 19200)\n", changed);
    assert(!buttons[1].dirty && !buttons[2].dirty && !buttons[3].dirty);
    SetLabel(&buttons[0], "ON");
    BeginDiff();
    assert(DrawDirtyCanvas(&canvas));
    AssertDiffOnly();
    assert(memcmp(expected, screen, sizeof(screen)) == 0); // Erase longer OFF label.
    SetButtonEnable(&buttons[0], false);
    BeginDiff();
    assert(DrawDirtyCanvas(&canvas));
    AssertDiffOnly();
    AssertCanvasMatches(&canvas);
    SetButtonEnable(&buttons[0], true);
    BeginDiff();
    assert(DrawDirtyCanvas(&canvas));
    AssertDiffOnly();
    before = bytes;
    InvalidateButton(&buttons[0]);
    assert(!DrawDirtyCanvas(&canvas) && bytes == before);

    char mutable_label[16] = "ON";
    buttons[0].label = mutable_label;
    assert(!DrawDirtyCanvas(&canvas)); // Same pixels despite a different pointer.
    mutable_label[1] = 'F';
    BeginDiff();
    assert(DrawDirtyCanvas(&canvas)); // No setter or invalidation required.
    AssertDiffOnly();
    AssertCanvasMatches(&canvas);
    buttons[0].bound.x = 8; // Restore the old position and respect overlapping cards.
    buttons[1].accent = GUI_PURPLE;
    BeginDiff();
    assert(DrawDirtyCanvas(&canvas));
    AssertDiffOnly();
    AssertCanvasMatches(&canvas);
    puts("PASS: exact pixel diffs, unchanged suppression, mutable text, hide/show, move/overlap");
}

static uint8_t scale;
static void PaintPattern(void *context)
{
    assert(context == &scale);
    DrawUI();
    DrawRect(1, 1, 238, 319, GREEN);
    DrawRect(3, 4, 38, 41, RED);
    DrawChar(5, 6, 'A', scale, BLUE, WHITE);
    DrawChar(232, 313, 'R', 1, WHITE, RED);
    DrawString(120, 40, "LEVEL 3", scale, BLACK, GREEN);
    DrawString(120, 50, "", 2, BLACK, WHITE);
    DrawString(120, 50, NULL, 2, BLACK, WHITE);
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

static void PaintSolid(void *context)
{
    painting = true;
    DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, *(uint16_t *)context);
    painting = false;
}

static void TestChangedRegion(void)
{
    uint16_t old_color = GREEN, new_color = RED;
    Reset();
    TFT_RenderRegion(7, 9, 120, 72, BLACK, PaintSolid, &old_color);
    BeginDiff();
    assert(TFT_RenderChangedRegion(7, 9, 120, 72, BLACK, PaintSolid, &old_color, &new_color));
    assert(AssertDiffOnly() == 113UL * 63);
    unsigned long before = bytes;
    assert(!TFT_RenderChangedRegion(7, 9, 120, 72, BLACK, PaintSolid, &new_color, &new_color));
    assert(!TFT_RenderChangedRegion(7, 9, 7, 72, BLACK, PaintSolid, &old_color, &new_color));
    assert(bytes == before);
    puts("PASS: full-stripe diff, final partial stripe, identical/invalid diff suppression");
}

// Run the real application initialization and touch loop with hardware stubs.
volatile uint32_t tick;
static jmp_buf application_done;
static unsigned touch_step, power_calls, speed_calls, rotate_calls;
static unsigned long application_changed;
void uartInit(void) {}
void fanInit(void) {}
void ServoInit(void) {}
void TOUCH_Init(void) {}
void DebugLog(const char *format, ...) { (void)format; }
void fanToggle(void) { power_calls++; }
void SetSpeed(uint8_t speed) { assert(speed == 2); speed_calls++; }
void SetRotateMode(void) { rotate_calls++; }
void SetNoneMode(void) { assert(0); }
void ServoMain(void) {}
uint8_t TOUCH_IsPressed(void) { return touch_step % 4 != 3; }
uint8_t TOUCH_GetXY(uint16_t *x, uint16_t *y)
{
    *x = (touch_step >= 4 && touch_step < 8) || touch_step >= 12 ? 150 : 30;
    *y = touch_step >= 8 ? 220 : 100;
    return TOUCH_IsPressed();
}
void TickMain(void)
{
    unsigned long changed = AssertDiffOnly();
    if (touch_step % 4 == 0) assert(changed > 0);
    else assert(changed == 0);
    application_changed += changed;
    if (++touch_step == 12) memcpy(expected, screen, sizeof(screen));
    if (touch_step == 20)
    {
        assert(memcmp(expected, screen, sizeof(screen)) == 0); // Auto ON then OFF.
        longjmp(application_done, 1);
    }
    BeginDiff();
}

static void ExportPreview(const char *name)
{
    const char *directory = getenv("RENDER_PREVIEW_DIR");
    if (directory == NULL) return;
    char path[1024];
    assert(snprintf(path, sizeof(path), "%s/%s.ppm", directory, name) < (int)sizeof(path));
    FILE *file = fopen(path, "wb");
    assert(file);
    fprintf(file, "P6\n240 320\n255\n");
    for (unsigned y = 0; y < SCREEN_HEIGHT; y++)
        for (unsigned x = 0; x < SCREEN_WIDTH; x++)
        {
            uint16_t color = screen[y][x];
            fputc(((color >> 11) & 31) * 255 / 31, file);
            fputc(((color >> 5) & 63) * 255 / 63, file);
            fputc((color & 31) * 255 / 31, file);
        }
    fclose(file);
}

static void TestApplication(void)
{
    Reset();
    DDRB = 0xB0;
    PORTB = 0xB0;
    assert(apInit() == 0);
    assert(DDRB == 0xB7 && (PORTB & 0xB0) == 0xB0); // Preserve PWM pins.
    assert(SPCR == ((1 << SPE) | (1 << MSTR)));
    assert(SPSR == (TFT_SPI_DOUBLE_SPEED ? (1 << SPI2X) : 0));
    assert(pixels == 76800);
    ExportPreview("smart-fan-idle");
    BeginDiff();
    if (setjmp(application_done) == 0) apMain();
    assert(power_calls == 1 && speed_calls == 1 && rotate_calls == 1);
    assert(motor_on == 1 && rotation_on == 1 && motor_level == 2);
    assert(pixels == 76800 + application_changed);
    assert(application_changed < 5 * 104 * 104);
    printf("five application clicks: %lu changed pixels (formerly 54080)\n", application_changed);
    ExportPreview("smart-fan-active");
    puts("PASS: actual application layout, active states, speed level, one action per press");
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
    TestChangedRegion();
    TestApplication();
    return 0;
}
