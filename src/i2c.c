#include <stdint.h>
#include "i2c.h"

#define SYSCTL_BASE_ADDR 0x400FE000U

#define RCGC_I2C_OFFSET 0x620U
#define RCGC_GPIO_OFFSET 0x608U

#define RCGC_I2C_R (*((volatile uint32_t *)(SYSCTL_BASE_ADDR + RCGC_I2C_OFFSET)))
#define RCGC_GPIO_R (*((volatile uint32_t *)(SYSCTL_BASE_ADDR + RCGC_GPIO_OFFSET)))

#define GPIO_PORTB_BASE_ADDR 0x40005000U

#define GPIO_PORTB_AFSEL_OFFSET 0x420U
#define GPIO_PORTB_DEN_OFFSET 0x51CU
#define GPIO_PORTB_ODR_OFFSET 0x50CU
#define GPIO_PORTB_PCTL_OFFSET 0x52CU

#define GPIO_PORTB_AFSEL_R (*((volatile uint32_t *)(GPIO_PORTB_BASE_ADDR + GPIO_PORTB_AFSEL_OFFSET)))
#define GPIO_PORTB_DEN_R (*((volatile uint32_t *)(GPIO_PORTB_BASE_ADDR + GPIO_PORTB_DEN_OFFSET)))
#define GPIO_PORTB_ODR_R (*((volatile uint32_t *)(GPIO_PORTB_BASE_ADDR + GPIO_PORTB_ODR_OFFSET)))
#define GPIO_PORTB_PCTL_R (*((volatile uint32_t *)(GPIO_PORTB_BASE_ADDR + GPIO_PORTB_PCTL_OFFSET)))

#define I2C0_BASE_ADDR 0x40020000U

#define I2C0_MCR_OFFSET 0x020U
#define I2C0_MTPR_OFFSET 0x00CU
#define I2C0_MSA_OFFSET 0x000U
#define I2C0_MDR_OFFSET 0x008U
#define I2C0_MCS_OFFSET 0x004U

#define I2C0_MCR_R (*((volatile uint32_t *)(I2C0_BASE_ADDR + I2C0_MCR_OFFSET)))
#define I2C0_MTPR_R (*((volatile uint32_t *)(I2C0_BASE_ADDR + I2C0_MTPR_OFFSET)))
#define I2C0_MSA_R (*((volatile uint32_t *)(I2C0_BASE_ADDR + I2C0_MSA_OFFSET)))
#define I2C0_MDR_R (*((volatile uint32_t *)(I2C0_BASE_ADDR + I2C0_MDR_OFFSET)))
#define I2C0_MCS_R (*((volatile uint32_t *)(I2C0_BASE_ADDR + I2C0_MCS_OFFSET)))

#define I2C0SCL (1U << 2) // PB2
#define I2C0SDA (1U << 3) // PB3
#define I2C0_PINS (I2C0SCL | I2C0SDA)

#define I2C0_PCTL_SCL (0x3U << (8)) // PCTL for PB2 (I2C0SCL)
#define I2C0_PCTL_SDA (0x3U << (12)) // PCTL for PB3 (I2C0SDA)
#define I2C0_PCTL_PINS (I2C0_PCTL_SCL | I2C0_PCTL_SDA)

#define I2C0_MCS_ERROR (1U << 1) // ERROR BIT / Read 
#define I2C0_MCS_BUSBSY (1U << 6) // BUSBSY BIT / Read
#define I2C0_MCS_BUSY (1U << 0) // BUSY BIT / Read
#define I2C0_MCS_STOP (1U << 2) // STOP BIT / Write
#define I2C0_MCS_START (1U << 1) // START BIT / Write
#define I2C0_MCS_RUN (1U << 0) // RUN BIT / Write
#define I2C0_MCS_ACK (1U << 3) // ACK BIT / Write

