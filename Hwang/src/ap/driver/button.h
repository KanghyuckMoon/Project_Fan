#include "def.h"

#define BUTTON_DDR      DDRG
#define BUTTON_PIN      PING
#define BUTTON_RIGHT    0
#define BUTTON_ON       2
#define BUTTON_OFF      3
#define BUTTON_LEFT     4

enum{PUSHED, RELEASED};
enum{NO_ACT, ACT_PUSH, ACT_RELEASE};

typedef struct 
{
    volatile uint8_t *ddr;
    volatile uint8_t *pin;
    uint8_t          btnPinnumber;
    uint8_t          prevState;
}Button;

void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum);
uint8_t buttonGetState(Button *button);






// #include "def.h"


// #define BUTTON_DDR      DDRG
// #define BUTTON_PIN      PING
// #define BUTTON_ON       2
// #define BUTTON_OFF      3
// #define BUTTON_TOGGLE   4


// enum{PUSHED, RELEASED};
// enum{NO_ACT, ACT_PUSH, ACT_RELEASE};


// typedef struct 
// {
//     volatile uint8_t *ddr;
//     volatile uint8_t *pin;
//     uint8_t          btnPinnumber;
//     uint8_t          prevState;
// }Button;

// void buttonInit(Button *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum);
// uint8_t buttonGetState(Button *button);