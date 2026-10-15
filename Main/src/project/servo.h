#ifndef SERVO_H_
#define SERVO_H_

#include "def.h"
#include "tick.h"
#include "debug.h"

#define Mode_None 0
#define Mode_Rotate 1
#define Mode_Tracking 2


void ServoInit(void);

void SetNoneMode(void);
void SetRotateMode(void);
void SetTrackingMode(void);

void ServoMain(void);
#endif