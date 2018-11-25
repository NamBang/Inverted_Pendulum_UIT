#include "B_stm32f4_dma.h"
/*Define*/
typedef struct {
	uint32_t DMA_Channel;
	DMA_Stream_TypeDef* DMA_Stream;
} TM_USART_DMA_INT_t;

static DMA_InitTypeDef DMA_InitStruct;
const static uint8_t DMA_Flags_Bit_Pos[4] = {
	0, 6, 16, 22
};

void B_USART_DMA_Init(void)
{
	//enable clock DMA2
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	/*or
		RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_DMA2EN,ENABLE);
	*/
	/* configure DMA for USART1 TXDMA2, stream 7 channel4*/
	//TM_DMA_ClearFlags(DMA2_Stream7);
	DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	
	
}

uint8_t B_USART_DMA_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count)
{
	
	/* Check if DMA is working now*/
	if(DMA2_Stream7->NDTR)
	{
		return 0;
	}
	
	/*set DMA option*/
	DMA_InitStruct.DMA_BufferSize = count;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &USARTx->DR;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) &DataArray[0];
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
		/* Deinit first, clear all flags */
	TM_DMA_ClearFlags(DMA2_Stream7);
	/* Init DMA*/
	DMA_Init(DMA2_Stream7,&DMA_InitStruct);
	/* Enable DMA Stream*/
	DMA_Cmd(DMA2_Stream7,ENABLE);
	/* or
	DMA2_Stream7->CR |= DMA_SxCR_EN;
	*/
	/* Enable UART TX DMA */
	USART_DMACmd(USARTx,USART_DMAReq_Tx , ENABLE);
	
	/* or  Enable USART TX DMA 
	USARTx->CR3 |= USART_CR3_DMAT
	*/
	return 1;
}


void TM_DMA_ClearFlags(DMA_Stream_TypeDef* DMA_Stream) {
	/* Clear all flags */
	TM_DMA_ClearFlag(DMA_Stream, DMA_FLAG_ALL);
}

void TM_DMA_ClearFlag(DMA_Stream_TypeDef* DMA_Stream, uint32_t flag) {
	uint32_t location;
	uint32_t stream_number;

	/* Check stream value */

		location = (uint32_t)&DMA2->LIFCR;
		stream_number = GET_STREAM_NUMBER_DMA2(DMA_Stream);
	
	/* Get register offset */
	if (stream_number >= 4) {
		/* High registers for DMA clear */
		location += 4;
		
		/* Do offset for high DMA registers */
		stream_number -= 4;
	}
	
	/* Clear flags */
	*(__IO uint32_t *)location = (flag & DMA_FLAG_ALL) << DMA_Flags_Bit_Pos[stream_number];
}
