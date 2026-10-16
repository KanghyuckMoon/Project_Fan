#ifndef FONT_AND_COLOR_H_
#define FONT_AND_COLOR_H_

#include "def.h"

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define WHITE   0xFFFF
#define BLACK   0x0000
#define YELLOW  0xFFE0

// 글꼴: A, E, F, L, N, O, R, T, U, V, 1, 2, 3, /, 공백
static const uint8_t font[15][7] = {
    {14,17,17,31,17,17,17}, // A
    {31,16,16,30,16,16,31}, // E
    {31,16,16,30,16,16,16}, // F
    {16,16,16,16,16,16,31}, // L
    {17,25,25,21,19,19,17}, // N
    {14,17,17,17,17,17,14}, // O
    {30,17,17,30,20,18,17}, // R
    {31,4,4,4,4,4,4},       // T
    {17,17,17,17,17,17,14}, // U
    {17,17,17,17,17,10,4},  // V
    {4,12,4,4,4,4,14},      // 1
    {14,17,1,2,4,8,31},     // 2
    {30,1,1,14,1,1,30},     // 3
    {1,2,2,4,8,8,16},       // /
    {0,0,0,0,0,0,0}         // 공백
};

static uint8_t FontIndex(char c)
{
    switch (c)
    {
        case 'A': return 0;
        case 'E': return 1;
        case 'F': return 2;
        case 'L': return 3;
        case 'N': return 4;
        case 'O': return 5;
        case 'R': return 6;
        case 'T': return 7;
        case 'U': return 8;
        case 'V': return 9;
        case '1': return 10;
        case '2': return 11;
        case '3': return 12;
        case '/': return 13;
        case ' ': return 14;
        default:  return 14;
    }
}

#endif