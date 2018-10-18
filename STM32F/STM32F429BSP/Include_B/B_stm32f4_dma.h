#ifndef TM_DMA_H
#define TM_DMA_H 110
#include "stm32f4xx.h"
#include "misc.h"
#define DMA_FLAG_TCIF    ((uint32_t)0x00000020) /*!< DMA stream transfer complete */
#define DMA_FLAG_HTIF    ((uint32_t)0x00000010) /*!< DMA stream half transfer complete */
#define DMA_FLAG_TEIF    ((uint32_t)0x00000008) /*!< DMA stream transfer error */
#define DMA_FLAG_DMEIF   ((uint32_t)0x00000004) /*!< DMA stream direct mode error */
#define DMA_FLAG_FEIF    ((uint32_t)0x00000001) /*!< DMA stream FIFO error */
#define DMA_FLAG_ALL     ((uint32_t)0x0000003D) /*!< DMA stream all flags */

/* DMA1 preemption priority */
#ifndef DMA1_NVIC_PREEMPTION_PRIORITY
#define DMA1_NVIC_PREEMPTION_PRIORITY   0x01
#endif

/* DMA2 preemption priority */
#ifndef DMA2_NVIC_PREEMPTION_PRIORITY
#define DMA2_NVIC_PREEMPTION_PRIORITY   0x01
#endif

void TM_DMA_ClearFlag(DMA_Stream_TypeDef* DMA_Stream, uint32_t flag);






















#endif
