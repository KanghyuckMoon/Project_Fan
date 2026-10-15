#ifndef TOUCH_H_
#define TOUCH_H_

// 터치스크린 핀: ATmega128A PORTA
#define TOUCH_CS  PA3
#define TOUCH_CLK PA4
#define TOUCH_DIN PA5
#define TOUCH_DO  PA6
#define TOUCH_IRQ PA7

#define TOUCH_CS_LOW()  (PORTA &= ~(1 << TOUCH_CS))
#define TOUCH_CS_HIGH() (PORTA |=  (1 << TOUCH_CS))

#include <stdint.h>

void TOUCH_Init(void);
uint8_t TOUCH_IsPressed(void);
uint16_t TOUCH_ReadX(void);
uint16_t TOUCH_ReadY(void);
uint8_t TOUCH_GetXY(uint16_t *x, uint16_t *y);

#endif