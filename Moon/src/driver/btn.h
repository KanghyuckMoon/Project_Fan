#ifndef BTN_H_
#define BTN_H_

#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT PORTD
#define LED_DDR DDRD
#define BUTTON_DDR DDRG
#define BUTTON_PIN PING
#define BUTTON_ON 3
#define BUTTON_OFF 4
#define BUTTON_TOGGLE 2
#define BUTTON_SHIFT_LEFT 1
#define BUTTON_SHIFT_RIGHT 0


enum {PUSHED, RELEASED};
enum {NO_ACT, ACT_PUSH, ACT_RELEASE};



typedef struct
{
    volatile uint8_t *ddr;
    volatile uint8_t *pin;
    uint8_t btnPinNumber;
    uint8_t prevState;
} Button_t;


void buttonInit(Button_t *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t btnPinNumber);
uint8_t buttonGetState(Button_t *button);

#endif