#include "ap.h"

int main(void)
{
      p_main();
      return 0;
      //apInit();
      //apMain();

      // DDRB |= (1 << PB5);           // OC1A 출력 때문

      // TCCR1A |= (1 << COM1A1) | (1 << WGM11);
      // TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
      // TCCR1C = 0;

      // ICR1 = 4999;            // 아까 계산한 TOP 값

      // //OCR1A = 625;            // 약 25%

      // while (1)
      // {
      //       OCR1A = 100;
      //       _delay_ms(2000);
            
      //       OCR1A = 350;
      //       _delay_ms(2000);

      //       OCR1A = 600;
      //       _delay_ms(2000);
      // }
      
}







// int main()
// {
//       // apInit();
//       // apMain();

//       DDRB |= (1 << PB4);
//       TCCR0 |= (1 << WGM00) | (1 << COM01) | (1 << WGM01) | (1 << CS02);
//       OCR0 = 64;

//       while (1)
//       {
//             for (uint8_t i = 0; i <= 255; i++)
//             {
//                   OCR0 = i;
//                   _delay_ms(10);
//             }
            
//       }
      
// }

// #include <avr/io.h>
// #include <util/delay.h>

// #include "button.h"
// #include "led.h"

// int main()
// {
//     LED_DDR = 0xFF;

//     Button btnOn;
//     Button btnOff;
//     Button btnLeft;
//     Button btnRight;

//     buttonInit(&btnOn, &BUTTON_DDR, &BUTTON_PIN, BUTTON_ON);
//     buttonInit(&btnOff, &BUTTON_DDR, &BUTTON_PIN, BUTTON_OFF);
//     buttonInit(&btnLeft, &BUTTON_DDR, &BUTTON_PIN, BUTTON_LEFT);
//     buttonInit(&btnRight, &BUTTON_DDR, &BUTTON_PIN, BUTTON_RIGHT);

//     uint8_t ledData = 0x01;
//     LED_PORT = ledData;

//     while (1)
//     {
//         if(buttonGetState(&btnOn) == ACT_RELEASE)
//         {
//             ledData = 0xFF;
//         }
//         if(buttonGetState(&btnOff) == ACT_RELEASE)
//         {
//             ledData = 0x00;
//         }
//         if(buttonGetState(&btnLeft) == ACT_RELEASE)
//         {
//             ledData = (ledData << 1) | (ledData >> 7);
//         }
//         if(buttonGetState(&btnRight) == ACT_RELEASE)
//         {
//             ledData = (ledData >> 1) | (ledData << 7);
//         }

//         LED_PORT = ledData;
//     }
// }















// #define LED_DDR     
// #define LED_PORT    

// // LED 출력 함수 작성 -> 원형 선언
// void GPIO_Output(unit8_t data);

// void ledInit();

// void ledShift(unit8_t i, unit8_t *data)

// int main()
// {
//     ledInit();
//     unit8_t ledData = 0x81;
    
//     while (1)
//     {
//         for(int i = 0; i < 8; i++)
//         {
//             ledShift(i, &ledData);
//             GPIO_Output(ledData);
//             _delay_ms(1000);
//         }
//     }

// }

// void ledShift(unit8_t i, unit8_t *data)
// {
//     *data = (1 << i) | (1 << (7 - i));
// }

// void ledInit()
// {
//     LED_DDR = 0xFF;
// }

// void GPIO_Output(unit8_t data)
// {
//     LED_PORT = data;
// }