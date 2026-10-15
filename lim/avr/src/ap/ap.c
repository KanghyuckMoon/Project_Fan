#include "ap.h"


Button btnSpeed;
Button btnRotate;


void apInit(void)
{
    // ========================================
    // 팬 초기화
    // ========================================

    fanInit();


    // ========================================
    // 서보 초기화
    // ========================================

    ServoInit();
    SetRotateMode();

    // ========================================
    // 버튼 초기화
    // ========================================

    // 버튼 1 : 풍속 변경
    buttonInit(&btnSpeed, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);

    // 버튼 2 : 회전 ON / OFF
    buttonInit(&btnRotate, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
}


void apMain(void)
{
    // ========================================
    // 풍속 상태
    //
    // 0 = 정지
    // 1 = 약풍
    // 2 = 중풍
    // 3 = 강풍
    // ========================================

    uint8_t speed = 0;


    // ========================================
    // 회전 상태
    //
    // 0 = 정지
    // 1 = 회전
    // ========================================

    uint8_t rotate = 0;


    while(1)
    {
        DebugLog("Check0");
        // ========================================
        // Tick 처리
        // ========================================

        TickMain();


        // ========================================
        // 버튼 1 : 풍속 변경
        //
        // 정지 → 약풍 → 중풍 → 강풍 → 정지
        // ========================================

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


        // ========================================
        // 서보 회전 처리
        // ========================================
        DebugLog("Check1");
        ServoMain();
    }
}