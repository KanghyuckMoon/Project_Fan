#ifndef FAN_H
#define FAN_H

#include "def.h"

#define FAN_LOW     128
#define FAN_MID     192
#define FAN_HIGH    255

void fanInit(void);

void fanOn(void);
void fanOff(void);
void fanToggle(void);

void fanLow(void);
void fanMid(void);
void fanHigh(void);

void SetSpeed(uint8_t speed);

#endif