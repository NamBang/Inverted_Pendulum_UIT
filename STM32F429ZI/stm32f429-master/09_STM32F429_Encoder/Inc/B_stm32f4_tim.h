#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "B_stm32f4_gpio.h" 

#ifndef B_STM32F4_TIM_H
#define B_STM32F4_TIM_H 100

void encoder1(void);
void encoder2(void);
int32_t Encoder1_Get_Counter(void);
int32_t Encoder2_Get_Counter(void);
void TIM4_TIME(void);
void TIM1_INIT(void);
//void TIM1_PWM(__IO uint32_t Pulse );
void TIM1_PWM( );
void TIM2_TIME(void);
void control_PWM1(__IO uint8_t Dir );
#endif
