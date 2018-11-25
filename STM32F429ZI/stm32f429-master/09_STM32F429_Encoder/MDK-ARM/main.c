#include <stdio.h>
#include "string.h"
#include"stm32f4xx.h"
/*PIN
//encoder  encoder2 timer4 counted
PB6 CH1
PB7 CH2
//encoder motor  encoder1 timer 3 
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
 __IO int32_t count_temp1;
 __IO int32_t PSP_encoder, p_encoder, p_encoder_temp;
 __IO  float PSP=0.0;
 __IO int32_t count_recent1;
 __IO int32_t  count_update1,PSP_Update;
 __IO uint16_t duty,Pule,speed, set_speed;
 PIDControl pid;
 __IO int32_t count_e1=0,count_e2=0;
 void quay(uint32_t psp_encoder){
	if(psp_encoder > 0){
		control_PWM1(8399,0);
	}else{
		control_PWM1(8399,1);
	}
}
 
/* Create USART working buffer */
char USART_Buffer[100] = "Hello namb bang to via USART2 with TX DMA\r\n";

int main()
{
	SystemInit();
//  char data_Buffer[50];
//  char data_Buffer1[50];
	char data_Buffer2[50]="";
	//use external clock
	Clock_HSE();
	/* Initialize Encoder */
  encoder1();
	encoder2();
	USART1_Init();
	B_USART_DMA_Init();
////	

	TIM2_TIME();
	TIM1_PWM(8399);
	
	//TIM1_PWM(8399);
	
	PIDInit(&pid,0,0,0,0.03,0,8399,AUTOMATIC,DIRECT);
	set_speed=60;
	
	PIDTuningKpSet (&pid,400.0);
	PIDTuningKiSet (&pid,10.0);//100
	PIDTuningKdSet (&pid,0.0);//0.09
	PIDSetpointSet(&pid,set_speed);
	while (1) {
		B_USART_DMA_Send(USART1, (uint8_t *)USART_Buffer, strlen(USART_Buffer));
		sprintf(data_Buffer2, "PSP: %f\r\n",PSP);//338
	  B_USART_DMA_Send(USART1, (uint8_t *)data_Buffer2, strlen(data_Buffer2));
		//print PSP
//		USART_PutString(data_Buffer2);
		
//		sprintf(data_Buffer2, "PSP_encoder: %d\r\n",PSP_encoder);//338
//		USART_PutString(data_Buffer2);
		//quay(PSP_encoder);
	}
}


