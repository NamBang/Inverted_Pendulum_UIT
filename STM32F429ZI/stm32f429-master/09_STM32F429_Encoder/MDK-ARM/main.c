#include <stdio.h>
#include "string.h"
#include"stm32f4xx.h"
/*PIN
//encoder  encoder2 timer4 counted
PB6 CH1
PB7 CH2
//encoder motor  encoder1 timer3 
PA6 Channel A
PA7 channel B

PA9 TX
PA10 RX
*/
/* Include my libraries here */
#include "tm_stm32f4_delay.h"
#include "B_stm32f4_usart.h"
#include "B_stm32f4_gpio.h"
#include "B_stm32f4_tim.h"
#include "stm32f4xx_it.h"
#include "pid_controller.h"
#include "B_stm32f4_dma.h"


//khai bao bien
 __IO int32_t count_temp1,count_temp2=0;
 __IO int32_t PSP_encoder, p_encoder, p_encoder_temp,count_temp;
 __IO  float PSP=0.0;
 __IO int32_t count_recent1=0,count_recent2=0;
 __IO int32_t  count_update1,count_update2;
 __IO float PSP_Update=0.0;
 __IO uint32_t duty=0,Pule,speed;

 __IO int32_t count_e1=0,count_e2=0;
 
/* Create USART working buffer */
char USART_Buffer[100] = "Hello namb bang to via USART2 with TX DMA\r\n";

int main()
{
	
	SystemInit();
//  char data_Buffer[50];
char data_Buffer1[50];
	char data_Buffer2[50];
	//use external clock
	Clock_HSE();
	B_GPIO_Init(GPIOG,GPIO_Pin_13,RCC_AHB1Periph_GPIOG,GPIO_Mode_OUT);
	/* Initialize Encoder */
  encoder1();
	encoder2();
	USART1_Init();

	B_USART_DMA_Init();
 	TIM2_TIME();
	TIM1_INIT();
	
	TIM1_PWM(8399);
	TIM1_PWM();

	PID_Init(20.0,900.0,220.0,90.0);
	
	while (1) {
	quay(PSP_encoder,TIM4->CNT);
//		PSP_encoder=0;
//	sprintf(data_Buffer2, "TIM3: %d\r\n",TIM3->CNT);//338
//	  B_USART_DMA_Send(USART1, (uint8_t *)data_Buffer2, strlen(data_Buffer2));
		//USART_PutString(data_Buffer2);
//		
//		sprintf(data_Buffer2, " %d\r\n",PSP_encoder);//338
//	  B_USART_DMA_Send(USART1, (uint8_t *)data_Buffer2, strlen(data_Buffer2));
		
//		count_temp = (int32_t)TIM4->CNT;
////		if(count_temp > 32767)
////			count_temp = 65535-count_temp;
////		else if(count_temp < -32767)
////			count_temp += 65535;
////		
//		sprintf(data_Buffer1, " count_temp %d\r\n",count_temp);//338
//	  B_USART_DMA_Send(USART1, (uint8_t *)data_Buffer1, strlen(data_Buffer1));
	}
}
