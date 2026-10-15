#ifndef AP_H_
#define AP_H_
#include "def.h"

#include "led.h"
#include "fnd.h"
#include "fnd4.h"
#include "uart0.h"
#include "i2c_lcd.h"
#include "debug.h"
#include "tick.h"
#include "servo.h"
#include "fan.h"
#include "button.h"
#include "tft.h"
#include "touch.h"


void apInit(void);
void apMain(void);

void jmod_remote_control_main(void);

#endif