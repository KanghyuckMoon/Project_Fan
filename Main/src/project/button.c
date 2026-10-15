#include "button.h"



Button btnOnOff;
Button btnLow;
Button btnMid;
Button btnHigh;

void ButtonSystemInit()
{
    buttonInit(&btnOnOff, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);
    buttonInit(&btnLow,   &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
    buttonInit(&btnMid,   &BUTTON_DDR, &BUTTON_PIN, BUTTON_RIGHT);
    buttonInit(&btnHigh,  &BUTTON_DDR, &BUTTON_PIN, BUTTON_LEFT);
}

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

void ButtonMain()
{
        // ========================================
        // 버튼 1 : 팬 ON / OFF
        // ========================================

        if(buttonGetState(&btnOnOff) == ACT_RELEASE)
        {
            fanToggle();
        }


        // ========================================
        // 버튼 2 : 약풍
        // ========================================

        if(buttonGetState(&btnLow) == ACT_RELEASE)
        {
            fanLow();
        }


        // ========================================
        // 버튼 3 : 중풍
        // ========================================

        if(buttonGetState(&btnMid) == ACT_RELEASE)
        {
            fanMid();
        }


        // ========================================
        // 버튼 4 : 강풍
        // ========================================

        if(buttonGetState(&btnHigh) == ACT_RELEASE)
        {
            fanHigh();
        }
}