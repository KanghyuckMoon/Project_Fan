#ifndef GUIBOUNDS_H_
#define GUIBOUNDS_H_

#include "def.h"

#define IN_RESULT_ERROR 0
#define IN_RESULT_SUCCESS 1
#define IN_RESULT_PASS 2
#define IN_RESULT_OTHER 3

typedef struct Bounds
{
    uint16_t x, y;
    uint16_t width, height;

} Bound;

bool Bound_ContainsPoint(Bound* self, uint16_t x, uint16_t y);

#endif