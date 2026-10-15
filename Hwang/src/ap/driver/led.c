#include "led.h"


void ledInit(LED *led)
{
    // 포트에 해당되는 핀을 출력으로 설정
    *(led->port - 1) |= (1 << led->pinNumber);
    // DDR 레지스터는 PORT 레지스터보다 1 낮게 위치 함(데이터시트 참조)
    // *(led->port - 1) 를 이용하여 PORT에서 DDR로 접근함
    // |= (1 << led->pinNumber) OR 연산을 통해서 led->pinNumber로 지정된 포트를 
    // 출력으로 설정 함
}

void ledOn(LED *led)
{
    // 해당 핀을 High로 설정
    *(led->port) |= (1 << led->pinNumber);
}

void ledOff(LED *led)
{
    // 해당 핀을 Low로 설정
    *(led->port) &= ~(1 << led->pinNumber);
}













// void ledInit(LED *led)
// {
//     // 포트에 해당되는 핀을 출력으로 설정
//     *(led -> port - 1) |= (1 << led -> pinNumber);
//     // DDR 레지스터는 PORT 레지스터보다 1 낮게 위치함(데이트시트 참조)
//     // *(led -> port - 1)를 이용하여 PORT에서 DDR로 접근함
//     // |= (1 << led -> pinNumber); OR 연산을 통해서
//     // LED->pinNumber로 지정된 포트를 출력으로 설정함  
// }

// void ledOn(LED *led)
// {
//     // 해당 핀을 High로 설정
//     *(led -> port) |= (1 << led -> pinNumber);
// }


// void ledOff(LED *led)
// {
//     // 해당 핀을 Low로 설정
//     *(led -> port) &= ~(1 << led -> pinNumber);
// }