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

// RGB565 부분 버퍼 두 개: 480픽셀 * 2바이트 * 2 = 1920바이트.
#define TFT_BUFFER_PIXELS (SCREEN_WIDTH * 2U)

typedef void (*TFT_Painter)(void *context);

// 끝 좌표는 제외한다. painter는 각 띠마다 호출되므로 그리기만 수행해야 한다.
// DrawRect/DrawChar/DrawString은 현재 띠에 자동으로 클리핑된다.
// 동기식 API이며 ISR/다른 painter 안에서 호출하지 않는다.
// 반환 시 모든 전송이 완료된다. CS/DC 직접 제어나 TFT_Data 사용은 금지한다.
void TFT_RenderRegion(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                      uint16_t clear_color, TFT_Painter painter, void *context);


void DrawRect(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t color);
// 8픽셀 모서리의 채워진 사각형. 끝 좌표는 제외한다.
void DrawRoundRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void DrawChar(uint16_t x, uint16_t y, char c, uint8_t scale, uint16_t fg, uint16_t bg);
void DrawString(uint16_t center_x, uint16_t y, const char *str, uint8_t scale, uint16_t fg, uint16_t bg);


void TFT_FillScreen(uint16_t color);
void DrawUI(void);

#endif
