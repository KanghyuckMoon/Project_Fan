#ifndef UART_0_H
#define UART_0_H

#include <def.h>

void uartInit();
void uartTransmit(char data);
void uartTransmitString(const char* string);
unsigned char uartReceive();

#endif