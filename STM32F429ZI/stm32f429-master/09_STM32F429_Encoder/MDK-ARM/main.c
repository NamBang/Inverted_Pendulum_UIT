#include <stdio.h>
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

//khai bao bien
 __IO int32_t count_temp1;
 __IO int32_t RMP,PSP;
 __IO int32_t count_recent1;
 __IO int32_t  count_update1,PSP_Update;
 __IO uint16_t duty,Pule,speed, set_speed, time, count_value, flag_set=0;
 PIDControl pid;
 __IO int32_t count_e1=0,count_e2=0;

int main()
{
//  char data_Buffer[50];
//  char data_Buffer1[50];
	char data_Buffer2[50];
	//use external clock
	Clock_HSE();
	/* Initialize Encoder */
  encoder1();
	encoder2();
	USART1_Init();
	TIM2_TIME();
	
	PIDInit(&pid,0,0,0,0.015,0,8399,AUTOMATIC,DIRECT);
	set_speed=25;
	PIDTuningKpSet (&pid,180.0);
	PIDTuningKiSet (&pid,32.0);//100
	PIDTuningKdSet (&pid,10.0);//0.09
	PIDSetpointSet(&pid,set_speed);
	while (1) {

		//print PSP
		sprintf(data_Buffer2, "PSP: %d\r\n",PSP);//338
		USART_PutString(data_Buffer2);
		//Delay_ms(500);
		
		
		
		//control motor
//		GPIO_ToggleBits(GPIOA,GPIO_Pin_12);
//		Delay_ms(100);
//		GPIO_ToggleBits(GPIOA,GPIO_Pin_14  );
//		Delay_ms(1000);
		
	}
}
