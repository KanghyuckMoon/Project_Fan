#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdint.h>

void TOUCH_Init(void);
uint8_t TOUCH_IsPressed(void);
uint16_t TOUCH_ReadX(void);
uint16_t TOUCH_ReadY(void);
uint8_t TOUCH_GetXY(uint16_t *x, uint16_t *y);

#endif