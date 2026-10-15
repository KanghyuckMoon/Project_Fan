#include "ap.h"
// #include "servo.h"
#include "tft.h"

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
    uartInit();
    TFT_Init();

    DDRC |= (1 << PC5);                  // T_CS 출력
    DDRC &= ~((1 << PC6) | (1 << PC7));  // IRQ, DO 입력

    TOUCH_CS_HIGH();
        DebugLog("START");

    while (1)
    {
        DebugLog("START\n");
        TouchTest();
        TFT_FillScreen(RED); // Red
        _delay_ms(100);
    }
}

// int main(void)
// {
//     ServoInit();
//     uartInit();
//     SetRotateMode();
//     while(1)
//     {
//         ServoMain();
//         TickMain();
//     }
// }   

// int main(void)
// {    
//     DDRB = 0xFF;
//     DDRB |= (1 << PB5);

//     // Fast PWM, Mode 14
//     // TOP = ICR1
//     // OC1A non-inverting
//     TCCR1A = (1 << COM1A1) |
//              (1 << WGM11);

//     // WGM13:0 = 1110
//     // Prescaler = 64
//     TCCR1B = (1 << WGM13) |
//              (1 << WGM12) |
//              (1 << CS11) |
//              (1 << CS10);

//     TCCR1C = 0;

//     ICR1 = 4999;
//     while(1)
//     {
//         OCR1A = 100;
//         _delay_ms(500);

//         // 중앙
//         OCR1A = 350;
//         _delay_ms(500);

//         // +90도 근처
//         OCR1A = 600;
//         _delay_ms(500);
//     }

// }