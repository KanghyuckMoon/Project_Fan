#include "button.h"



void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum)
{
    button->ddr = ddr;
    button->pin = pin;
    button->btnPinnumber = pinNum;
    button->prevState = RELEASED;
    *button->ddr &= ~(1 << button->btnPinnumber);
}


uint8_t buttonGetState(Button *button)
{
    uint8_t curState = *button->pin & (1 << button->btnPinnumber);

    if((curState == PUSHED) && (button->prevState == RELEASED))     // 안누른 상태에서 누르면
    {
        _delay_ms(50);                      // 디바운스코드
        button->prevState = PUSHED;         // 버튼의 상태를 누른 상태로 변환
        return ACT_PUSH;                    // 버튼이 눌렸음을 반환
    }
    else if((curState != PUSHED) && (button->prevState == PUSHED))  // 버튼을 누른 상태에서 떼면...
    {
        _delay_ms(50);
        button->prevState = RELEASED;
        return ACT_RELEASE;
    }
    return NO_ACT;
}

// Button btnOn;
// Button btnOff;
// Button btnTog;

// void apInit()
// {

//     buttonInit(&btnOn, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);
//     buttonInit(&btnOff, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
//     buttonInit(&btnTog, &BUTTON_DDR, &BUTTON_PIN, BUTTON_TOGGLE);
// }

// void apMain()
// {
//     while (1)
//     {
//         if(buttonGetState(&btnOn) == ACT_RELEASE)
//         {
//             LED_PORT = 0xFF;
//         }
//         if(buttonGetState(&btnOff) == ACT_RELEASE)
//         {
//             LED_PORT = 0x00;
//         }
//         if(buttonGetState(&btnTog) == ACT_RELEASE)
//         {
//             LED_PORT ^= 0xFF;
//         }
//     }  
// }
