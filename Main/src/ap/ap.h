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
#include "tft.h"
#include "touch.h"


#include "GUI.h"
#include "GUIBounds.h"
#include "GUIButton.h"

int apInit(void);
void apMain(void);

int testInit(void);
void testMain(void);

void jmod_remote_control_main(void);

#endif
