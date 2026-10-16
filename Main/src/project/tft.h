#ifndef TFT_H_
#define TFT_H_

#include "def.h"
#include "FontAndColor.h"

// ========================
// TFT SPI
// ========================

#define TFT_PORT PORTB
#define TFT_DDR  DDRB

#define TFT_CS   PB0
#define TFT_SCK  PB1
#define TFT_MOSI PB2

// ========================
// TFT Control
// ========================

#define TFT_RST_PORT PORTA
#define TFT_RST_DDR  DDRA
#define TFT_RST      PA0

#define TFT_DC_PORT  PORTA
#define TFT_DC_DDR   DDRA
#define TFT_DC       PA1


// ========================
// Control Macros
// ========================

#define CS_LOW()     (TFT_PORT &= ~(1 << TFT_CS))
#define CS_HIGH()    (TFT_PORT |=  (1 << TFT_CS))

#define DC_LOW()     (TFT_DC_PORT &= ~(1 << TFT_DC))
#define DC_HIGH()    (TFT_DC_PORT |=  (1 << TFT_DC))

#define RST_LOW()    (TFT_RST_PORT &= ~(1 << TFT_RST))
#define RST_HIGH()   (TFT_RST_PORT |=  (1 << TFT_RST))



extern volatile uint8_t motor_on;
extern volatile uint8_t motor_level;
extern volatile uint8_t rotation_on;



void SPI_Write(uint8_t data);

void TFT_Command(uint8_t cmd);
void TFT_Data(uint8_t data);

void TFT_Init(void);

void TFT_SetWindow(
    uint16_t x0,
    uint16_t y0,
    uint16_t x1,
    uint16_t y1
);


void DrawRect(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t color);
void DrawChar(uint16_t x, uint16_t y, char c, uint8_t scale, uint16_t fg, uint16_t bg);
void DrawString(uint16_t center_x, uint16_t y, const char *str, uint8_t scale, uint16_t fg, uint16_t bg);


void TFT_FillScreen(uint16_t color);
void DrawUI(void);

#endif