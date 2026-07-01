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

#define UART0_RX_PIN (1U << 0) // PA0
#define UART0_TX_PIN (1U << 1) // PA1

