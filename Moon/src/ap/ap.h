#ifndef AP_H_
#define AP_H_
#include "def.h"

#include "led.h"
#include "btn.h"
#include "fnd.h"
#include "fnd4.h"
#include "uart0.h"
//#include "lcd_8bit.h"
//#include "lcd_4bit.h"
#include "i2c_lcd.h"


extern Button_t btnOn;
extern Button_t btnOff;
extern Button_t btnToggle;
extern Button_t btnShiftL;
extern Button_t btnShiftR;


void apInit(void);
void apMain(void);

void jmod_remote_control_main(void);

#endif