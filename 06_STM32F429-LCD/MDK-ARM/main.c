#include <stdio.h>
#include"stm32f4xx.h"

/*PIN
PB6 TX
PB7 RX
PA6 Channel A
PA7 channel B
IN1 PA9
IN2 PB14
*/
/* Include my libraries here */
#include "tm_stm32f4_delay.h"
#include "B_stm32f4_tim.h"
#include "stm32f4xx_it.h"
#include "B_stm32f4_usart.h"
#include "pid_controller.h"

__IO int32_t count_temp1=0;
__IO int32_t RMP=0;
__IO int32_t count_encoder = 0;
__IO int32_t count_recent1 =0;
__IO int32_t count_update1=0;
__IO int32_t count_encoder2 = 0;
__IO int32_t  change=0;
__IO uint16_t duty=0, speed, set_speed, time, count_value, flag_set=0;

float kp= 0;
float ki= 0;
float kd= 0;
PIDControl pid;

int main()
{
//	char data_Buffer[50];
	char data_Buffer1[50];
	//xai xung clock ngoai
	Clock_HSE();
	/* Initialize Encoder */
	encoder1();
	/*initialize timer4*/
	TIM4_TIME();
	TIM1_INIT();
	TIM1_PWM(8399);
	/* Init UART */
	USART1_Init();
	PIDInit(&pid,0,0,0,0.015,0,8399,AUTOMATIC,DIRECT);
	set_speed=50.0;
	// kp=20 ki=5 kd=70
	PIDTuningKpSet (&pid,180.0);
	PIDTuningKiSet (&pid,32.0);//100
	PIDTuningKdSet (&pid,10.0);//0.09
	PIDSetpointSet(&pid,set_speed);
	while (1) {
//  	set_speed=50.0;
//		// kp=20 ki=5 kd=70
//		PIDTuningKpSet (&pid,170.0);
//		PIDTuningKiSet (&pid,10.0);
//		PIDTuningKdSet (&pid,60.0);
//		PIDSetpointSet(&pid,set_speed);
//		
//	
//		PIDInputSet(&pid,(float)(RMP));
//		PIDCompute(&pid);
//		duty=PIDOutputGet(&pid);
//		TIM1_PWM(duty);
		
//	  sprintf(data_Buffer2, "duty: %d\r\n",duty);//338
//		USART_PutString(data_Buffer2);
		sprintf(data_Buffer1, "\r\n%d ",RMP);//338
		USART_PutString(data_Buffer1);
		
	}
}

