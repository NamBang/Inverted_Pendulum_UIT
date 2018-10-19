#include "B_stm32f4_tim.h"
///* Encoder variables */
//__IO int32_t Num_Encoder_1;
////use timer1 bam xung 
//void TIM1_INIT(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	/*Enable clock*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);
//	 
//	/*configure PWM Pin out TIM1 */
//	GPIO_InitTypeDef  GPIO_InitStructure;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;//CH2
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //chon che do chuc nang thay the
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Push Pull Mode||  GPIO_OType_OD: Output Open Drain Mode // tim hieu lai
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIOA, &GPIO_InitStructure); 
//  
// // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ; //-CH2N
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1); //Changes the mapping of the specified pin.
// // GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM1);//selects the pin to used as Alternate function.
//	
//	// config pin DDR1 DDR2

//	// Create 1kHz PWM
//	// TIM is connected to APB1 bus that have default clock 84MHz
//	// So, the frequency of TIM is 84MHz
//	// We use prescaler 10 here
//	// So, the frequency of TIM now is 8.4MHz
//	// TIM_Period determine the PWM frequency by this equation:
//	// PWM_frequency = timer_clock / (TIM_Period + 1)
//	// If we want 1kHz PWM we can calculate:
//	// TIM_Period = (timer_clock / PWM_frequency) - 1
//	// TIM_Period = (8.4MHz / 1kHz) - 1 = 8399
//	/*configure Freqency for PWM by: 84MHZ/(Period+1)/(Prescaler+1)*/
//	TIM_TimeBaseInitStructure.TIM_Prescaler= 9;
//	TIM_TimeBaseInitStructure.TIM_Period =8399;// gia tri cua thanh ghi ARR dem de duoc tan so pwm là môt khz 
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;// count down up
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=0x0;
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
////		/*Configure PWM*/
////  TIM_OCInitTypeDef TIM_OCInitStructure;
////	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//chon mode pwm1 oc: output compare
////	/*Chon muc cao cho dau ra, neu chon muc thap thi no se bi dao nguoc hinh dang dau ra, chu ky tan so deu nhu nhau*/
////	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
////	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
////	/*khia bao cho kenh dao*/
////	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
////	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;
////	
////	// Duty cycle calculation equation:
////	// TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
////	// Ex. 25% duty cycle:
////	// TIM_Pulse = (((8399 + 1) * 25) / 100) - 1 = 2099// gia tri thanh ghi ARR dem tu 0-2099 pwm muc cao va tu 2099 - 8399 PWM muc thap 
////	// TIM_Pulse = (((8399 + 1) * 75) / 100) - 1 = 6299
////	TIM_OCInitStructure.TIM_Pulse=6299;
////	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
////	TIM_OCInitStructure.TIM_OCNIdleState =TIM_OCNIdleState_Reset;
////	/*PWM Channel 1*/
////	TIM_OC2Init(TIM1, &TIM_OCInitStructure);//cho phep channel 2 active
////	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
////  TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
////  TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
////  TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
//	
//	
//	TIM_ARRPreloadConfig(TIM1, ENABLE);//preloadconfig toan bo timer
//		/*Enable PWM_TIM*/
//  TIM_Cmd(TIM1, ENABLE);
//	/* Main Output Enable */
//  TIM_CtrlPWMOutputs(TIM1, ENABLE);
//	
//}
//void TIM1_PWM(__IO uint32_t Pulse )
//{
//	
//	
//	/*Configure PWM*/
//  TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//chon mode pwm1 oc: output compare
//	/*Chon muc cao cho dau ra, neu chon muc thap thi no se bi dao nguoc hinh dang dau ra, chu ky tan so deu nhu nhau*/
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
//	/*khia bao cho kenh dao*/
//	//TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
////	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;
//	
//	// Duty cycle calculation equation:
//	// TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
//	// Ex. 25% duty cycle:
//	// TIM_Pulse = (((8399 + 1) * 25) / 100) - 1 = 2099// gia tri thanh ghi ARR dem tu 0-2099 pwm muc cao va tu 2099 - 8399 PWM muc thap 
//	// TIM_Pulse = (((8399 + 1) * 75) / 100) - 1 = 6299
//	TIM_OCInitStructure.TIM_Pulse=Pulse;
//	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
////	TIM_OCInitStructure.TIM_OCNIdleState =TIM_OCNIdleState_Reset;
//	/*PWM Channel 2*/
//	TIM_OC2Init(TIM1, &TIM_OCInitStructure);//cho phep channel 2 active
//	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
//  TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
//  TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
//  //TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
//	
//	
//	TIM_ARRPreloadConfig(TIM1, ENABLE);//preloadconfig toan bo timer

