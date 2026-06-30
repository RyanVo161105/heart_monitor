#include <stdint.h>
#include "gpio.h"

/* 
 *TM4C123GXL onboard RGB LED pins:
 *PF1 = RED
 *PF2 = BLUE
 *PF3 = GREEN
 *According to Tiva C Series TM4C123G LaunchPad Eval Board
 */

 /*
  * GPIO Port F register definitions
  *
  * Register address are calculated using:
  * GPIO PortF base address + register offset
  */
 #define GPIO_PORTF_BASE_ADDR 0x40025000U

 #define GPIO_DATA_OFFSET 0x3FCU //Allow all pins to be configured
 #define GPIO_DEN_OFFSET 0x51CU
 #define GPIO_DIR_OFFSET 0x400U

 #define GPIO_PORTF_DATA_R (*((volatile uint32_t *)(GPIO_PORTF_BASE_ADDR + GPIO_DATA_OFFSET)))
 #define GPIO_PORTF_DEN_R (*((volatile uint32_t *)(GPIO_PORTF_BASE_ADDR + GPIO_DEN_OFFSET)))
 #define GPIO_PORTF_DIR_R (*((volatile uint32_t *)(GPIO_PORTF_BASE_ADDR + GPIO_DIR_OFFSET)))

 /*Defining LEDs definitions*/
 #define LED_RED   (1U << 1)
 #define LED_BLUE  (1U << 2)
 #define LED_GREEN (1U << 3)
 #define LED_YELLOW (LED_RED | LED_GREEN) //Yellow = Red + Green
 