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

    if((curState == PUSHED) && (button->prevState == RELEASED))
    {
        _delay_ms(50);
        button->prevState = PUSHED;
        return ACT_PUSH;
    }
    else if((curState != PUSHED) && (button->prevState == PUSHED))
    {
        _delay_ms(50);
        button->prevState = RELEASED;
        return ACT_RELEASE;
    }
    return NO_ACT;
}
// #include "button.h"



// void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum)
// {
//     button->ddr = ddr;
//     button->pin = pin;
//     button->btnPinnumber = pinNum;
//     button->prevState = RELEASED;
//     *button->ddr &= ~(1 << button->btnPinnumber);
// }


// uint8_t buttonGetState(Button *button)
// {
//     uint8_t curState = *button->pin & (1 << button->btnPinnumber);

//     if((curState == PUSHED) && (button->prevState == RELEASED))     // 안누른 상태에서 누르면
//     {
//         _delay_ms(50);                      // 디바운스코드
//         button->prevState = PUSHED;         // 버튼의 상태를 누른 상태로 변환
//         return ACT_PUSH;                    // 버튼이 눌렸음을 반환
//     }
//     else if((curState != PUSHED) && (button->prevState == PUSHED))  // 버튼을 누른 상태에서 떼면...
//     {
//         _delay_ms(50);
//         button->prevState = RELEASED;
//         return ACT_RELEASE;
//     }
//     return NO_ACT;
// }

