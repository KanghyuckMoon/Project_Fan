#include "ap.h"


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
    // 추후 추가
}


// ============================================================
// Button Label Update Events
// ============================================================

void LU_OnOff(Button* btn)
{
    DebugLog("ONOFF Update");
    SetLabel(
        btn,
        motor_on ? "ON" : "OFF"
    );
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
}


void LU_Servo(Button* btn)
{
    DebugLog("Servo Update");
    SetLabel(
        btn,
        rotation_on ? "ROT ON" : "ROT OFF"
    );
}


void LU_Auto(Button* btn)
{
    DebugLog("Auto Update");
    SetLabel(btn, "AUTO");
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
            .x = 0,
            .y = 0,
            .width = 120,
            .height = 160
        },

        .enable = true,

        .color = 0xFFFF,
        .label = "Button",
        .text_color = 0x0000,

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
            .x = 120,
            .y = 0,
            .width = 120,
            .height = 160
        },

        .enable = true,

        .color = 0xFFFF,
        .label = "Button",
        .text_color = 0x0000,

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
            .x = 0,
            .y = 160,
            .width = 120,
            .height = 160
        },

        .enable = true,

        .color = 0xFFFF,
        .label = "Button",
        .text_color = 0x0000,

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
            .x = 120,
            .y = 160,
            .width = 120,
            .height = 160
        },

        .enable = true,

        .color = 0xFFFF,
        .label = "Button",
        .text_color = 0x0000,

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

    while (1)
    {
        // ----------------------------------------------------
        // Touch / GUI
        // ----------------------------------------------------

        if (TOUCH_GetXY(&x, &y))
        {
            DebugLog("Touch %d %d\n", x, y);

            uint8_t result =
                GUIMain(
                    &mainCanvas,
                    x,
                    y
                );

            // GUI 변경이 발생했을 때만 다시 그림
            if (result == RESULT_CHANGE)
            {
                DrawCanvas(&mainCanvas);
            }
        }
        DrawUI();

        // ----------------------------------------------------
        // System Update
        // ----------------------------------------------------

        ServoMain();
        TickMain();
    }
}