#ifndef TM_DMA_H
#define TM_DMA_H 110
/* include library*/
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "B_stm32f4_usart.h"
/*function user define*/

/**
 * @brief  Clears all flags for selected DMA stream
 * @param  *DMA_Stream: Pointer to @ref DMA_Stream_TypeDef DMA stream where you want to clear flags 
 * @retval None
 */
void TM_DMA_ClearFlags(DMA_Stream_TypeDef* DMA_Stream);

/** 
 * @brief  Clears selected DMA interrupt flag
 * @param  *DMA_Stream: Pointer to @ref DMA_Stream_TypeDef DMA stream where you want to clear flags
 * @param  flags: Flag(s) which you wanna clear:
 *           - DMA_FLAG_TCIF: Transfer complete interrupt flag
 *           - DMA_FLAG HTIF: Half transfer complete interrupt flag
 *           - DMA_FLAG_TEIF: Transfer error interrupt flag
 *           - DMA_FLAG_DMEIF: Direct mode error interrupt flag
 *           - DMA_FLAG_FEIF: FIFO error interrupt flag
 *           - DMA_FLAG_ALL: All flags
 * @retval None
 */
void TM_DMA_ClearFlag(DMA_Stream_TypeDef* DMA_Stream, uint32_t flags);


#define DMA_FLAG_ALL     ((uint32_t)0x0000003D) /*!< DMA stream all flags */
#define GET_STREAM_NUMBER_DMA2(stream)    (((uint32_t)(stream) - (uint32_t)DMA2_Stream0) / (0x18))
/* Offsets for bits */


void B_USART_DMA_Init(void);
uint8_t B_USART_DMA_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count);



#endif
