#include "ap.h"

int main(void)
{
    //Init
    fanInit();
    ButtonSystemInit();
    ServoInit();
    uartInit();


    //Main
    SetRotateMode();
    while(1)
    {
        ServoMain();
        ButtonMain();
        TickMain();
    }
}