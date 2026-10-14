#include "ap.h"



int main()
{
   apInit();
   apMain();

   // DDRB |= (1<<PB5);

   // TCCR1A |= (1<<COM1A1)|(1<<WGM11);
   // TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
   // TCCR1C = 0;

   // ICR1 = 4999;

   // // OCR1A = 650;


   // while(1)
   // {
   //     _delay_ms(1000);
   //     OCR1A = 100;
   //     _delay_ms(1000);
   //     OCR1A = 350;
   //     _delay_ms(1000);
   //     OCR1A = 600;
       
   // }
}