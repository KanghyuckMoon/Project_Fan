#include "led.h"

void ledInit(LED_t *led)
{
    //포트에 해당되는 핀을 출력으로 설정
    *(led->port - 1) |= (1 << led->pinNumber);

    // DDR 레지스터는 PORT 레지스터보다 1 낮게 위치함
    // (led->port - 1) 를 이용하여 PORT에서 DDR로 접근함
    // |= (1 << led->pinNumber) OR 연산을 통해서 led->pinNumber로 지정된 포트를 출력으로 설정함
}

void ledOn(LED_t *led)
{
    // LED를 켜기 위해 해당 핀을 HIGH로 설정
    *(led->port) |= (1 << led->pinNumber);
}

void ledOff(LED_t *led)
{
    // LED를 끄기 위해 해당 핀을 LOW로 설정
    *(led->port) &= ~(1 << led->pinNumber);
}
