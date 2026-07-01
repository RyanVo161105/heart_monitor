#include <stdint.h>
#include "uart.h"

/*
  * RCGCUART enables the clock for the UART modules
  *
  * RCGCGPIO enables the clock for the GPIO modules that UART pins are connected to
  * 
  */
#define SYSCTL_BASE_ADDR 0x400FE000U

#define RCGCUART_OFFSET 0x618U
#define RCGCGPIO_OFFSET 0x608U

#define RCGC_UART_R (*((volatile uint32_t *)(SYSCTL_BASE_ADDR + RCGCUART_OFFSET)))
#define RCGC_GPIO_R (*((volatile uint32_t *)(SYSCTL_BASE_ADDR + RCGCGPIO_OFFSET)))

#define GPIO_PORTA_BASE_ADDR 0x40004000U

#define GPIO_PORTA_AFSEL_OFFSET 0x420U
#define GPIO_PORTA_DEN_OFFSET 0x51CU
#define GPIO_PORTA_PCTL_OFFSET 0x52CU

#define GPIO_PORTA_AFSEL_R (*((volatile uint32_t *)(GPIO_PORTA_BASE_ADDR + GPIO_PORTA_AFSEL_OFFSET)))
#define GPIO_PORTA_DEN_R (*((volatile uint32_t *)(GPIO_PORTA_BASE_ADDR + GPIO_PORTA_DEN_OFFSET)))
#define GPIO_PORTA_PCTL_R (*((volatile uint32_t *)(GPIO_PORTA_BASE_ADDR + GPIO_PORTA_PCTL_OFFSET)))

#define UART0_BASE_ADDR 0x4000C000U

#define UART0_FR_OFFSET 0x018U
#define UART0_DR_OFFSET 0x000U
#define TXFF (1U << 5)

#define UART0_DR_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_DR_OFFSET)))
#define UART0_FR_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_FR_OFFSET)))

#define UART0_RX_PIN (1U << 0) // PA0
#define UART0_TX_PIN (1U << 1) // PA1

void UART_Init(void){
    RCGC_UART_R |= (1U << 0); // Enable clock for UART0
    RCGC_GPIO_R |= (1U << 0); // Enable clock for GPIO Port A
    volatile uint32_t delay = RCGC_UART_R; // Delay to allow clock to stabilize
    (void)delay; // Suppress unused variable warning
    GPIO_PORTA_AFSEL_R |= (UART0_RX_PIN | UART0_TX_PIN); // Enable alternate function for UART pins
    GPIO_PORTA_DEN_R |= (UART0_RX_PIN | UART0_TX_PIN); // Enable digital I/O for UART pins
    GPIO_PORTA_PCTL_R &= ~(0xFFU); // Clear PCTL bits for PA0 and PA1 just to make sure
    GPIO_PORTA_PCTL_R |= (1U << 0) | (1U << 4); // Configure pin functions for UART0
}

void UART_SendChar(char c){
    while(UART0_FR_R & TXFF); // Wait until the transmit FIFO is not full
    UART0_DR_R = c; // Send the character
}
void UART_SendString(const char *str){
    while(*str != '\0'){
        UART_SendChar(*str); 
        str++;
    }
}

