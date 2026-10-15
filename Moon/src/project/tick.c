#include "tick.h"

volatile uint32_t tick = 0;

#define TICK tick

void TickMain(void)
{
    tick++;
    _delay_ms(1);
}