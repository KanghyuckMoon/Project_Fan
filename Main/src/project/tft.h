#ifndef TFT_H_
#define TFT_H_

#include "def.h"

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


// ========================
// Colors RGB565
// ========================

#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define WHITE   0xFFFF
#define BLACK   0x0000
#define YELLOW  0xFFE0


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

void TFT_FillScreen(uint16_t color);
void DrawUI(void);

#endif