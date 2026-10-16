// #ifndef BUTTON_H_
// #define BUTTON_H_
// #include "def.h"
// #include "fan.h"

// #define BUTTON_DDR      DDRG
// #define BUTTON_PIN      PING
// #define BUTTON_ON       0
// #define BUTTON_OFF      1
// #define BUTTON_TOGGLE   4
// #define BUTTON_RIGHT    2
// #define BUTTON_LEFT     3


// enum{PUSHED, RELEASED};
// enum{NO_ACT, ACT_PUSH, ACT_RELEASE};


// typedef struct 
// {
//     volatile uint8_t *ddr;
//     volatile uint8_t *pin;
//     uint8_t          btnPinnumber;
//     uint8_t          prevState;
// }Button;

// void ButtonSystemInit();
// void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum);
// uint8_t buttonGetState(Button *button);
// void ButtonMain(void);
// #endif