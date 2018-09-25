

TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
TIM_OCInitTypeDef          TIM_OCInitStructure;
GPIO_InitTypeDef           GPIO_InitStructure;
 
void TIM_PWM_Configuration(void);
 
int main(void)
{
  TIM_PWM_Configuration();
  TIM1->CCR1 = 10 * 65535 / 100;  // 10% Duty cycle
  TIM1->CCR2 = 50 * 65535 / 100;  // 50% Duty cycle 
  TIM1->CCR3 = 90 * 65535 / 100;  // 90% Duty cycle
  while (1)
  {}
}
 
void TIM_PWM_Configuration(void)
{ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM1);
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;   // 65535
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCStructInit(&TIM_OCInitStructure);
  
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
  TIM_ARRPreloadConfig(TIM1, ENABLE);
 
  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
 
#ifdef  USE_FULL_ASSERT
 
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %drn", file, line) */
 
  while (1)
  {}
}
#endif







/**
	
  ******************************************************************************
  * @file    LTDC/LTDC_Display_1Layer/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "RGB565_480x272.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/*
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
TIM_OCInitTypeDef          TIM_OCInitStructure;
GPIO_InitTypeDef           GPIO_InitStructure;
 
void TIM_PWM_Configuration(void);
 
int main(void)
{
  TIM_PWM_Configuration();
  TIM1->CCR1 = 10 * 65535 / 100;  // 10% Duty cycle
  TIM1->CCR2 = 50 * 65535 / 100;  // 50% Duty cycle 
  TIM1->CCR3 = 90 * 65535 / 100;  // 90% Duty cycle
  while (1)
  {}
}
 
void TIM_PWM_Configuration(void)
{ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM1);
  

  TIM_TimeBaseStructure.TIM_Prescaler = 0;  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;   // 65535
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 0;
  //TIM_OCStructInit(&TIM_OCInitStructure);
  
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
  TIM_ARRPreloadConfig(TIM1, ENABLE);
 
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
 
*/
