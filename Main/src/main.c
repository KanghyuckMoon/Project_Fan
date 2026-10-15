#include "ap.h"



// int main(void)
// {
//     TFT_Init();

//     while (1)
//     {
//         TFT_FillScreen(RED); // Red
//         _delay_ms(1000);

//         TFT_FillScreen(GREEN); // Green
//         _delay_ms(1000);

//         TFT_FillScreen(BLUE); // Blue
//         _delay_ms(1000);
//     }
// }

int main(void)
{
    //Init
    fanInit();
    ButtonSystemInit();
    ServoInit();
    uartInit();

    // TFT 초기화
    TFT_Init();

    // 터치 초기화
    TOUCH_Init();

    // 초기 화면
    DrawUI();

    //Main
    SetRotateMode();
    while(1)
    {
        if (TOUCH_GetXY(&x, &y))
        {
            DebugLog("Touch %d %d\n", x, y);
            /*
             * 현재 사용 중인 터치 방향 보정 유지
             * 좌표 중앙값 2048을 기준으로 구역 분리
             */
            if (x >= 2048 && y >= 2048)
                zone = 0;  // 1번: 왼쪽 위
            else if (x < 2048 && y >= 2048)
                zone = 1;  // 2번: 오른쪽 위
            else if (x >= 2048 && y < 2048)
                zone = 2;  // 3번: 왼쪽 아래
            else
                zone = 3;  // 4번: 오른쪽 아래

            switch (zone)
            {
                case 0:
                    // ON / OFF 전환
                    motor_on = !motor_on;
                    fanToggle();
                    DrawUI();
                    break;

                case 1:
                    // 강도 1 → 2 → 3 → 1
                    motor_level++;

                    if (motor_level > 2)
                        motor_level = 0;

                    SetSpeed(motor_level);
                    DrawUI();
                    break;

                case 2:
                    // 회전 ON / OFF 전환
                    rotation_on = !rotation_on;
                    if(rotation_on == true)
                    {
                        SetRotateMode();
                    }
                    else
                    {
                        SetNoneMode();
                    }
                    DrawUI();
                    break;

                case 3:
                    // AUTO: 현재는 글자만 표시
                    break;
            }

            // 상태 변경 후 화면 갱신

            //DrawUI();
            //손을 뗄 때까지 대기
            //while (TOUCH_IsPressed())
            //    _delay_ms(10);
            //_delay_ms(100);
        }
        ServoMain();
        ButtonMain();
        TickMain();
    }
}