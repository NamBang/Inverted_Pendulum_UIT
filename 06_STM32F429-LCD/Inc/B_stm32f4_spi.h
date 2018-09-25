#ifndef B_SPI_H
#define B_SPI_H 200
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "B_stm32f4_gpio.h"
/**
 * @brief  Check SPI busy status
 */
#define SPI_IS_BUSY(SPIx) (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))
// Check SPI busy status
#define SPI_CHECK_ENABLED_RESP(SPIx, val)   if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return (val);}
//SPI wait till end
#define SPI_WAIT(SPIx)            while (SPI_IS_BUSY(SPIx))
static __INLINE uint8_t TM_SPI_Send(uint8_t data) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED_RESP(SPI5, 0);
	
	/* Wait for previous transmissions to complete if DMA TX enabled for SPI */
	SPI_WAIT(SPI5);
	
	/* Fill output buffer with data */
	SPI5->DR = data;
	
	/* Wait for transmission to complete */
	SPI_WAIT(SPI5);
	
	/* Return data from buffer */
	return SPI5->DR;
}
typedef enum {
	TM_SPI_DataSize_8b, /*!< SPI in 8-bits mode */
	TM_SPI_DataSize_16b /*!< SPI in 16-bits mode */        
} TM_SPI_DataSize_t;
void  SPI5_Init(void);
#endif
