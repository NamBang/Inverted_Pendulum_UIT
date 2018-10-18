
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define TXBUFFERSIZE   (countof(aTxBuffer) - 1)
#define RXBUFFERSIZE   0x20
#define EVAL_COM1 USART1
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";
uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t ubNbrOfDataToTransfer = TXBUFFERSIZE;
uint8_t ubNbrOfDataToRead = RXBUFFERSIZE;
__IO uint8_t ubTxCounter = 0; 
__IO uint16_t uhRxCounter = 0; 
extern __IO uint32_t count_temp1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
* @brief  This function handles USRAT interrupt request.
* @param  None
* @retval None
*/
void USART1_IRQHandler(void)
{
//	config_GPIO_IO();
//  if(USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
//  {
//    /* Read one byte from the receive data register */
//    aRxBuffer[uhRxCounter++] = (USART_ReceiveData(EVAL_COM1) & 0x7F);
//		GPIO_SetBits(GPIOG,GPIO_Pin_13);
//		USART_PutString("Turn LED 13\r\n");
////    if(uhRxCounter == ubNbrOfDataToRead)
////    {
////      /* Disable the EVAL_COM1 Receive interrupt */
////      USART_ITConfig(EVAL_COM1, USART_IT_RXNE, DISABLE);
////    }
  }

//  if(USART_GetITStatus(EVAL_COM1, USART_IT_TXE) != RESET)
//  {   
//    /* Write one byte to the transmit data register */
//    USART_SendData(EVAL_COM1, aTxBuffer[ubTxCounter++]);

//    if(ubTxCounter == ubNbrOfDataToTransfer)
//    {
//      /* Disable the EVAL_COM1 Transmit interrupt */
//      USART_ITConfig(EVAL_COM1, USART_IT_TXE, DISABLE);
//    }
//  }
	
void TIM3_IRQHandler(void)
{
	uint16_t timer_temp1;
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
	{
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		timer_temp1 = TIM3->CNT;
		count_temp1 = timer_temp1+65535;
	}
	
}
void TIM4_IRQHandler(void)
{
	
}
