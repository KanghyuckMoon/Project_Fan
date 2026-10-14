#ifndef LED_H_ //LED_H_가 정의되어 있지 않을 때만
#define LED_H_ //LED_H_를 정의한다

#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD


//LED 구조체 정의
typedef struct
{
    volatile uint8_t *port;  // LED가 연결될 포트의 포인터
    uint8_t pinNumber; // LED 출력 데이터
} LED_t;

// LED 관련된 함수의 원형 선언

void ledInit(LED_t *led);
void ledShift(LED_t *led);
void GPIO_Output(LED_t *led);

void ledOn(LED_t *led);
void ledOff(LED_t *led);

#endif /* LED_H_ */