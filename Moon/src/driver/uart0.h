#ifndef UART_0_H
#define UART_0_H

#include <def.h>

void uartInit();
void uartTransmit(char data);
unsigned char uartReceive();

#endif