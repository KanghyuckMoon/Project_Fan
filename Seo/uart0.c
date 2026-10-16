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
    // 송수신 비활성화 상태에서 레지스터 설정 변경
    UCSR0B = 0x00;
    
    // 16MHz 기준 115200 bps 정확한 설정 (U2X0 = 1, UBRR = 16)
    UCSR0A = (1 << U2X0);       // 2배속 모드 활성화
    UBRR0H = 0x00;
    UBRR0L = 0x10;              // UBRR = 16 (16진수 0x10)

    // 프레임 포맷 설정: 8비트 데이터, 패리티 없음, 스톱 비트 1개
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // 송신, 수신 및 수신 인터럽트 활성화
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    stdout = &OUTPUT;
}

void uartTransmit(char data)
{
    while(!(UCSR0A & (1<<UDRE0)));  // TX 가능?
    UDR0 = data;
}

unsigned char uartReceive()
{
    while(!(UCSR0A & (1<<RXC0)));  // RX 가능?
    return UDR0;
}