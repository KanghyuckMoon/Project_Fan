// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdint.h>

// #define LED_DDR DDRD
// #define LED_PORT PORTD

// //LED 출력 함수 작성
// void GPIO_Output(uint8_t data);
// void ledInit(void);
// void ledShift(uint8_t i, uint8_t *data);

// int main(void)
// {
//     ledInit();
//     int8_t ledData = 0x81;

//     while (1)
//     {
//         for(uint8_t i = 0; i < 8; i++)
//         {
//             ledShift(i, &ledData);
//             GPIO_Output(ledData);
//             _delay_ms(1000);
//         }
//     }


//     return 0;
// }

// void ledShift(uint8_t i, uint8_t *data)
// {
//     *data = (1 << i) | (1 << (7 - i));
//     LED_PORT = *data;
//     _delay_ms(500);
// }

// void GPIO_Output(uint8_t data)
// {
//     LED_PORT = data;
// }

// void ledInit(void)
// {
//     LED_DDR = 0xFF; // Set all pins of PORTD as output
// }