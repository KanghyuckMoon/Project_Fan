#include <avr/io.h>
#include <util/delay.h>
#include "tft.h"

#define TOUCH_IRQ PA7

int p_main(void)
{
    DDRA &= ~(1 << TOUCH_IRQ);
    PORTA |= (1 << TOUCH_IRQ);

    TFT_Init();

    while (1)
    {
        if (!(PINA & (1 << TOUCH_IRQ)))
        {
            TFT_FillScreen(GREEN);
        }
        else
        {
            TFT_FillScreen(BLUE);
        }

        _delay_ms(100);
    }
}