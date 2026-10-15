#include "ap.h"


Button btnOnOff;
Button btnLow;
Button btnMid;
Button btnHigh;


void apInit(void)
{
    // 팬 초기화
    fanInit();


    // 버튼 초기화
    buttonInit(&btnOnOff, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);
    buttonInit(&btnLow,   &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
    buttonInit(&btnMid,   &BUTTON_DDR, &BUTTON_PIN, BUTTON_RIGHT);
    buttonInit(&btnHigh,  &BUTTON_DDR, &BUTTON_PIN, BUTTON_LEFT);
}


void apMain(void)
{
    while(1)
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
}