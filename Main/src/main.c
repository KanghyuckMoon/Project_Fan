#include "ap.h"


int main(void)
{
    //Init
    int result = apInit(); 
    if(result == -1)
    {
        DebugLog("Error");
    }

    //Main
    apMain();
}