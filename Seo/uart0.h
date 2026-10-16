#include "def.h"

void uartInit();
void uartTransmit(char data);
unsigned char uartReceive();

int uartPutchar(char c, FILE *stream);