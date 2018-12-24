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
#include "B_stm32f4_gpio.h"
#include "pid_controller.h"



//extern variable
extern __IO int32_t count_temp1,count_temp2,speed;
extern __IO int32_t RMP,p_encoder,p_encoder_temp,PSP_encoder;
extern __IO  float PSP;
extern __IO int32_t count_recent1,count_recent2;
extern __IO int32_t  count_update1,PSP_Update,count_update2;
extern __IO uint32_t duty,Pule;
extern PIDControl pid;
char data_Buffer3[40];
char data_Buffer4[40];
__IO int32_t x,y=0xFFFF;
int32_t max(int32_t a, int32_t b){
	return a>b ? a : b;
}

int32_t min(int32_t a, int32_t b){
	return a<b ? a : b;
}
 void quay(int32_t psp_encoder, int32_t encoder ){
	 if(encoder < 0xAAAA){
	 x = max(x,encoder);
	 }else{
	 y = min(y,encoder);
	 }
	 int32_t A = 0xFFFF-y +x;
	 int step = A/100;
	 if(step !=0){
		 if(encoder > 0xAAAA){
			speed = 100 -((0xFFFF - encoder) / step);
		 }
	 speed = 100-(encoder / step);
	 }
	 sprintf(data_Buffer3, "ABC: %d\r\n",speed);//338
	 B_USART_DMA_Send(USART1, (uint8_t *)data_Buffer3, strlen(data_Buffer3));
	if(psp_encoder > 0 ){
		GPIO_ResetBits(GPIOA,GPIO_Pin_14);
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		TIM1->CCR1 = 21 * speed;
	}else if(psp_encoder < 0 ){
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		GPIO_SetBits(GPIOA,GPIO_Pin_14);
		TIM1->CCR1 = 21 * speed;
	}
}
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
		if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==SET)
	{
		if(TIM4->CR1 & 0x10) // check direction
		{
			count_temp2 -= 0xFFFF;
		}
		else
		{
			count_temp2 += 0xFFFF;
		}
		TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==SET)
	{
	
	//	GPIO_ToggleBits(GPIOG,GPIO_Pin_13);
		count_recent1 =  count_temp1 + TIM3->CNT;
	/*number pules of encoder4 */
		p_encoder = TIM4->CNT;
				sprintf(data_Buffer4, "E_Right: %d\r\n",PSP_encoder);//338
				B_USART_DMA_Send(USART1, (uint8_t *)data_Buffer4, strlen(data_Buffer4));
		
	//dem xung encoder2
		count_recent2 = (int32_t)TIM4->CNT;
		if(count_recent2 > 32767)
			count_recent2 = 65535-count_recent2;
		else if(count_recent2 < -32767)
			count_recent2 += 65535;
		
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//		
		if(p_encoder < 0xAAAA && p_encoder_temp < 0xAAAA){
			PSP_encoder=(int32_t)(p_encoder - p_encoder_temp);
		}
		
		if(p_encoder > 0xAAAA&& p_encoder_temp > 0xAAAA){
			PSP_encoder=(int32_t)(p_encoder - p_encoder_temp);
		}
//				sprintf(data_Buffer3, " %d\r\n",PSP_encoder);//338
//	  B_USART_DMA_Send(USART1, (uint8_t *)data_Buffer3, strlen(data_Buffer3));
		//quay(PSP_encoder);
		/* compute pule in 10ms*/
		if(count_recent1 > count_update1)
		{
			PSP=(float)(count_recent1-count_update1);
		}
		else if (count_recent1 <= count_update1)
		{
			PSP = -(float)(count_recent1-count_update1);
		}
	/*compute PID*/
	PSP = (float)(PSP_Update-(float)LPF_Beta*(PSP_Update-PSP));
	PIDInputSet(&pid,PSP);
	PIDCompute(&pid);
	duty=(uint32_t)PIDOutputGet(&pid);
	
		/*PWM*/
	TIM1->CCR1 = duty;
	//quay(PSP_encoder);
	PSP_Update=PSP;
	count_update1 = count_temp1 + TIM3->CNT;
	/*update count encoder 2*/
	p_encoder_temp = TIM4->CNT;
	}
}
