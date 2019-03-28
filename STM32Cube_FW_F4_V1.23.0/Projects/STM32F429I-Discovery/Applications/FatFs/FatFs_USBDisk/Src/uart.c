/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "stm32f4xx_hal.h"
#include "main.h"

DMA_HandleTypeDef hdma_usart1_tx;
UART_HandleTypeDef huart1;
/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void B_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

///**
//  * Enable DMA controller clock
//  */
void B_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Stream7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/