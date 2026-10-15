#include "def.h"

// FND 숫자 데이터 포트 설정
#define FND_DATA_DDR    DDRB
#define FND_DATA_PORT   PORTB

// FND 자릿수 포트 설정
#define FND_SELECT_DDR  DDRC
#define FND_SELECT_PORT PORTC
#define FND_SELECT_MASK 0x0F

// 함수의 원형 선언
void FND_4_Init(void);
void FND_4_AllDigitOff(void);
void FND_4_Scan(uint16_t number);