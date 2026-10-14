// #include "btn.h"

// int main()
// {
//     LED_DDR = 0xFF; // LED 포트 출력 설정

//     Button_t btnOn;
//     Button_t btnOff;
//     Button_t btnToggle;
//     Button_t btnShiftL;
//     Button_t btnShiftR;

//     buttonInit(&btnOn, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);
//     buttonInit(&btnOff, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
//     buttonInit(&btnToggle, &BUTTON_DDR, &BUTTON_PIN, BUTTON_TOGGLE);
//     buttonInit(&btnShiftL, &BUTTON_DDR, &BUTTON_PIN, BUTTON_SHIFT_LEFT);
//     buttonInit(&btnShiftR, &BUTTON_DDR, &BUTTON_PIN, BUTTON_SHIFT_RIGHT);

//     while(1)
//     {
//         if(buttonGetState(&btnOn) == ACT_RELEASE)
//         {
//             LED_PORT = 0xFF; // 모든 LED 켜기
//         }
//         if(buttonGetState(&btnOff) == ACT_RELEASE)
//         {
//             LED_PORT = 0x00; // 모든 LED 끄기
//         }
//         if(buttonGetState(&btnToggle) == ACT_RELEASE)
//         {
//             LED_PORT ^= 0xFF; // 모든 LED 토글
//         }
//         if(buttonGetState(&btnShiftL) == ACT_RELEASE)
//         {
//             LED_PORT <<= 1; // 모든 LED 왼쪽으로 이동
//         }
//         if(buttonGetState(&btnShiftR) == ACT_RELEASE)
//         {
//             LED_PORT >>= 1; // 모든 LED 오른쪽으로 이동
//         }
//     }

    


// }