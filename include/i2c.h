#ifndef I2C_H
#define I2C_H
typedef enum {
    I2C0_STATUS_OK,
    I2C0_STATUS_ERROR
} I2C0_STATUS;

void I2C_Init(void);
I2C0_STATUS I2C_WriteRegister(uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
I2C0_STATUS I2C_ReadRegister(uint8_t slaveAddr, uint8_t regAddr, uint8_t data);

#endif