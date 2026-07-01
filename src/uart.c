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
#define UART_FR_TXFF (1U << 5)

#define UART0_DR_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_DR_OFFSET)))
#define UART0_FR_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_FR_OFFSET)))

#define UART0_IBRD_OFFSET 0x024U
#define UART0_FBRD_OFFSET 0x028U
#define UART0_LCRH_OFFSET 0x02CU
#define UART0_CTL_OFFSET 0x030U
#define UART0_CC_OFFSET 0xFC8U

#define UART0_IBRD_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_IBRD_OFFSET)))
#define UART0_FBRD_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_FBRD_OFFSET)))
#define UART0_LCRH_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_LCRH_OFFSET)))
#define UART0_CTL_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_CTL_OFFSET)))
#define UART0_CC_R (*((volatile uint32_t *)(UART0_BASE_ADDR + UART0_CC_OFFSET)))

#define UARTEN (1U << 0)
#define RXE (1U << 9)
#define TXE (1U << 8)
#define WLEN_8 ((1U << 5) | (1U << 6)) // Word length: 8 bits
#define SYSCLK 0U

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
    UART0_CTL_R &= ~UARTEN; // Disable UART0 before configuration
    UART0_IBRD_R = 8; // Set integer part of baud rate divisor for 115200 baud
    UART0_FBRD_R = 44; // Set fractional part of baud rate divisor for 115200 baud
    UART0_LCRH_R = WLEN_8; // Set word length to 8 bits
    UART0_CC_R = SYSCLK; // Use system clock for UART
    UART0_CTL_R |= (UARTEN | RXE | TXE); // Enable UART0, RX, and TX
}

void UART_SendChar(char c){
    while(UART0_FR_R & UART_FR_TXFF); // Wait until the transmit FIFO is not full
    UART0_DR_R = c; // Send the character
}
void UART_SendString(const char *str){
    while(*str != '\0'){
        UART_SendChar(*str); 
        str++;
    }
}

