#include "debug.h"
#include <stdarg.h>
#include <stdio.h>

void DebugLog(const char* format, ...)
{
    char buffer[128];

    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    uartTransmitString(buffer);
}