/**
  ******************************************************************************
  * @file    LTDC/LTDC_Display_1Layer/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "B_stm32f4_ili9341.h"
#include "B_stm32f4_fonts.h"
#include "tm_stm32f4_delay.h"
#include "B_stm32f4_ili9341.h"
#include "B_stm32f4_tim.h"
#include "stdio.h"
#include "B_stm32f4_usart.h"
#include "pid_controller.h"

//extern variable
extern __IO int32_t count_temp1;
extern __IO int32_t RMP,PSP;
extern __IO int32_t count_recent1;
extern __IO int32_t  count_update1,PSP_Update;
extern __IO uint16_t duty,Pule;
extern PIDControl pid;

//define he so beta
#define LPF_Beta 0.25
__IO int32_t RMP_Update;
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup LTDC_Display_1Layer
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
	{
		if(TIM3->CR1 & 0x10) // check direction
		{
			count_temp1 -= 0xFFFF;
		}
		else
		{
			count_temp1 += 0xFFFF;
		}
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	}
	
}

void TIM4_IRQHandler(void)
{

}

void TIM2_IRQHandler(void)
{
	if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==SET)
	{
		count_recent1 = count_temp1 + TIM3->CNT;
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		if(count_recent1 > count_update1)
		{
			PSP=((count_recent1-count_update1));
		}
		else if (count_recent1 < count_update1)
		{
			PSP=-((count_recent1-count_update1));
		}
		else if (count_recent1 == count_update1)
		{
			PSP=0.0;
		}
	}
	
	PSP = PSP_Update-LPF_Beta*(PSP_Update-PSP);
	PIDInputSet(&pid,PSP);
	PIDCompute(&pid);
	duty=PIDOutputGet(&pid);
	TIM1_PWM(duty);
	PSP_Update=PSP;
	count_update1 = count_temp1 + TIM3->CNT;
}

	
//	//SmoothData = SmoothData - (LPF_Beta * (SmoothData - RawData));
//		RMP=RMP_Update-LPF_Beta*(RMP_Update-RMP);
//	// tinh khoang thoi gian no tinh toan thi dong co quay duoc bao nhieu
//		PIDInputSet(&pid,(float)(RMP));
//		PIDCompute(&pid);
//		duty=PIDOutputGet(&pid);
//		TIM1_PWM(duty);
//		RMP_Update=RMP;


