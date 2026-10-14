#include "btn.h"

void buttonInit(Button_t *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t btnPinNumber)
{
    button->ddr = ddr;
    button->pin = pin;
    button->btnPinNumber = btnPinNumber;
    button->prevState = RELEASED;


    *button->ddr &= ~(1 << button->btnPinNumber);
}

uint8_t buttonGetState(Button_t *button)
{
    uint8_t curState = *button->pin & (1 << button->btnPinNumber) ? RELEASED : PUSHED;

    if (curState == PUSHED && button->prevState == RELEASED)
    {
        _delay_ms(50); // 디바운싱을 위한 지연
        button->prevState = PUSHED; // 버튼의 상태를 PUSHED로 업데이트
        return ACT_PUSH; //버튼이 눌렀음을 반환
    }
    else if (curState == RELEASED && button->prevState == PUSHED)
    {
        _delay_ms(50); // 디바운싱을 위한 지연
        button->prevState = RELEASED;
        return ACT_RELEASE;
    }
    else
    {
        return NO_ACT;
    }
}