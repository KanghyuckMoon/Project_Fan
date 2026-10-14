#include <avr/io.h>

int main(void)
{
    // PB4(OC0)를 출력으로 설정
    DDRB |= (1 << PB4);

    // Timer0 Fast PWM
    // Non-Inverting Mode
    TCCR0 =
        (1 << WGM00) |
        (1 << WGM01) |
        (1 << COM01);

    // 분주비 64
    TCCR0 |=
        (1 << CS01) |
        (1 << CS00);

    // PWM Duty 약 50%
    OCR0 = 128;

    while (1)
    {
    }

    return 0;
}