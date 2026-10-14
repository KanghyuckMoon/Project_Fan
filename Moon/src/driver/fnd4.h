#ifndef FND_4_H_
#define FND_4_H_

#include "def.h"

#define FND_DATA_DDR     DDRB
#define FND_DATA_PORT    PORTB

#define FND_SELECT_DDR   DDRC
#define FND_SELECT_PORT  PORTC
#define FND_SELECT_MASK  0x0F

void fnd_4_Init(void);
void fnd_4_AllDigitOff(void);
void fnd_4_Scan(uint16_t data);

#endif