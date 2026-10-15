#include "def.h"
#include "fnd.h"

uint8_t fnd_1_Number[] = {
    0b10111011, //0
    0b00001010, //1
    0b01110011, //2
    0b01011011, //3
    0b11001010, //4
    0b11011001, //5
    0b11111000, //6
    0b10001011, //7
    0b11111011, //8
    0b11001011 //9
};

void fnd_1_Init(){
    FND_DDR = 0xFF;
}

void fnd_1_Display(uint8_t data){
    FND_PORT = fnd_1_Number[data];
}

void fnd_1_Dp_Display(uint8_t data){
    FND_PORT = fnd_1_Number[data] + 0b00000100;
}