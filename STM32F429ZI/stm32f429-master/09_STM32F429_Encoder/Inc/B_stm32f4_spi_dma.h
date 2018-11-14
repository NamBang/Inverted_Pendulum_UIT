#ifndef B_SPI_DMA_H
#define B_SPI_DMA_H 111


#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "B_stm32f4_dma.h"
#include "B_stm32f4_spi.h"
#include "stdlib.h"


/* SPI5 TX and RX default settings */
#ifndef SPI5_DMA_TX_STREAM
#define SPI5_DMA_TX_STREAM    DMA2_Stream6
#define SPI5_DMA_TX_CHANNEL   DMA_Channel_7
#endif
#ifndef SPI5_DMA_RX_STREAM
#define SPI5_DMA_RX_STREAM    DMA2_Stream5
#define SPI5_DMA_RX_CHANNEL   DMA_Channel_7
#endif


//define function
void SPI5_DMA_Init(void);
uint8_t TM_SPI_DMA_SendHalfWord(SPI_TypeDef* SPIx, uint16_t value, uint16_t count);
uint8_t TM_SPI_DMA_Working(SPI_TypeDef* SPIx);



































#endif
