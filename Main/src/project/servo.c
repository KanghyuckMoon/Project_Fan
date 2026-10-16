#include "servo.h"

static uint8_t servo_state = 0;
uint8_t rotateIndex = 0;

static int servoDirection = 1;
static uint16_t servoOCR = 250;
static uint32_t prevTick = 0;

void ServoInit(void)
{
    DDRB |= (1 << PB5);

    TCCR1A = (1 << COM1A1) |
             (1 << WGM11);

    // WGM13:0 = 1110
    // Prescaler = 64
    TCCR1B = (1 << WGM13) |
             (1 << WGM12) |
             (1 << CS11) |
             (1 << CS10);

    TCCR1C = 0;

    ICR1 = 4999;
}

void SetNoneMode(void)
{
    servo_state = Mode_None;
}

void SetRotateMode(void)
{
    servo_state = Mode_Rotate;
}

void SetTrackingMode(void)
{
    //추후 AI트랙킹과 연동
    servo_state = Mode_Tracking;
}

void ServoMain(void)
{
    // 20ms마다 한 번만 갱신
    if (tick - prevTick < 20)
    {
        return;
    }

    prevTick = tick;

    switch (servo_state)
    {
        case Mode_None:
            break;

        case Mode_Rotate:
            servoOCR += servoDirection * 2;

            // 오른쪽 끝
            if (servoOCR >= 500)
            {
                servoOCR = 500;
                servoDirection = -1;
            }

            // 왼쪽 끝
            else if (servoOCR <= 250)
            {
                servoOCR = 250;
                servoDirection = 1;
            }

            OCR1A = servoOCR;

            DebugLog("Servo OCR: %d\r\n", servoOCR);
            break;

        case Mode_Tracking:
            break;
    }
}