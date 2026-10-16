#include "ap.h"

// 각 프레임 전송이 끝난 뒤 색상을 500ms 동안 유지한다.
#define TEST_COLOR_HOLD_MS 500

static const uint16_t testColors[] = {RED, GREEN, BLUE, YELLOW, WHITE, BLACK};
static uint8_t testColorIndex;

static void PaintTestFrame(void *context)
{
    const uint16_t color = *(const uint16_t *)context;
    DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

static void DrawTestFrame(void)
{
    // 동기식 전송이므로 반환 전까지 color가 유효하다.
    uint16_t color = testColors[testColorIndex];
    TFT_RenderRegion(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                     color, PaintTestFrame, &color);
}

int testInit(void)
{
    uartInit();
    TFT_Init();
    testColorIndex = 0;
    DrawTestFrame();
    return 0;
}

void testMain(void)
{
    while (1)
    {
        _delay_ms(TEST_COLOR_HOLD_MS);
        testColorIndex++;
        if (testColorIndex >= sizeof(testColors) / sizeof(testColors[0]))
            testColorIndex = 0;
        DrawTestFrame();
    }
}


// ============================================================
// GUI Objects
// ============================================================

static Canvas mainCanvas = {0};

static Button buttonOnOff;
static Button buttonServo;
static Button buttonSpeed;
static Button buttonAuto;


// ============================================================
// Button Click Events
// ============================================================

void BTN_OnOff(void)
{
    motor_on = !motor_on;

    fanToggle();
}


void BTN_Speed(void)
{
    motor_level++;

    if (motor_level > 2)
    {
        motor_level = 0;
    }

    SetSpeed(motor_level);
}


void BTN_Servo(void)
{
    rotation_on = !rotation_on;

    if (rotation_on)
    {
        SetRotateMode();
    }
    else
    {
        SetNoneMode();
    }
}


void BTN_Auto(void)
{
    // 표시 상태만 전환한다. 실제 자동 제어는 연결하지 않는다.
    SetButtonActive(&buttonAuto, !buttonAuto.active);
}


// ============================================================
// Button Label Update Events
// ============================================================

void LU_OnOff(Button* btn)
{
    DebugLog("ONOFF Update");
    SetLabel(btn, motor_on ? "ON" : "OFF");
    SetButtonActive(btn, motor_on != 0);
    btn->hint = motor_on ? "TAP TO STOP" : "TAP TO START";
}


void LU_Speed(Button* btn)
{
    DebugLog("Speed Update");
    const char* label;

    if (motor_level == 0)
    {
        label = "LEVEL 1";
    }
    else if (motor_level == 1)
    {
        label = "LEVEL 2";
    }
    else
    {
        label = "LEVEL 3";
    }

    SetLabel(btn, label);
    SetButtonActive(btn, true);
    SetButtonLevel(btn, motor_level + 1);
}


void LU_Servo(Button* btn)
{
    DebugLog("Servo Update");
    SetLabel(btn, rotation_on ? "ON" : "OFF");
    SetButtonActive(btn, rotation_on != 0);
    btn->hint = rotation_on ? "SWEEP ACTIVE" : "TAP TO SWING";
}


void LU_Auto(Button* btn)
{
    DebugLog("Auto Update");
    SetLabel(btn, btn->active ? "ON" : "OFF");
}


// ============================================================
// Application Init
// ============================================================

int apInit(void)
{
    // --------------------------------------------------------
    // Hardware Init
    // --------------------------------------------------------

    fanInit();
    ServoInit();

    // Debug UART
    uartInit();

    // TFT
    TFT_Init();

    // Touch
    TOUCH_Init();


    // --------------------------------------------------------
    // Canvas Init
    // --------------------------------------------------------

    mainCanvas = (Canvas){0};


    // --------------------------------------------------------
    // ON / OFF Button
    // --------------------------------------------------------

    buttonOnOff = (Button)
    {
        .bound =
        {
            .x = 12,
            .y = 76,
            .width = 104,
            .height = 104
        },

        .enable = true,

        .color = GUI_SURFACE,
        .label = "Button",
        .text_color = GUI_TEXT,
        .title = "POWER",
        .hint = "TAP TO START",
        .icon = ICON_POWER,
        .accent = GUI_TEAL,

        .OnClick = BTN_OnOff,
        .OnLabelUpdate = LU_OnOff
    };


    // --------------------------------------------------------
    // Servo Button
    // --------------------------------------------------------

    buttonServo = (Button)
    {
        .bound =
        {
            .x = 124,
            .y = 76,
            .width = 104,
            .height = 104
        },

        .enable = true,

        .color = GUI_SURFACE,
        .label = "Button",
        .text_color = GUI_TEXT,
        .title = "SWING",
        .hint = "TAP TO SWING",
        .icon = ICON_SWING,
        .accent = GUI_BLUE,

        .OnClick = BTN_Servo,
        .OnLabelUpdate = LU_Servo
    };


    // --------------------------------------------------------
    // Speed Button
    // --------------------------------------------------------

    buttonSpeed = (Button)
    {
        .bound =
        {
            .x = 12,
            .y = 188,
            .width = 104,
            .height = 104
        },

        .enable = true,

        .color = GUI_SURFACE,
        .label = "Button",
        .text_color = GUI_TEXT,
        .title = "AIRFLOW",
        .hint = "",
        .icon = ICON_SPEED,
        .accent = GUI_AMBER,

        .OnClick = BTN_Speed,
        .OnLabelUpdate = LU_Speed
    };


    // --------------------------------------------------------
    // Auto Button
    // --------------------------------------------------------

    buttonAuto = (Button)
    {
        .bound =
        {
            .x = 124,
            .y = 188,
            .width = 104,
            .height = 104
        },

        .enable = true,

        .color = GUI_SURFACE,
        .label = "Button",
        .text_color = GUI_TEXT,
        .title = "AUTO MODE",
        .hint = "",
        .icon = ICON_AUTO,
        .accent = GUI_PURPLE,

        .OnClick = BTN_Auto,
        .OnLabelUpdate = LU_Auto
    };


    // --------------------------------------------------------
    // Add Buttons
    // --------------------------------------------------------

    if (!AddButton(&mainCanvas, &buttonOnOff))
    {
        DebugLog("Add buttonOnOff failed\n");
        return -1;
    }

    if (!AddButton(&mainCanvas, &buttonServo))
    {
        DebugLog("Add buttonServo failed\n");
        return -1;
    }

    if (!AddButton(&mainCanvas, &buttonSpeed))
    {
        DebugLog("Add buttonSpeed failed\n");
        return -1;
    }

    if (!AddButton(&mainCanvas, &buttonAuto))
    {
        DebugLog("Add buttonAuto failed\n");
        return -1;
    }


    // --------------------------------------------------------
    // Initial Draw
    // --------------------------------------------------------

    DrawCanvas(&mainCanvas);

    return 0;
}


// ============================================================
// Application Main
// ============================================================

void apMain(void)
{
    uint16_t x = 0;
    uint16_t y = 0;
    bool touchHandled = false;

    while (1)
    {
        //터치임

        if (!TOUCH_IsPressed())
            touchHandled = false;

        if (TOUCH_GetXY(&x, &y) && !touchHandled)
        {
            DebugLog("Touch %d %d\n", x, y);

            GUIMain(&mainCanvas, x, y);
            touchHandled = true;
        }

        //변경된것만 있을 때 갱신
        DrawDirtyCanvas(&mainCanvas);

        // ----------------------------------------------------
        // System Update
        // ----------------------------------------------------

        ServoMain();
        TickMain();
    }
}
