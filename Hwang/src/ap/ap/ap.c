#include "ap.h"

void apInit()
{
    LCD_Init();
}


void apMain()
{
    char buffer[17];
    uint16_t count = 0;

    LCD_WriteStringXY(0, 0, "HELLO I2C_Comm");

    while(1)
    {
        sprintf(buffer, "Count : %-5u", (unsigned int)count);

        LCD_WriteStringXY(1, 0, buffer);
        count++;

        _delay_ms(200);
    }
              
}
 
// char rxBuff[100] = {0,};
// volatile uint8_t rxFlag = 0;

// LED led = {&PORTD, 0};   // 예시: PORTB의 0번 핀에 연결된 LED (실제 배선에 맞게 수정)

// ISR(USART0_RX_vect)
// {
//     static uint8_t rxHead = 0;
//     uint8_t rxData = UDR0;

//     if(rxData == '\n' || rxData == '\r')
//     {
//         rxBuff[rxHead] = '\0';
//         rxHead = 0;
//         rxFlag = 1;
//     }
//     else
//     {
//         if(rxHead < sizeof(rxBuff) - 1)
//         {
//             rxBuff[rxHead++] = rxData;
//         }
//     }
// }

// void apInit()
// {
//     uartInit();
//     ledInit(&led);       // LED 핀을 출력으로 설정
// }

// void apMain()
// {
//     sei();               // 인터럽트 전역 활성화

//     while (1)
//     {
//         if(rxFlag == 1)
//         {
//             rxFlag = 0;

//             if(rxBuff[0] == 'a')
//             {
//                 ledOn(&led);
//                 printf("LED ON\r\n");
//             }
//             else if(rxBuff[0] == 'b')
//             {
//                 ledOff(&led);
//                 printf("LED OFF\r\n");
//             }
//             else
//             {
//                 printf("%s\r\n", rxBuff);   // a, b가 아니면 받은 내용을 그대로 출력
//             }
//         }
//     }
// }

/*
교수님이 적으신 코드
#include "ap.h"


char rxBuff[100] = {0,};
volatile uint8_t rxFlag = 0;


ISR(USART0_RX_vect)
{
    static uint8_t rxHead = 0;
    uint8_t rxData = UDR0;

    if(rxData == '\n' || rxData == '\r')
    {
        rxBuff[rxHead] = '\0';
        rxHead = 0;
        rxFlag = 1;
    }
    else
    {
        if(rxHead < sizeof(rxBuff) - 1)
        {
            rxBuff[rxHead++] = rxData;
        }
    }
}



void apInit()
{
    uartInit();

    DDRD = 0xff;
    PORTD = 0x00;

}

void apMain()
{
    sei();          // 인터럽트 전역 활성화

    uint8_t ledOn[] = "LED ON";
    uint8_t ledOff[] = "LED OFF";

    while (1)
    {
        if(rxFlag == 1)
        {
            rxFlag = 0;
            printf("%s\r\n",rxBuff);

            if(rxBuff[0] == 'a')
            {
                printf("%s\r\n",ledOn);
                for(uint8_t i = 0; i < 8; i++)
                {
                    PORTD |= (1<<i);
                }
            }
            if(rxBuff[0] == 'b')
            {
                printf("%s\r\n",ledOff);
                for(uint8_t i = 0; i < 8; i++)
                {
                    PORTD &= ~(1<<i);
                }
            }
        }
    }  
}
    */