//}
////dung timer 3 de dem xung 
//void encoder1(void)
//{
//	
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef					NVIC_InitStructure;
//	
//	//enable colck for timers
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	//khoi tao phin noi 
//	B_GPIO_Init(GPIOA,GPIO_Pin_6 | GPIO_Pin_7,RCC_AHB1Periph_GPIOA,GPIO_Mode_AF);
//	
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
//	
//	/* Configure Timer for Encoder Interface 1*/		
//	TIM_TimeBaseInitStructure.TIM_Prescaler=0;												//Specifies the prescaler value used to divide the TIM clock.
//	TIM_TimeBaseInitStructure.TIM_Period=0xFFFF; 											// dem toi gia tri 
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//chon che do dem len
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				//Specifies the clock division
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
//	
//	/*Configure Encoder Interface 1 capture channel*/
//	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;	//lua chon channel
//	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;				//xac dinh che do hoat dong canh cho tin hieu dau vao
//	TIM_ICInitStructure.TIM_ICFilter=0; 														//chi dinh bo loc dau vao
//	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;		//lua chon dau vao truc tiep
//	TIM_ICInit(TIM3, &TIM_ICInitStructure);
//	
//	/*Configure Encoder Interface 1*/
//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	
//	/*Reset Counter 1*/
//	TIM_SetCounter(TIM3, 0);	
//	
//	/*Clear Interrupt Update flag 1*/
//	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
//	
//	/*Enable Update Interrupt of 2ng TIMER*/
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	
//	/* 2 bit for pre-emption priority, 2 bits for subpriority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	/*Configure NVIC for Encoder Interface Update Interrupt 1*/
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
//	NVIC_Init(&NVIC_InitStructure);
//	Num_Encoder_1=0;
//	
//	/*Enable 1nd Timer */
//	TIM_Cmd(TIM3,ENABLE);	
//}

////dung timer 4 de tinh rpm
//void TIM4_TIME(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
//  TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef					NVIC_InitStructure;
//	
//	// TIM4 configuration 
//	TIM_TimeBaseInitStructure.TIM_Period = 99; // delay 100ms/999  300ms/9     
//	TIM_TimeBaseInitStructure.TIM_Prescaler = (8399);	 //10khz// do no dem tu 0
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0x0;    
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
//	
//	// Clear TIM4 update pending flags 
//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//	
//	//Enable TIM4 Update interrupts 
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//	
//	/* 2 bit for pre-emption priority, 2 bits for subpriority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
//	
//	// Enable the TIM4 global Interrupt 
//  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	//TIM enable counter 
//	TIM_Cmd(TIM4, ENABLE);	
//	
//}



//void encoder2()
//{
//}





/////////////////////////////////////////////////////////////////////////////////////////


void TM_LEDS_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Alternating functions for pins */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	
	/* Set pins */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void TM_TIMER_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	
	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
/*	
	TIM4 is connected to APB1 bus, which has on F407 device 42MHz clock 				
	But, timer has internal PLL, which double this frequency for timer, up to 84MHz 	
	Remember: Not each timer is connected to APB1, there are also timers connected 	
	on APB2, which works at 84MHz by default, and internal PLL increase 				
	this to up to 168MHz 															
	
	Set timer prescaller 
	Timer count frequency is set with 
	
	timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)		
	
	In our case, we want a max frequency for timer, so we set prescaller to 0 		
	And our timer will have tick frequency		
	
	timer_tick_frequency = 84000000 / (0 + 1) = 84000000 
*/	
	TIM_BaseStruct.TIM_Prescaler = 0;
	/* Count up */
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
/*
	Set timer period when it have reset
	First you have to know max value for timer
	In our case it is 16bit = 65535
	To get your frequency for PWM, equation is simple
	
	PWM_frequency = timer_tick_frequency / (TIM_Period + 1)
	
	If you know your PWM frequency you want to have timer period set correct
	
	TIM_Period = timer_tick_frequency / PWM_frequency - 1
	
	In our case, for 10Khz PWM_frequency, set Period to
	
	TIM_Period = 84000000 / 10000 - 1 = 8399
	
	If you get TIM_Period larger than max timer value (in our case 65535),
	you have to choose larger prescaler and slow down timer tick frequency
*/
    TIM_BaseStruct.TIM_Period = 8399; /* 10kHz PWM */
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM4 */
    TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	/* Start count on TIM4 */
    TIM_Cmd(TIM4, ENABLE);
}

void TM_PWM_Init(void) {
	TIM_OCInitTypeDef TIM_OCStruct;
	
	/* Common settings */
	
	/* PWM mode 2 = Clear on compare match */
	/* PWM mode 1 = Set on compare match */
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	
/*
	To get proper duty cycle, you have simple equation
	
	pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
	
	where DutyCycle is in percent, between 0 and 100%
	
	25% duty cycle: 	pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
	50% duty cycle: 	pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
	75% duty cycle: 	pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
	100% duty cycle:	pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399
	
	Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
*/
	TIM_OCStruct.TIM_Pulse = 2099; /* 25% duty cycle */
	TIM_OC1Init(TIM4, &TIM_OCStruct);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_OCStruct.TIM_Pulse = 4199; /* 50% duty cycle */
	TIM_OC2Init(TIM4, &TIM_OCStruct);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_OCStruct.TIM_Pulse = 6299; /* 75% duty cycle */
	TIM_OC3Init(TIM4, &TIM_OCStruct);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_OCStruct.TIM_Pulse = 8399; /* 100% duty cycle */
	TIM_OC4Init(TIM4, &TIM_OCStruct);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
