#ifndef TOUCH_H_
#define TOUCH_H_

#include <avr/io.h>
#include <stdint.h>


// ========================
// Touch PORTA
// ========================

#define TOUCH_CS   PA3
#define TOUCH_CLK  PA4
#define TOUCH_DIN  PA5

#define TOUCH_DO   PA6
#define TOUCH_IRQ  PA7


#define TOUCH_CS_LOW() \
    (PORTA &= ~(1 << TOUCH_CS))

#define TOUCH_CS_HIGH() \
    (PORTA |= (1 << TOUCH_CS))


void TOUCH_Init(void);

uint8_t TOUCH_IsPressed(void);

uint16_t TOUCH_ReadX(void);
uint16_t TOUCH_ReadY(void);

uint8_t TOUCH_GetXY(
    uint16_t *x,
    uint16_t *y
);

extern volatile uint16_t x, y;
extern volatile uint8_t zone;

#endif