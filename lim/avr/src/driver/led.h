#include "def.h"

#define LED_DDR     DDRD
#define LED_PORT    PORTD



// LED 구조체 정의
typedef struct 
{
    volatile uint8_t  *port;        // LED가 연결될 포트의 포인터변수 선언
    uint8_t           pinNumber;    // LED가 연결될 핀 번호를 일반변수로 선언
}LED;


// LED 관련된 함수의 원형 선언
void ledInit(LED *led);
void ledOn(LED *led);
void ledOff(LED *led);