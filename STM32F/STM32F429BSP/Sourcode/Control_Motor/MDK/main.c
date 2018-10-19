#include <stdio.h>
#include"stm32f4xx.h"

/*PIN
PB6 TX
PB7 RX
PA6 Channel A
PA7 channel B
IN1 PA9 CH2
PA10 
PA12
IN2 PB14 CH2N
BaudRate = 9600
*/
/* Include my libraries here */
#include "tm_stm32f4_delay.h"
#include "B_stm32f4_tim.h"
#include "stm32f4xx_it.h"
#include "B_stm32f4_usart.h"
#include "pid_controller.h"

__IO int32_t count_temp1=0,count_temp12=0;
__IO int32_t RMP=0;
__IO int32_t count_encoder = 0;
__IO int32_t count_recent1 =0;
__IO int32_t count_update1=0;
__IO int32_t count_encoder2 = 0;
__IO int32_t  change=0;
__IO uint16_t duty=0, speed, set_speed, time, count_value, flag_set=0;
__IO int32_t count=0;

float kp= 0;
float ki= 0;
float kd= 0;
PIDControl pid;
void Configure_PD0(void);
int main()
{
//	char data_Buffer[50];
//	char data_Buffer1[50];
	//xai xung clock ngoai
	Clock_HSE();
	Configure_PD0();
//	/* Initialize Encoder */
//	encoder1();
//	/*initialize timer4*/
//	TIM4_TIME();
//	TIM1_INIT();
//	TIM1_PWM(4199);
//	/* Init UART */
//	USART1_Init();
//	
//	//PIDInit(&pid,0,0,0,0.015,0,8399,AUTOMATIC,DIRECT);
//	//set_speed=50.0;
//	// kp=20 ki=5 kd=70
////PIDTuningKpSet (&pid,180.0);
//	//PIDTuningKiSet (&pid,32.0);//100
//	//PIDTuningKdSet (&pid,10.0);//0.09
////	PIDSetpointSet(&pid,set_speed);
//	//USART_SendData(USART1, 'd');
//	
//	B_GPIO_Init(GPIOG,GPIO_Pin_13,RCC_AHB1Periph_GPIOG,GPIO_Mode_OUT);
//	B_GPIO_Init(GPIOG,GPIO_Pin_14,RCC_AHB1Periph_GPIOG,GPIO_Mode_OUT);
//	

////Enable clock for GPOIG
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//Initialize struct
GPIO_InitTypeDef GPIO_InitDef;
 
//Pins 13 and 14
GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_8;
//Mode output
GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
//Output type push-pull
GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
//Without pull resistors
GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
//50MHz pin speed
GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
 
//Initialize pins on GPIOG port
GPIO_Init(GPIOA, &GPIO_InitDef);
GPIO_ResetBits(GPIOA,GPIO_Pin_12);
Delay_ms(100);
GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
		TM_LEDS_Init();
	/* Init timer */
	TM_TIMER_Init();
	/* Init PWM */
	TM_PWM_Init();
	while (1) {
//	GPIO_SetBits(GPIOA,GPIO_Pin_12);
//	Delay_ms(1000);
	
	GPIO_ToggleBits(GPIOA,GPIO_Pin_8);
		Delay_ms(100);
	GPIO_ToggleBits(GPIOA,GPIO_Pin_12);
	Delay_ms(10000);
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
		
		// de xuat tranh truong hop bi tran 
//		count_temp12 = (int32_t)TIM3->CNT;
//		if(count_temp12 > 32767)
//			count_temp12 -= 65535;
//		else if(count_temp12 < -32767)
//			count_temp12 += 65535;
//		
//	  sprintf(data_Buffer1, "count: %d\r\n",count_temp12);//338
//		USART_PutString(data_Buffer1);
//		Delay_ms(1000);
//		
//	//	GPIO_ToggleBits(GPIOA,GPIO_Pin_12);
////		GPIO_ToggleBits(GPIOA,GPIO_Pin_10);
//		Delay_ms(1000);
//		
//		GPIO_ToggleBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);
//		Delay_ms(10000);
////		sprintf(data_Buffer1, "\r\n%d ",RMP);//338
////		USART_PutString(data_Buffer1);
//		
	}
}


void Configure_PD0(void) {
    /* Set variables used */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* Tell system that you will use PD0 for EXTI_Line0 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);
    
    /* PD0 is connected to EXTI_Line0 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);
 
    /* Add IRQ vector to NVIC */
    /* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}
