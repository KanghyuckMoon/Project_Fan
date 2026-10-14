#include "ap.h"

void apInit()
{
    LCD_Init();
}
void apMain()
{
    char buffer[17];
    uint16_t count = 0;

    LCD_WriteStringXY(0, 0, "HELLO 4bit");


    
    while(1)
    {
        sprintf(buffer, "Count : %-5u", (unsigned int)count); 

        LCD_WriteStringXY(1,0, buffer);
        count++;

        _delay_ms(200);
    }
              
}
 