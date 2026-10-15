#include "uart0.h"

static int uartPutChar(char data, FILE *stream)
{
    (void)stream;
    uartTransmit(data);
    return 0;
}

static FILE OUTPUT = FDEV_SETUP_STREAM(uartPutChar, NULL, _FDEV_SETUP_WRITE);

void uartInit()
{
    // speed setting -> 9600bps
    UBRR0H = 0x00;
    UBRR0L = 0xCF;

    UCSR0A |= (1 << U2X0);                 // 2배속 모드 설정
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Rx Enable, Tx Enable setting
    UCSR0B |= (1 << RXCIE0);               // Rx Interrupt Enable
    // UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);     // 비동기, 패리티없음, 스톱비트1개, 8비트데이터

    stdout = &OUTPUT; // printf를 uart0 으로 연결 함
}

void uartTransmit(char data)
{
    while (!(UCSR0A & (1 << UDRE0)));   // 송신 가능?? UDR이 비어있는지??
    UDR0 = data;
}

unsigned char uartReceive()
{
    while (!(UCSR0A & (1 << RXC0)));    // 데이터 수신대기
    return UDR0;
}