#ifndef MAX30102_H
#define MAX30102_H
#include "i2c.h"
#include <stdint.h>
typedef enum{
  MAX30102_readSuccess,
  MAX30102_readFailed,
} MAX30102_readStatus;
MAX30102_readStatus readPartID(uint8_t *id);
#endif

