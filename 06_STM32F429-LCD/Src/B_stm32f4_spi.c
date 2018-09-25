#include "B_stm32f4_spi.h"
GPIO_InitTypeDef 					GPIO_InitStructure;
SPI_InitTypeDef   				SPI_InitStructure;
NVIC_InitTypeDef 					NVIC_InitStructure;

/* Init SPI5*/
void  SPI5_Init(void)
{
		// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
		B_GPIO_InitAlternate(GPIOF, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9,RCC_AHB1Periph_GPIOF, GPIO_AF_SPI5);

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI5, &SPI_InitStructure);		

		SPI_Cmd(SPI5, ENABLE);	
	
	
}

