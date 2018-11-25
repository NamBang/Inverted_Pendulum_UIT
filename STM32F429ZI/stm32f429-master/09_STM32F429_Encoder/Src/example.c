/**
 * Tutorial on how to use DMA to receive data from UART together with IDLE line feature on USART
 *
 * @author    Tilen Majerle
 * @email     tilen@majerle.eu
 * @website   http://stm32f4-discovery.net
 *
 * Example was tested on Nucleo-F411 and Nucleo-F401 where VCP is connected to:
 *   - USART2, TX: PA2, RX: PA3, used baudrate: 115200
 *   - USART2 DMA: DMA1 Stream5 Channel 4
 *   
 */
/* Include core modules */
#include "stm32f4xx.h"
#include "string.h"

/* Receive buffer for DMA */
#define DMA_RX_BUFFER_SIZE          64
uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];

/* Buffer after received data */
#define UART_BUFFER_SIZE            256
uint8_t UART_Buffer[UART_BUFFER_SIZE];
size_t Write, Read;

USART_InitTypeDef USART_InitStruct;
DMA_InitTypeDef DMA_InitStruct;
GPIO_InitTypeDef GPIO_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

int main(void) {
	/* Initialize system */
	SystemInit();
    
    /* Init GPIO pins for UART */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_DMA1EN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    (void)RCC->AHB1ENR;
    (void)RCC->APB1ENR;
    
    /* Set alternate functions */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    
    /* Init GPIO pins */
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* Configure UART setup */
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 921600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStruct);
    
    /* Enable global interrupts for USART */
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
    
    /* Enable USART */
    USART_Cmd(USART2, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    /* Enable IDLE line detection for DMA processing */
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    
    /* Configure DMA for USART RX, DMA1, Stream5, Channel4 */
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)DMA_RX_Buffer;
    DMA_InitStruct.DMA_BufferSize = DMA_RX_BUFFER_SIZE;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Init(DMA1_Stream5, &DMA_InitStruct);
    
    /* Enable global interrupts for DMA stream */
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    /* Enable transfer complete interrupt */
    DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Stream5, ENABLE);
	
    while (1) {
        /**
         * Loop data back to UART data register
         */
        while (Read != Write) {                 /* Do it until buffer is empty */
            USART2->DR = UART_Buffer[Read++];   /* Start byte transfer */
            while (!(USART2->SR & USART_SR_TXE));   /* Wait till finished */
            if (Read == UART_BUFFER_SIZE) {     /* Check buffer overflow */
                Read = 0;
            }
        }
    }
}

/**
 * \brief       Global interrupt handler for USART2
 */
void USART2_IRQHandler(void) {
    /* Check for IDLE flag */
    if (USART2->SR & USART_FLAG_IDLE) {         /* We want IDLE flag only */
        /* This part is important */
        /* Clear IDLE flag by reading status register first */
        /* And follow by reading data register */
        volatile uint32_t tmp;                  /* Must be volatile to prevent optimizations */
        tmp = USART2->SR;                       /* Read status register */
        tmp = USART2->DR;                       /* Read data register */
        (void)tmp;                              /* Prevent compiler warnings */
        DMA1_Stream5->CR &= ~DMA_SxCR_EN;       /* Disabling DMA will force transfer complete interrupt if enabled */
    }
}

/**
 * \brief       Global interrupt handler for DMA1 stream5
 * \note        Except memcpy, there is no functions used to 
 */
void DMA1_Stream5_IRQHandler(void) {
    size_t len, tocopy;
    uint8_t* ptr;
    
    /* Check transfer complete flag */
    if (DMA1->HISR & DMA_FLAG_TCIF5) {
        DMA1->HIFCR = DMA_FLAG_TCIF5;           /* Clear transfer complete flag */
        
        /* Calculate number of bytes actually transfered by DMA so far */
        /**
         * Transfer could be completed by 2 events:
         *  - All data actually transfered (NDTR = 0)
         *  - Stream disabled inside USART IDLE line detected interrupt (NDTR != 0)
         */
        len = DMA_RX_BUFFER_SIZE - DMA1_Stream5->NDTR;
        tocopy = UART_BUFFER_SIZE - Write;      /* Get number of bytes we can copy to the end of buffer */
        
        /* Check how many bytes to copy */
        if (tocopy > len) {
            tocopy = len;
        }
        
        /* Write received data for UART main buffer for manipulation later */
        ptr = DMA_RX_Buffer;
        memcpy(&UART_Buffer[Write], ptr, tocopy);   /* Copy first part */
        
        /* Correct values for remaining data */
        Write += tocopy;
        len -= tocopy;
        ptr += tocopy;
        
        /* If still data to write for beginning of buffer */
        if (len) {
            memcpy(&UART_Buffer[0], ptr, len);      /* Don't care if we override Read pointer now */
            Write = len;
        }
        
        /* Prepare DMA for next transfer */
        /* Important! DMA stream won't start if all flags are not cleared first */
        DMA1->HIFCR = DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5;
        DMA1_Stream5->M0AR = (uint32_t)DMA_RX_Buffer;   /* Set memory address for DMA again */
        DMA1_Stream5->NDTR = DMA_RX_BUFFER_SIZE;    /* Set number of bytes to receive */
        DMA1_Stream5->CR |= DMA_SxCR_EN;            /* Start DMA transfer */
    }
}