#include "i2c.h"

void I2C_Init()
{
    I2C_DDR |= (1<<I2C_SCL) | (1<<I2C_SDA); // 출력 설정
    TWBR = 72;
    //TWBR = 32; // 200KHz
    //TWBR = 12; // 400KHz


}

void I2C_Start()
{
    // TWINT: 인터럽트 플래그 클리어
    // TWSTA : 스타트 조건 전송
    // TWEN : I2C 통시 기능 활성화
    TWCR |= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));

}

void I2C_Stop()
{
    TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

void I2C_TxData(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while(!(TWCR & (1<<TWINT)));
}

void I2C_TxByte(uint8_t devAddRw, uint8_t data)
{
    I2C_Start();
    I2C_TxData(devAddRw);
    I2C_TxData(data);
    I2C_Stop();
}
