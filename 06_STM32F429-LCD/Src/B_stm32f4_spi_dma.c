#include "B_stm32f4_spi_dma.h"

/* Private structure */
typedef struct {
	uint32_t TX_Channel;
	DMA_Stream_TypeDef* TX_Stream;
	uint32_t RX_Channel;
	DMA_Stream_TypeDef* RX_Stream;
	uint32_t Dummy32;
	uint16_t Dummy16;
} SPI_DMA_INT_t;

static SPI_DMA_INT_t SPI5_DMA_INT = {SPI5_DMA_TX_CHANNEL, SPI5_DMA_TX_STREAM, SPI5_DMA_RX_CHANNEL, SPI5_DMA_RX_STREAM};
static SPI_DMA_INT_t* SPI_DMA_INT_GetSettings(void);
/* Private DMA structure */
static DMA_InitTypeDef DMA_InitStruct;

void SPI5_DMA_Init()
{
	/* Enable DMA2 clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	/* Set DMA options for TX stream */
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
}

static SPI_DMA_INT_t* SPI_DMA_INT_GetSettings(void)
{
	SPI_DMA_INT_t* result;
	result = &SPI5_DMA_INT;
	return result;
}

uint8_t TM_SPI_DMA_SendHalfWord(SPI_TypeDef* SPIx, uint16_t value, uint16_t count) {
	/* Get USART settings */
	SPI_DMA_INT_t* Settings = SPI_DMA_INT_GetSettings();
	
	/* Check if DMA available */
	if (Settings->TX_Stream->NDTR) {
		return 0;
	}
	
	/* Set dummy memory to value we specify */
	Settings->Dummy16 = value;
	
	/* Set DMA peripheral address, number of bytes and disable memory increase pointer */
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &SPIx->DR;
	DMA_InitStruct.DMA_BufferSize = count;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) &Settings->Dummy16;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
	
	/* Configure TX DMA */
	DMA_InitStruct.DMA_Channel = Settings->TX_Channel;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	
	/* Set memory size */
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	/* Deinit first TX stream */
	TM_DMA_ClearFlag(Settings->TX_Stream, DMA_FLAG_ALL);
	
	/* Init TX stream */
	DMA_Init(Settings->TX_Stream, &DMA_InitStruct);
	
	/* Enable TX stream */
	Settings->TX_Stream->CR |= DMA_SxCR_EN;
	
	/* Enable SPI TX DMA */
	SPIx->CR2 |= SPI_CR2_TXDMAEN;
	
	/* Return OK */
	return 1;
}

uint8_t TM_SPI_DMA_Working(SPI_TypeDef* SPIx) {
	/* Get SPI settings */
	SPI_DMA_INT_t* Settings = SPI_DMA_INT_GetSettings();
	
	/* Check if TX or RX DMA are working */
	return (
		Settings->RX_Stream->NDTR || /*!< RX is working */
		Settings->TX_Stream->NDTR || /*!< TX is working */
		SPI_IS_BUSY(SPIx)            /*!< SPI is busy */
	);
}
