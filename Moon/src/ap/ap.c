#include "ap.h"



void apInit()
{
    LCD_Init();
}

void apMain()
{
    // char buffer[17];
    // uint16_t count = 0;

    LCD_GotoXY(0,0);
    LCD_WriteString("Hello AVR");

    //LCD_WriteStringXY(0, 0, "Hello AVR");


   while (1)
   {
    
   }
}