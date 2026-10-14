// #include "ap.h"

// int main(void)
// {
//     // OC1A = PB5
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

//     int count = 0;
//     while (1)
//     {
//         OCR1A = 100;
//         _delay_ms(1000);

//         // 중앙
//         OCR1A = 350;
//         _delay_ms(1000);

//         // +90도 근처
//         OCR1A = 600;
//         _delay_ms(1000);
//     }
// }   