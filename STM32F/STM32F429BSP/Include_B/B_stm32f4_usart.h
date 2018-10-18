#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"

#ifndef B_USART_H
#define B_USART_H 
void USART1_Init(void);
void USART2_Init(void);
void USART_PutChar(char c);
void USART_PutString(char *s);
void Delay_ms(uint16_t time);

#endif
