#include "ap.h"

// 1: 더블 버퍼 색상 테스트, 0: 기존 선풍기 애플리케이션
#ifndef AP_RENDER_TEST
#define AP_RENDER_TEST 1
#endif

int main(void)
{
    //Init
#if AP_RENDER_TEST
    int result = testInit();
#else
    int result = apInit();
#endif
    if(result == -1)
    {
        DebugLog("Error");
    }

    //Main
#if AP_RENDER_TEST
    testMain();
#else
    apMain();
#endif
}
