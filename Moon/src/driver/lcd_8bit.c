// #include "lcd_8bit.h"

// #define LCD_DATA_DDR DDRF
// #define LCD_DATA_PORT PORTF

// #define LCD_CTRL_DDR DDRE
// #define LCD_CTRL_PORT PORTE

// #define LCD_RS_PIN PE5
// #define LCD_RW_PIN PE6
// #define LCD_E_PIN PE7

// #define LCD_NORMAL_DELAY_US 50U
// #define LCD_CLEAR_DELAY_MS 2U

// //Enable 펄스 발생
// static void LCD_PulseEnble(){
//     LCD_CTRL_PORT &= (uint8_t)~(1 << LCD_E_PIN);
//     _delay_us(1);

//     LCD_CTRL_PORT |= (1 << LCD_E_PIN);
//     _delay_us(1);

//     LCD_CTRL_PORT &= (uint8_t)~(1 << LCD_E_PIN);
//     _delay_us(1);
// }

// static void LCD_WriteRawCommand(uint8_t command){
//     //RS -> 0 으로 만들어야 명령어 레지스터 선탱
//     LCD_CTRL_PORT &= (uint8_t)~(1 << LCD_RS_PIN);

//     //RW -> 0 Write 모드
//     LCD_CTRL_PORT &= (uint8_t)~(1 << LCD_RW_PIN);

//     LCD_DATA_PORT = command;
//     _delay_us(1);

//     LCD_PulseEnble();
// }

// void LCD_Init(){
//     LCD_DATA_DDR = 0xFF;
//     LCD_DATA_PORT = 0x00;

//     LCD_CTRL_DDR = (1 << LCD_RS_PIN) | (1 << LCD_RW_PIN) | (1 << LCD_E_PIN);
//     LCD_CTRL_PORT &= (uint8_t)~((1 << LCD_RS_PIN) | (1 << LCD_RW_PIN) | (1 << LCD_E_PIN));

//     _delay_ms(20);

//     LCD_WriteCommand(0x30);
//     _delay_ms(5);
//     LCD_WriteCommand(0x30);
//     _delay_us(200);
//     LCD_WriteCommand(0x30);
//     _delay_us(50);
//     LCD_WriteCommand(LCD_CMD_FUNCTION_8BIT);
//     LCD_WriteCommand(LCD_CMD_DISPLAY_OFF);
//     LCD_Clear();
//     LCD_WriteCommand(LCD_CMD_ENTRY_INCREMENT);
//     LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
// }

// void LCD_WriteCommand(uint8_t command){
//     LCD_WriteRawCommand(command);
//     if((command == LCD_CMD_CLEAR) || (command == LCD_CMD_HOME)){
//         _delay_ms(LCD_CLEAR_DELAY_MS);
//     }
//     else{
//         _delay_us(LCD_NORMAL_DELAY_US);
//     }
// }

// void LCD_WriteData(uint8_t data){
//     //RS = 1
//     LCD_CTRL_PORT |= (1 << LCD_RS_PIN);
    
//     LCD_CTRL_PORT &= (uint8_t)~(1 << LCD_RW_PIN);

//     LCD_DATA_PORT = data;
//     _delay_us(1);

//     LCD_PulseEnble();

//     _delay_us(LCD_NORMAL_DELAY_US);
// }

// void LCD_Clear(){
//     LCD_WriteCommand(LCD_CMD_CLEAR);
// }

// void LCD_GotoXY(uint8_t row, uint8_t column){
//     uint8_t ddramAddress;
//     if(row > 1U){
//         row = 1U;
//     }
//     if(column > 15U){
//         column = 15U;
//     }

//     if(row == 0U){
//         ddramAddress = column;
//     }
//     else{
//         ddramAddress = (uint8_t)(0x40 + column);
//     }

//     LCD_WriteCommand((uint8_t)(LCD_CMD_SET_DDRAM | ddramAddress));
// }

// void LCD_WriteString(const char *string){
//     if(string == 0){
//         return;
//     }
//     while(*string != '\0'){
//         LCD_WriteData((uint8_t)*string);
//         string++;
//     }
// }

// void LCD_WriteStringXY(uint8_t row, uint8_t column, const char *string){
//     LCD_GotoXY(row, column);
//     LCD_WriteString(string);
// }