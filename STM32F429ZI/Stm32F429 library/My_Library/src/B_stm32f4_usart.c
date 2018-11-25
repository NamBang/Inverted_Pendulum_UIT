#include <stdio.h>
#include "B_stm32f4_usart.h"


static void USARTx_NVIC(USART_TypeDef* USARTx,uint8_t USARTx_IRQn);
static void USARTx_GPIO(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_TX,uint16_t GPIO_Pin_RX);
static void USART_Config(USART_TypeDef* USARTx,uint32_t BaudRate );

void USART_PutChar(char c)
{
    // Wait until transmit data register is empty
    while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    // Send a char using USART1
    USART_SendData(USART1, c);
}

void USART_PutString(char *s)
{
   // Send a string
    while (*s)
    {
			USART_PutChar(*s++);
    }
}

void USART1_Init(void)
{
	// Enable clock for USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  // Enable clock for GPIOB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//Connect PA9 to USART1_Tx
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	// Connect PA10 to USART1_Rx
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	//initialization GPIO_USART 
	USARTx_GPIO(GPIOA,GPIO_Pin_9,GPIO_Pin_10);
	
	// Initialization of USARTx
	USART_Config( USART1,115200);
	//Interrup
	USARTx_NVIC(USART1,USART1_IRQn);//
	//Enable Interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// Enable USART1
	USART_Cmd(USART1, ENABLE);
}

static void USARTx_GPIO(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_TX,uint16_t GPIO_Pin_RX)
{
	// Initialization of GPIOx
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	 = GPIO_Pin_TX | GPIO_Pin_RX;//change GPIO at "B_stm32f4_usart.h"
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}
static void USART_Config(USART_TypeDef* USARTx,uint32_t BaudRate  )
{
	// Initialization of USARTx
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate 						=	BaudRate;
	USART_InitStruct.USART_HardwareFlowControl  =	USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 								=	USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity 							=	USART_Parity_No;
	USART_InitStruct.USART_StopBits 						=	USART_StopBits_1;
	USART_InitStruct.USART_WordLength 					=	USART_WordLength_8b;
	USART_Init(USARTx, &USART_InitStruct);
}
static void USARTx_NVIC(USART_TypeDef* USARTx,uint8_t USARTx_IRQn)
{
	NVIC_InitTypeDef nvicInit;
	//detele flag first
	USART_ClearFlag(USARTx,USART_IT_RXNE);
	//Config NVIC
	nvicInit.NVIC_IRQChannel										= USARTx_IRQn;
	nvicInit.NVIC_IRQChannelCmd									= ENABLE;
	nvicInit.NVIC_IRQChannelPreemptionPriority	= 0;
	nvicInit.NVIC_IRQChannelSubPriority					= 0;
	NVIC_Init(&nvicInit);
}

/* printf handler */
int fputc(int ch, FILE* fil)
{
	/* Wait to be ready, buffer empty */
	USART_PutChar( (char) ch);

	/* Return OK */
	return ch;
}

void Delay_ms(uint16_t time)
{
uint32_t time_n=time*12000;
	while(time_n!=0){time_n--;}

}
