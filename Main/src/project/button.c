#include "button.h"



Button btnSpeed;
Button btnRotate;
    uint8_t speed = 0;
    uint8_t rotate = 0;

void ButtonSystemInit()
{
    // 버튼 1 : 풍속 변경
    buttonInit(&btnSpeed, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);

    // 버튼 2 : 회전 ON / OFF
    buttonInit(&btnRotate, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
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

        if(buttonGetState(&btnSpeed) == ACT_RELEASE)
        {
            speed++;


            if(speed > 3)
            {
                speed = 0;
            }


            switch(speed)
            {
                case 0:
                    fanOff();
                    break;


                case 1:
                    fanLow();
                    break;


                case 2:
                    fanMid();
                    break;


                case 3:
                    fanHigh();
                    break;
            }
        }


        // ========================================
        // 버튼 2 : 회전 ON / OFF
        // ========================================

        if(buttonGetState(&btnRotate) == ACT_RELEASE)
        {
            rotate = !rotate;


            if(rotate)
            {
                // 회전 시작
                SetRotateMode();
            }
            else
            {
                // 회전 정지
                SetNoneMode();
            }
        }
}