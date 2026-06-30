#include <stdint.h>
#include "gpio.h"

/* 
 *TM4C123GXL onboard RGB LED pins:
 *PF1 = RED
 *PF2 = BLUE
 *PF3 = GREEN
 *According to Tiva C Series TM4C123G LaunchPad Eval Board
 */

 /* Configure Clock gating control for Port F*/
 #define RCGC_ADDR 0x400FE000U
 #define RCGC_GPIO_OFFSET 0x608U
 #define RCGC_GPIO_R (*((volatile uint32_t *)(RCGC_ADDR + RCGC_GPIO_OFFSET)))
 /*
  * GPIO Port F register definitions
  *
  * Register address are calculated using:
  * GPIO PortF base address + register offset
  */
 #define GPIO_PORTF_BASE_ADDR 0x40025000U

 #define GPIO_DATA_OFFSET 0x3FCU //Give full access for GPIODATA masked address
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
 #define LED_ALL (LED_RED | LED_BLUE | LED_GREEN) //All LEDs


void GPIO_LED_Init(void){
    RCGC_GPIO_R |= (1U << 5); //Enable clock for Port F
    volatile uint32_t delay = RCGC_GPIO_R; //Delay to allow clock to stabilize
    (void)delay; //Suppress unused variable warning
    GPIO_PORTF_DIR_R |= LED_ALL; //Set PF1, PF2, PF3 as output
    GPIO_PORTF_DEN_R |= LED_ALL; //Enable digital function for PF1, PF2, PF3
    GPIO_LED_AllOff(); //Turn off all LEDs initially
}
void GPIO_LED_AllOff(void){
    GPIO_PORTF_DATA_R &= ~LED_ALL; //Turn off all LEDs
}
void GPIO_RedLED_On(void){
    GPIO_LED_AllOff(); //Turn off all LEDs before turning on Red LED
    GPIO_PORTF_DATA_R |= LED_RED; //Turn on Red LED
}
void GPIO_RedLED_Off(void){
    GPIO_PORTF_DATA_R &= ~LED_RED; //Turn off Red LED
}
void GPIO_RedLED_Toggle(void){
    GPIO_PORTF_DATA_R ^= LED_RED; //Toggle Red LED
}
void GPIO_BlueLED_On(void){
    GPIO_LED_AllOff(); //Turn off all LEDs before turning on Blue LED
    GPIO_PORTF_DATA_R |= LED_BLUE; //Turn on Blue LED
}
void GPIO_BlueLED_Off(void){
    GPIO_PORTF_DATA_R &= ~LED_BLUE; //Turn off Blue LED
}
void GPIO_BlueLED_Toggle(void){
    GPIO_PORTF_DATA_R ^= LED_BLUE; //Toggle Blue LED
}
void GPIO_GreenLED_On(void){
    GPIO_LED_AllOff(); //Turn off all LEDs before turning on Green LED
    GPIO_PORTF_DATA_R |= LED_GREEN; //Turn on Green LED
}
void GPIO_GreenLED_Off(void){
    GPIO_PORTF_DATA_R &= ~LED_GREEN; //Turn off Green LED
}
void GPIO_GreenLED_Toggle(void){
    GPIO_PORTF_DATA_R ^= LED_GREEN; //Toggle Green LED
}
void GPIO_YellowLED_On(void){
    GPIO_LED_AllOff(); //Turn off all LEDs before turning on Yellow LED
    GPIO_PORTF_DATA_R |= LED_YELLOW; //Turn on Yellow LED
}
void GPIO_YellowLED_Off(void){
    GPIO_PORTF_DATA_R &= ~LED_YELLOW; //Turn off Yellow LED
}
void GPIO_YellowLED_Toggle(void){
    GPIO_PORTF_DATA_R ^= LED_YELLOW; //Toggle Yellow LED
}