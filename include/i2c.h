#ifndef I2C_H
#define I2C_H

void I2C_Init(void);
I2C_WriteStatus I2C_WriteSlave(uint8_t slaveAddr, uint8_t *data_ptr, uint8_t length);
I2C_ReadStatus I2C_ReadSlave(uint8_t slaveAddr, uint8_t *data, uint8_t length);
#endif
