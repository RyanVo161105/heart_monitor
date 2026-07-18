#include <stdint.h>
#include "max30102.h"
#include <stddef.h>
#define max_addr 0x57U

#define partID_num 0x15U
#define partID_addr 0xFFU


MAX30102_readStatus readPartID(uint8_t *id){
  I2C_ReadStatus status;
  if(id == NULL){
    return MAX30102_readFailed;
  }
  status = I2C_ReadSlave(max_addr, partID_addr, id, 1U); 
  if(status == I2C_ReadFailed){
    return MAX30102_readFailed;
  }
  if( *id != partID_num){
    return MAX30102_readFailed;
  }
  return MAX30102_readSuccess;
}

