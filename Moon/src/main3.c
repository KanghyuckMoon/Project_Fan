// #include "led.h"

// int main()
// {
//     // LED 구조체 인스턴스 생성과 초기화
//     LED_t leds[] = {
//         {&LED_PORT, 0}, // LED 1: 포트 D, 핀 0
//         {&LED_PORT, 1}, // LED 2: 포트 D, 핀 1
//         {&LED_PORT, 2}, // LED 3: 포트 D, 핀 2
//         {&LED_PORT, 3}, // LED 4: 포트 D, 핀 3
//         {&LED_PORT, 4}, // LED 5: 포트 D, 핀 4
//         {&LED_PORT, 5}, // LED 6: 포트 D, 핀 5
//         {&LED_PORT, 6}, // LED 7: 포트 D, 핀 6
//         {&LED_PORT, 7}  // LED 8: 포트 D, 핀 7
//     };

//     //핀들 초기화하고 출력모드로 설정
//     for(uint8_t i = 0; i < 8; i++)
//     {
//         ledInit(&leds[i]); // LED 초기화
//     }

//     while(1)
//     {
//         for(uint8_t i = 0; i < 8; i++)
//         {
//             ledOn(&leds[i]); // LED 켜기
//         }
//         _delay_ms(2000); // 2000ms 지연

//         for(uint8_t i = 0; i < 8; i++)
//         {
//             ledOff(&leds[i]); // LED 끄기
//         }
//         _delay_ms(2000); // 2000ms 지연
//     }

// }