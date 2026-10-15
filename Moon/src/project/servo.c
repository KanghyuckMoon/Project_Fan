#include "servo.h"

static uint8_t servo_state = 0;
uint8_t rotateIndex = 0;

void ServoInit(void)
{
    DDRB = 0xFF;
    DDRB |= (1 << PB5);

    // Fast PWM, Mode 14
    // TOP = ICR1
    // OC1A non-inverting
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
    if (tick % 500 != 0)
    {
        return;
    }

    switch(servo_state)
    {
        case Mode_None:
            break;
        case Mode_Rotate:
            DebugLog("Rotate Index %d", rotateIndex);
            rotateIndex = rotateIndex % 3;
            if(rotateIndex == 0)
            {
                OCR1A = 100;
            }
            else if(rotateIndex == 1)
            {
                OCR1A = 350;
            }
            else if(rotateIndex == 2)
            {
                OCR1A = 600;
            }
            rotateIndex++;
            break;
        case Mode_Tracking:
            break;
    }
}