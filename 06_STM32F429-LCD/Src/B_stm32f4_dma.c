#include "B_stm32f4_dma.h"
#define GET_STREAM_NUMBER_DMA1(stream)    (((uint32_t)(stream) - (uint32_t)DMA1_Stream0) / (0x18))
#define GET_STREAM_NUMBER_DMA2(stream)    (((uint32_t)(stream) - (uint32_t)DMA2_Stream0) / (0x18))
const static uint8_t DMA_Flags_Bit_Pos[4] = {
	0, 6, 16, 22
};


void TM_DMA_ClearFlag(DMA_Stream_TypeDef* DMA_Stream, uint32_t flag) {
	uint32_t location;
	uint32_t stream_number;

	/* Check stream value */
	if (DMA_Stream < DMA2_Stream0) {
		location = (uint32_t)&DMA1->LIFCR;
		stream_number = GET_STREAM_NUMBER_DMA1(DMA_Stream);
	} else {
		location = (uint32_t)&DMA2->LIFCR;
		stream_number = GET_STREAM_NUMBER_DMA2(DMA_Stream);
	}
	
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

//void B_DMA_Init(void)
//{
//	DMA
//}
