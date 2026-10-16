#include "GUIBounds.h"


// Bound* boundsInstance[MAX_BOUNDS] = { 0 };

// uint16_t InputPoint(uint16_t x, uint16_t y)
// {
//     for (uint8_t i = 0; i < MAX_BOUNDS; i++)
//     {
//         Bound* bound = boundsInstance[i];

//         // 등록되지 않은 슬롯
//         if (bound == NULL)
//         {
//             continue;
//         }

//         // Bounds 내부인지 검사
//         if (
//             x >= bound->x &&
//             x <  bound->x + bound->width &&
//             y >= bound->y &&
//             y <  bound->y + bound->height
//         )
//         {
//             return IN_RESULT_SUCCESS;
//         }
//     }

//     return IN_RESULT_PASS;
// }

// uint8_t AddBound(Bound* bound)
// {
//     if (bound == NULL)
//     {
//         return 0;
//     }

//     // 이미 등록되어 있는지 검사
//     for (uint8_t i = 0; i < MAX_BOUNDS; i++)
//     {
//         if (boundsInstance[i] == bound)
//         {
//             return 0;
//         }
//     }

//     // 빈 슬롯 탐색
//     for (uint8_t i = 0; i < MAX_BOUNDS; i++)
//     {
//         if (boundsInstance[i] == NULL)
//         {
//             boundsInstance[i] = bound;
//             return 1;
//         }
//     }

//     // 공간 부족
//     return 0;
// }


// uint8_t RemoveBound(Bound* bound)
// {
//     if (bound == NULL)
//     {
//         return 0;
//     }

//     for (uint8_t i = 0; i < MAX_BOUNDS; i++)
//     {
//         if (boundsInstance[i] == bound)
//         {
//             boundsInstance[i] = NULL;
//             return 1;
//         }
//     }

//     // 찾지 못함
//     return 0;
// }


bool Bound_ContainsPoint(Bound* self, uint16_t x, uint16_t y)
{
    return
        x >= self->x &&
        x < self->x + self->width &&
        y >= self->y &&
        y < self->y + self->height;
}