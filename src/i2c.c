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


#define RCGC_I2C_R (*((volatile uint32_t *)(SYSCTL_BASE_ADDR + RCGC_I2C_OFFSET)))
void I2C_Init(void) {
    RCGC_I2C_R |= (1U << 0); // Enable clock for I2C0
    RCGC_GPIO_R |= (1U << 1); // Enable clock for GPIO Port B
    GPIO_PORTB_DEN_R |= I2C0_PINS; // Enable digital function for PB2 and PB3
    GPIO_PORTB_AFSEL_R |= I2C0_PINS; // Enable alternate function for PB2 and PB3
    GPIO_PORTB_ODR_R |= I2C0_PINS; // Enable open-drain for PB2 and PB3
    GPIO_PORTB_PCTL_R |= I2C0_PCTL_PINS; // Configure PCTL for PB2 and PB3
    I2C0_MCR_R |= 0x00000010U; // Enable I2C Master function, the value 0x0000.0010U is specified in the datasheet for this microcontroller
    /*
     * Set the I2C clock speed to 100 kbps
     * TPR = (System Clock / (2*(SCL_LP + SCL_HP)*I2C_Clk)) - 1 
     * SCL_LP = 6, SCL_HP = 4, I2C_Clk = 100000
     * System Clock = 16 MHz
     * TPR = (16,000,000 / (2*(6 + 4)*100,000)) - 1 = 7
     */
    I2C0_MTPR_R = 0x00000007U; // Set the I2C clock speed to 100 kbps
}
I2C0_STATUS I2C_WriteRegister(uint8_t slaveAddr, uint8_t regAddr, uint8_t data){
    /*
     * The first bit should be 0 
     * 0 = Transmit
     * 1 = Receive
     */
    I2C0_MSA_R = (slaveAddr << 1); //First bit is 0
    I2C0_MDR_R = regAddr;
    I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN); //Giving the slave which of its addresses need to be written to
    while(I2C0_MCS_R & I2C0_MCS_BUSY); //Polling to check if the transmission is done yet
    if(I2C0_MCS_R & I2C0_MCS_ERROR){
        I2C0_MCS_R = I2C0_MCS_STOP;
        return I2C0_STATUS_ERROR;
    }
    I2C0_MDR_R = data;
    I2C0_MCS_R = (I2C0_MCS_RUN | I2C0_MCS_STOP); //Giving the slave data which being written into regAddr
    while(I2C0_MCS_R & I2C0_MCS_BUSY); //Polling to check if the transmission is done yet
    if(I2C0_MCS_R & I2C0_MCS_ERROR){
        I2C0_MCS_R = I2C0_MCS_STOP;
        return I2C0_STATUS_ERROR;
    }
    return I2C0_STATUS_OK;
}
I2C0_STATUS I2C_ReadRegister(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data){
    /*
     * The first bit should be 0 
     * 0 = Transmit
     * 1 = Receive
     */
    I2C0_MSA_R = (slaveAddr << 1); //First bit is 0
    I2C0_MDR_R = regAddr;
    I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN); //Giving the slave which of its addresses need to be written to
    while(I2C0_MCS_R & I2C0_MCS_BUSY); //Polling to check if the transaction is done yet
    if(I2C0_MCS_R & I2C0_MCS_ERROR){
        I2C0_MCS_R = I2C0_MCS_STOP;
        return I2C0_STATUS_ERROR;
    }
    I2C0_MSA_R = (slaveAddr << 1 | (1U << 0)); //Change the next operation to Receive
    I2C0_MCS_R = (I2C0_MCS_START | I2C0_MCS_RUN | I2C0_MCS_STOP);
    while(I2C0_MCS_R & I2C0_MCS_BUSY); //Polling to check if the transaction is done yet
    if(I2C0_MCS_R & I2C0_MCS_ERROR){
        I2C0_MCS_R = I2C0_MCS_STOP;
        return I2C0_STATUS_ERROR;
    }
    *data = (uint8_t)(I2C0_MDR_R & 0xFFU); //Only 8 bits are needed here to be copied into data
    return I2C0_STATUS_OK;
}