// #include <avr/io.h>
// #include <util/delay.h>

// uint8_t ledArr[] =
// {
//     0x81, // 0b10000001
//     0x42, // 0b01000010
//     0x24, // 0b00100100
//     0x18, // 0b00011000
//     0x24, // 0b00100100
//     0x42, // 0b01000010
//     0x81 // 0b10000001
// };

// int main(void)
// {
//     DDRD = 0xFF; // Set all pins of PORTD as output
//     uint8_t arrSize = sizeof(ledArr) / sizeof(ledArr[0]);

    
//     while (1)
//     {
//         for (uint8_t i = 0; i < arrSize; i++)
//         {
//             PORTD = ledArr[i];
//             _delay_ms(500);
//         }
//     }

//     return 0;
// }