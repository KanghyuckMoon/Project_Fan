#include "def.h"

#define OV7670_I2C_ADDR   0x42  // SCCB 쓰기 주소 (7비트 0x21 << 1)
#define REG_PID           0x0A  // Product ID Register

extern uint8_t sample_pixel;
extern uint16_t non_zero_count;

void SCCB_Init(void);
uint8_t OV7670_ReadReg(uint8_t regAddr);
uint8_t OV7670_WriteReg(uint8_t regAddr, uint8_t data);

// 카메라 초기화 실행 함수
void OV7670_Init(void);
void Camera_Sync_Init(void);
void OV7670_Capture_Frame(void);
void OV7670_Stream_Frame_UART(void);