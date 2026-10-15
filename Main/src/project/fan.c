#include "fan.h"
#include "debug.h"

#define FAN_LOW     128
#define FAN_MID     192
#define FAN_HIGH    255

bool isOn;
int previouseSpeed;


void fanInit(void)
{
    // PB4 = OC0
    DDRB |= (1 << PB4);


    // Timer0 Fast PWM
    // Non-Inverting Mode

    TCCR0 =
        (1 << WGM00) |
        (1 << WGM01) |
        (1 << COM01);


    // 분주비 64

    TCCR0 |=
        (1 << CS01) |
        (1 << CS00);


    // 초기 상태 : OFF

    OCR0 = 0;
}


void fanOn(void)
{
    isOn = true;
    OCR0 = FAN_LOW;
}


void fanOff(void)
{
    isOn = false;
    OCR0 = 0;
}


void fanToggle(void)
{
    if(OCR0 == 0)
    {
        fanOn();
    }
    else
    {
        fanOff();
    }
    SetSpeed(previouseSpeed);
}


void fanLow(void)
{
    OCR0 = FAN_LOW;
}


void fanMid(void)
{
    OCR0 = FAN_MID;
}


void fanHigh(void)
{
    OCR0 = FAN_HIGH;
}

void SetSpeed(uint8_t speed)
{
    if(!isOn)
    {
        previouseSpeed = speed;
        return;
    }

    switch(speed)
    {
        case 0:
            //fanOff();
            fanLow();
            break;
        case 1:
            fanMid();
            break;
        case 2:
            fanHigh();
            break;
        //case 3:
        //    break;
    }
}