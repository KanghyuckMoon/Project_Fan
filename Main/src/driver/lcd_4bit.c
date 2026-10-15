// #include "lcd_4bit.h"


// // LCD 데이터 포트 -> 4bit 만 사용하므로 0~3번까지는 변경 안함
// #define LCD_DATA_DDR    DDRF
// #define LCD_DATA_PORT   PORTF

// #define LCD_DATA_MASK   0xF0

// // LCD 제어 포트 및 제어 핀번호
// #define LCD_CTRL_DDR    DDRE
// #define LCD_CTRL_PORT   PORTE

// #define LCD_RS_PIN      PE5
// #define LCD_RW_PIN      PE6
// #define LCD_E_PIN       PE7

// #define LCD_NORMAL_DELAY_US 50U
// #define LCD_CLEAR_DELAY_MS  2U


// static void LCD_SetNibble(uint8_t nibble)
// {
//     LCD_DATA_PORT = (uint8_t)((LCD_DATA_PORT & 0x0F) | ((nibble & 0x0F) << 4));
// }

// // Enable 펄스 발생
// static void LCD_PulseEnable()
// {
//     LCD_CTRL_PORT &= (uint8_t)~(1<<LCD_E_PIN);
//     _delay_us(1);

//     LCD_CTRL_PORT |= (1<<LCD_E_PIN);
//     _delay_us(1);
    
//     LCD_CTRL_PORT &= (uint8_t)~(1<<LCD_E_PIN);
//     _delay_us(1);
// }

// static void LCD_WriteIniNibble(uint8_t nibble)
// {
//     LCD_CTRL_PORT &= (uint8_t)~(1<<LCD_RS_PIN);     // RS => 명령어 레지스터 선택

//     LCD_CTRL_PORT &= (uint8_t)~(1<<LCD_RW_PIN);     // RW => Write Mode

//     LCD_SetNibble(nibble);
//     LCD_PulseEnable();
// }

// static void LCD_WriteByte(uint8_t value)
// {
//     // 데이터 7번비트부터 데이터 4번비트 까지 전송
//     LCD_SetNibble((uint8_t)(value >> 4));
//     LCD_PulseEnable();
//     // data3 ~ data0
//     LCD_SetNibble((uint8_t)(value & 0x0F));
//     LCD_PulseEnable();
// }

// void LCD_Init()
// {
//     LCD_DATA_DDR |= LCD_DATA_MASK;
//     LCD_DATA_PORT &= (uint8_t)~LCD_DATA_MASK;

//     LCD_CTRL_DDR |= (1<<LCD_RS_PIN) | (1<<LCD_RW_PIN) | (1<<LCD_E_PIN);
//     // 초기 제어 신호를 전부 0으로
//     LCD_CTRL_PORT &= (uint8_t)~((1<<LCD_RS_PIN) | (1<<LCD_RW_PIN) | (1<<LCD_E_PIN));

//     _delay_ms(20);

//     LCD_WriteIniNibble(0x03);
//     _delay_ms(5);
//     LCD_WriteIniNibble(0x03);
//     _delay_us(200);
//     LCD_WriteIniNibble(0x03);
//     _delay_us(50);
//     LCD_WriteIniNibble(0x02);
//     _delay_us(50);

//     LCD_WriteCommand(LCD_CMD_FUNCTION_4BIT);
//     LCD_WriteCommand(LCD_CMD_DISPLAY_OFF);
//     LCD_Clear();
//     LCD_WriteCommand(LCD_CMD_ENTRY_INCREMENT);
//     LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
// }

// void LCD_WriteCommand(uint8_t command)
// {
//     LCD_CTRL_PORT &= (uint8_t)~(1<<LCD_RS_PIN);
//     LCD_CTRL_PORT &= (uint8_t)~(1<<LCD_RW_PIN);
//     LCD_WriteByte(command);
//     if((command == LCD_CMD_CLEAR) || (command == LCD_CMD_HOME))
//     {
//         _delay_ms(LCD_CLEAR_DELAY_MS);
//     }
//     else
//     {
//         _delay_us(LCD_NORMAL_DELAY_US);
//     }
// }

// void LCD_WriteData(uint8_t data)
// {
//     LCD_CTRL_PORT |= (uint8_t)(1<<LCD_RS_PIN);
//     LCD_CTRL_PORT &= (uint8_t)~(1<<LCD_RW_PIN);

//     LCD_WriteByte(data);
//     _delay_us(LCD_NORMAL_DELAY_US);
// }

// void LCD_Clear()
// {
//     LCD_WriteCommand(LCD_CMD_CLEAR);
// }

// void LCD_GotoXY(uint8_t row, uint8_t column)
// {
//     uint8_t ddramAddress;
//     if(row > 1U)            // 1602A 타입의 LCD는 최대 2행 이므로
//     {
//         row = 1U;
//     }
//     if(column > 15U)        // 최대 16열 이므로
//     {
//         column = 15U;
//     }
    
//     if(row == 0U)
//     {
//         ddramAddress = column;
//     }
//     else
//     {
//         ddramAddress = (uint8_t)(0x40 + column);
//     }

//     LCD_WriteCommand((uint8_t)(LCD_CMD_SET_DDRAM | ddramAddress));
// }

// void LCD_WriteString(const char *string)
// {
//     if(string == 0)     // NULL pointer 가 들어오면 그냥 놀음
//     {
//         return;
//     }
//     while(*string != '\0')
//     {
//         LCD_WriteData((uint8_t)*string);
//         string++;
//     }
// }

// void LCD_WriteStringXY(uint8_t row, uint8_t column, const char *string)
// {
//     LCD_GotoXY(row, column);
//     LCD_WriteString(string);
// }
// uint8_t LCD_TestBusyFlag(void)
// {
//     uint8_t high;

//     // D4~D7 입력으로 전환
//     LCD_DATA_DDR &= ~0xF0;

//     // RS = 0 : 명령 레지스터
//     LCD_CTRL_PORT &= ~(1 << LCD_RS_PIN);

//     // RW = 1 : Read
//     LCD_CTRL_PORT |= (1 << LCD_RW_PIN);

//     // E = 1
//     LCD_CTRL_PORT |= (1 << LCD_E_PIN);
//     _delay_us(1);

//     high = PINF & 0xF0;

//     // E = 0
//     LCD_CTRL_PORT &= ~(1 << LCD_E_PIN);

//     // 4bit 모드이므로 하위 nibble도 한 번 읽어서 버림
//     LCD_CTRL_PORT |= (1 << LCD_E_PIN);
//     _delay_us(1);
//     LCD_CTRL_PORT &= ~(1 << LCD_E_PIN);

//     // 다시 출력으로
//     LCD_DATA_DDR |= 0xF0;

//     // RW = 0
//     LCD_CTRL_PORT &= ~(1 << LCD_RW_PIN);

//     return (high & (1 << PF7)) ? 1 : 0;
// }