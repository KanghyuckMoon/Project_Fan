#include <assert.h>
#include "tft.h"

uint8_t PORTA, PORTB, DDRA, DDRB;
volatile uint8_t SPCR, SPSR, SPDR;

int main(void)
{
    // Host registers cannot model the peripheral clock. Mark the transfer done
    // and verify that the production function uses SPDR without touching GPIO.
    PORTB = 0xB8;
    DDRB = 0x37;
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPIF) | (1 << SPI2X);
    for (unsigned value = 0; value < 256; value++)
    {
        SPI_Write(value);
        assert(SPDR == value);
        assert(PORTB == 0xB8 && DDRB == 0x37);
    }
    puts("PASS: production SPI byte function writes SPDR and preserves GPIO");
    return 0;
}