#define RCGC_I2C_R (*((volatile uint32_t *)(SYSCTL_BASE_ADDR + RCGC_I2C_OFFSET)))
void I2C_Init(void) {
    RCGC_I2C_R |= (1U << 0); 
    RCGC_GPIO_R |= (1U << 1);
    GPIO_PORTB_DEN_R |= I2C0_PINS;
    GPIO_PORTB_AFSEL_R |= I2C0_PINS; 
    GPIO_PORTB_ODR_R |= I2C0_PINS; 
    GPIO_PORTB_PCTL_R |= I2C0_PCTL_PINS; 
    I2C0_MCR_R |= 0x010U; 
    /*
     * Set the I2C clock speed to 100 kbps
     * TPR = (System Clock / (2*(SCL_LP + SCL_HP)*I2C_Clk)) - 1 
     * SCL_LP = 6, SCL_HP = 4, I2C_Clk = 100000
     * System Clock = 16 MHz
     * TPR = (16,000,000 / (2*(6 + 4)*100,000)) - 1 = 7
     */
    I2C0_MTPR_R = 0x07U;
}
I2C_WriteStatus I2C_WriteSlave(uint8_t slaveAddr, uint8_t *data_ptr, uint8_t length){
    I2C0_MSA_R = (slaveAddr << 1U);
    if(length == 1){
    I2C0_MDR_R = data_ptr[0];
    I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN | I2C0_MCS_STOP);
    while(I2C0_MCS_R & I2C0_MCS_BUSY){};
    if((I2C0_MCS_R & I2C0_MCS_ERROR) == 0){
       return I2C_WriteSuccess; 
    }else{
       return I2C_WriteFailed;
    }
    }
    else if(length >= 2){
    I2C0_MDR_R = data_ptr[0];
    I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN);
    while(I2C0_MCS_R & I2C0_MCS_BUSY){};
    if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
       I2C0_MCS_R = I2C0_MCS_STOP;
       return I2C_WriteFailed;
    }
    for(uint8_t i =1; i < length -1; i++){
      I2C0_MDR_R = data_ptr[i];
      I2C0_MCS_R = I2C_MCS_RUN;
      while(I2C0_MCS_R & I2C0_MCS_BUSY){};
      if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
        I2C0_MCS_R = I2C0_MCS_STOP;
        return I2C_WriteFailed;
      }
      }
    I2C0_MDR_R = data_ptr[length -1];
    I2C0_MCS_R = (I2C0_MCS_RUN|I2C0_MCS_STOP);
    while(I2C0_MCS_R & I2C0_MCS_BUSY){};
    if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
       return I2C_WriteFailed;
    }
    return I2C_WriteSuccess;
    }else{
      return I2C_WriteFailed;
    }
}
I2C_ReadStatus I2C_ReadSlave(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t length){
  I2C0_MSA_R = (slaveAddr << 1U);
  I2C0_MDR_R = regAddr;
  I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN);
  while(I2C0_MCS_R & I2C0_MCS_BUSY){};
  if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
    return I2C_ReadFailed;  
  }
  I2C0_MSA_R = (slaveAddr << 1U) | 1U;
  if(length == 1){
    I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN | I2C0_MCS_STOP);
    while(I2C0_MCS_R & I2C0_MCS_BUSY){};
    if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
      return I2C_ReadFailed;
    }
    data[0] = I2C0_MDR_R;
    return I2C_ReadSuccess;
  }else if(length >= 1){
    I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN | I2C0_MCS_ACK);
    while(I2C0_MCS_R & I2C0_MCS_BUSY){};
    if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
      return I2C_ReadFailed;
    }
    data[0] = I2C0_MDR_R;
    for(int i = 1; i < length -1; i++){
      I2C0_MCS_R = (I2C0_MCS_RUN | I2C0_MCS_ACK);
      while(I2C0_MCS_R & I2C0_MCS_BUSY){};
      if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
        return I2C_ReadFailed;
      }
      data[i] = I2C0_MDR_R;
    }
    I2C0_MCS_R = (I2C0_MCS_RUN | I2C0_MCS_STOP);
      while(I2C0_MCS_R & I2C0_MCS_BUSY){};
      if((I2C0_MCS_R & I2C0_MCS_ERROR) != 0){
        return I2C_ReadFailed;
      }
    data[length-1]=I2C0_MDR_R;
  }else{
    return I2C_ReadFailed;
  }
  return I2C_ReadSuccess;
}
