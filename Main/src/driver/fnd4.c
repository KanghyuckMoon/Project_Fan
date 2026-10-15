#include "fnd4.h"
#include <util/delay.h>

static const uint8_t fnd_4_Number[] = 
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x27, 0x7F, 0x67
};

static const uint16_t decimal_number[] = 
{
    1000, 100, 10, 1
};

void fnd_4_Init(void)
{
    FND_DATA_DDR = 0xFF;
    FND_SELECT_DDR |= FND_SELECT_MASK; //PC0~3번 핀을 출력으로 설정

    FND_DATA_PORT = 0x00; //Low로 시작 (FND를 Off로 시작)

    fnd_4_AllDigitOff();
}

void fnd_4_AllDigitOff(void)
{
    // 자리 선택 HIGH = ON 기준
    FND_SELECT_PORT &= ~FND_SELECT_MASK;
}
void fnd_4_Scan(uint16_t number)
{
    static uint8_t position = 0;

    uint8_t digit;
    uint8_t select_data;

    number %= 10000;        // 표시 범위를 9999로 제한

    // 현재 위치를 숫자로 계산
    digit = (number / decimal_number[position]) % 10;

    // 옵션 잔상을 위한 전체 자리  OFF
    fnd_4_AllDigitOff();

    // 데이터 (숫자)출력
    FND_DATA_PORT = fnd_4_Number[digit];

    // 현재 자리 선택 (Low 일때만 켜짐 -> Active Low)
    select_data = FND_SELECT_MASK & ~(1 << position);
    FND_SELECT_PORT = (FND_SELECT_PORT & ~FND_SELECT_MASK | select_data);

    // 다음 자리 이동
    position++;
    if(position >= 4)
    {
        position = 0;
    }
}