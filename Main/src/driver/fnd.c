#include "fnd.h"


uint8_t fnd_1_Number[] = 
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67
};


void fnd_1_Init()
{
    FND_DDR = 0xFF;
}

void fnd_1_Display(uint8_t data)
{
    FND_PORT = fnd_1_Number[data];
}