#ifndef FAN_H
#define FAN_H

#include "def.h"


void fanInit(void);

void fanOn(void);
void fanOff(void);
void fanToggle(void);

void fanLow(void);
void fanMid(void);
void fanHigh(void);

void SetSpeed(uint8_t speed);

#endif