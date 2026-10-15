#ifndef TFT_H_
#define TFT_H_

#include "def.h"

#define TFT_PORT PORTC
#define TFT_DDR  DDRC

#define TFT_CS   PC0
#define TFT_RST  PC1
#define TFT_DC   PC2
#define TFT_MOSI PC3
#define TFT_SCK  PC4

#define CS_LOW()    (TFT_PORT &= ~(1 << TFT_CS))
#define CS_HIGH()   (TFT_PORT |=  (1 << TFT_CS))

#define DC_LOW()    (TFT_PORT &= ~(1 << TFT_DC))
#define DC_HIGH()   (TFT_PORT |=  (1 << TFT_DC))

#define RST_LOW()   (TFT_PORT &= ~(1 << TFT_RST))
#define RST_HIGH()  (TFT_PORT |=  (1 << TFT_RST))

#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define WHITE   0xFFFF
#define BLACK   0x0000
#define YELLOW  0xFFE0

void SPI_Write(uint8_t data);


void TFT_Command(uint8_t cmd);
void TFT_Data(uint8_t data);
void TFT_Init(void);
void TFT_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void TFT_FillScreen(uint16_t color);

#endif