
#include "def.h"

// LED 구조체 정의
typedef struct
{
    volatile uint8_t *port; // LED 연결된 포트의 포인터 변수 선언
    uint8_t pinNumber;      // LED 연결된 핀 번호를 일반변수로 선언
} LED;

void ledInit(LED *led);
void ledOn(LED *led);
void ledOff(LED *led);

// 8개 LED 일괄 초기화 및 제어를 위한 보조 함수
void ledAllInit(LED *leds, uint8_t count);
void ledAllOn(LED *leds, uint8_t count);
void ledAllOff(LED *leds, uint8_t count);

