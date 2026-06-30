#ifndef GPIO_H
#define GPIO_H
/* 
 *TM4C123GXL onboard RGB LED pins:
 *PF1 = RED
 *PF2 = BLUE
 *PF3 = GREEN
 *According to Tiva C Series TM4C123G LaunchPad Eval Board
 */
void GPIO_LED_Init(void);
void GPIO_LED_AllOff(void);
// Basic colors
void GPIO_RedLED_On(void);
void GPIO_RedLED_Off(void);
void GPIO_RedLED_Toggle(void);

void GPIO_BlueLED_On(void);
void GPIO_BlueLED_Off(void);
void GPIO_BlueLED_Toggle(void);

void GPIO_GreenLED_On(void);
void GPIO_GreenLED_Off(void);
void GPIO_GreenLED_Toggle(void);

//Combined colors 
//Yellow = Red + Green
void GPIO_YellowLED_On(void);
void GPIO_YellowLED_Off(void);
void GPIO_YellowLED_Toggle(void);
#endif