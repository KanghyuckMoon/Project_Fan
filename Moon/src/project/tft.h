#ifndef TFT_H_
#define TFT_H_

#include "def.h"
#include "debug.h"

#define TFT_PORT PORTC
#define TFT_DDR  DDRC

#define TFT_CS   PC0
#define TFT_RST  PC1
#define TFT_DC   PC2
#define TFT_MOSI PC3
#define TFT_SCK  PC4

#define TOUCH_CS_PORT PORTC
#define TOUCH_CS_PIN  PC5

#define TOUCH_IRQ_PIN PINC
#define TOUCH_IRQ_BIT PC6

#define TOUCH_DO_PIN  PINC
#define TOUCH_DO_BIT  PC7

#define TOUCH_MOSI_PORT PORTC
#define TOUCH_MOSI_PIN  PC3

#define TOUCH_SCK_PORT PORTC
#define TOUCH_SCK_PIN  PC4


#define CS_LOW()    (TFT_PORT &= ~(1 << TFT_CS))
#define CS_HIGH()   (TFT_PORT |=  (1 << TFT_CS))

#define DC_LOW()    (TFT_PORT &= ~(1 << TFT_DC))
#define DC_HIGH()   (TFT_PORT |=  (1 << TFT_DC))

#define RST_LOW()   (TFT_PORT &= ~(1 << TFT_RST))
#define RST_HIGH()  (TFT_PORT |=  (1 << TFT_RST))

//Touch

#define TOUCH_CS_LOW()   (TOUCH_CS_PORT &= ~(1 << TOUCH_CS_PIN))
#define TOUCH_CS_HIGH()  (TOUCH_CS_PORT |=  (1 << TOUCH_CS_PIN))

#define TOUCH_SCK_LOW()  (TOUCH_SCK_PORT &= ~(1 << TOUCH_SCK_PIN))
#define TOUCH_SCK_HIGH() (TOUCH_SCK_PORT |=  (1 << TOUCH_SCK_PIN))

#define TOUCH_MOSI_LOW()  (TOUCH_MOSI_PORT &= ~(1 << TOUCH_MOSI_PIN))
#define TOUCH_MOSI_HIGH() (TOUCH_MOSI_PORT |=  (1 << TOUCH_MOSI_PIN))

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

//Touch
uint8_t Touch_SPI_Transfer(uint8_t data);
uint16_t Touch_ReadRaw(uint8_t command);
uint16_t Touch_ReadX(void);
uint16_t Touch_ReadY(void);
void TouchTest(void);

#endif