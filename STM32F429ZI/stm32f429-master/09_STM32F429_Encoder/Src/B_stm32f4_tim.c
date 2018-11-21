#include "B_stm32f4_tim.h"
#include "tm_stm32f4_delay.h"
#include "B_stm32f4_usart.h"

/* Encoder variables */
__IO int32_t Num_Encoder_1;
void TIM1_INIT(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	/*Enable clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	 
	/*configure PWM Pin out TIM1 CH2-CH2N*/
	GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); 
	// Create 1kHz PWM
	// TIM4 is connected to APB1 bus that have default clock 84MHz
	// So, the frequency of TIM4 is 84MHz
	// We use prescaler 10 here
	// So, the frequency of TIM4 now is 8.4MHz
	// TIM_Period determine the PWM frequency by this equation:
	// PWM_frequency = timer_clock / (TIM_Period + 1)
	// If we want 1kHz PWM we can calculate:
	// TIM_Period = (timer_clock / PWM_frequency) - 1
	// TIM_Period = (8.4MHz / 1kHz) - 1 = 8399
	
	/*configure Freqency for PWM by: 84MHZ/(Period+1)/(Prescaler+1)*/
	TIM_TimeBaseInitStructure.TIM_Prescaler= 9;
	TIM_TimeBaseInitStructure.TIM_Period =8399;// gia tri cua thanh ghi ARR dem de duoc tan so pwm là môt khz 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0x0;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	//Preloadconfig toan bo timer
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	/*Enable PWM_TIM*/
  TIM_Cmd(TIM1, ENABLE);
	/* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
}

void TIM1_PWM(__IO uint32_t Pulse )
{
	
	
	/*Configure PWM*/
  TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//choose mode pwm1 oc: output compare
	/*Chon muc cao cho dau ra, neu chon muc thap thi no se bi dao nguoc hinh dang dau ra, chu ky tan so deu nhu nhau*/
	TIM_OCInitStructure.TIM_OutputState	=	TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity	=	TIM_OCPolarity_High;
	// Duty cycle calculation equation:
	// TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
	// Ex. 25% duty cycle:
	// TIM_Pulse = (((8399 + 1) * 25) / 100) - 1 = 2099// gia tri thanh ghi ARR dem tu 0-2099 pwm muc cao va tu 2099 - 8399 PWM muc thap 
	// TIM_Pulse = (((8399 + 1) * 75) / 100) - 1 = 6299
	TIM_OCInitStructure.TIM_Pulse=Pulse;
	TIM_OCInitStructure.TIM_OCIdleState	=	TIM_OCIdleState_Reset;
	
	/*PWM Channel 1*/
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	//Enables or disables the TIMx peripheral Preload register on CCR1.
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable); 
	//Selects the TIM Output Compare Mode
  TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
	//Enables or disables the TIM Capture Compare Channel x
  TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);

	//Enables or disables TIMx peripheral Preload register on ARR
	TIM_ARRPreloadConfig(TIM1, ENABLE);//preloadconfig all timer
	////Enable clock for GPOI
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//Initialize struct
	GPIO_InitTypeDef GPIO_InitDef;
	 
	//Pins 12 and 14
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
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
	GPIO_SetBits(GPIOA,GPIO_Pin_14);

}

//use timer 3 count pulse 
void encoder1(void)
{
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef					NVIC_InitStructure;
	
	//enable colck for timers
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//Init pin n  
	B_GPIO_Init(GPIOA,GPIO_Pin_6 | GPIO_Pin_7,RCC_AHB1Periph_GPIOA,GPIO_Mode_AF);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
	
	/* Configure Timer for Encoder Interface 1*/		
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;												//Specifies the prescaler value used to divide the TIM clock.
	TIM_TimeBaseInitStructure.TIM_Period=0xFFFF; 											// dem toi gia tri 
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//chon che do dem len
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				//Specifies the clock division
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	/*Configure Encoder Interface 1 capture channel*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;	//lua chon channel
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;				//xac dinh che do hoat dong canh cho tin hieu dau vao
	TIM_ICInitStructure.TIM_ICFilter=0; 														//chi dinh bo loc dau vao
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;		//lua chon dau vao truc tiep
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	/*Configure Encoder Interface 1*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	/*Reset Counter 1*/
	TIM_SetCounter(TIM3, 0);	
	
//	/*Clear Interrupt Update flag 1*/
//	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
//	
//	/*Enable Update Interrupt of 2ng TIMER*/
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	
//	/* 2 bit for pre-emption priority, 2 bits for subpriority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
//	
//	/*Configure NVIC for Encoder Interface Update Interrupt 1*/
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
//	NVIC_Init(&NVIC_InitStructure);
//	Num_Encoder_1=0;
	
	/*Enable 1nd Timer */
	TIM_Cmd(TIM3,ENABLE);	
}

//use timer4 counted
void encoder2()
{
	TIM_ICInitTypeDef 				TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
//NVIC_InitTypeDef					NVIC_InitStructure;
	
	//enable colck for timers
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//khoi tao phin noi 
	B_GPIO_Init(GPIOB,GPIO_Pin_6 | GPIO_Pin_7,RCC_AHB1Periph_GPIOB,GPIO_Mode_AF);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	
	/* Configure Timer for Encoder Interface 1*/		
	TIM_TimeBaseInitStructure.TIM_Prescaler			=	0;												//Specifies the prescaler value used to divide the TIM clock.
	TIM_TimeBaseInitStructure.TIM_Period				=	0xFFFF; 											// dem toi gia tri 
	TIM_TimeBaseInitStructure.TIM_CounterMode 	= TIM_CounterMode_Up;		//chon che do dem len
	TIM_TimeBaseInitStructure.TIM_ClockDivision	=	TIM_CKD_DIV1; 				//Specifies the clock division
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	/*Configure Encoder Interface 1 capture channel*/
	TIM_ICInitStructure.TIM_Channel			=	TIM_Channel_1 | TIM_Channel_2;	//lua chon channel
	TIM_ICInitStructure.TIM_ICPolarity	=	TIM_ICPolarity_Rising;				//xac dinh che do hoat dong canh cho tin hieu dau vao
	TIM_ICInitStructure.TIM_ICFilter		=	0; 														//chi dinh bo loc dau vao
	TIM_ICInitStructure.TIM_ICSelection	=	TIM_ICSelection_DirectTI;		//lua chon dau vao truc tiep
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	/*Configure Encoder Interface 1*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	/*Reset Counter 1*/
	TIM_SetCounter(TIM4, 0);	
	
//	/*Clear Interrupt Update flag 1*/
//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
//	/*Enable Update Interrupt of 2ng TIMER*/
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//	
//	/* 2 bit for pre-emption priority, 2 bits for subpriority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	/*Configure NVIC for Encoder Interface Update Interrupt 1*/
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
//	NVIC_Init(&NVIC_InitStructure);
//	Num_Encoder_1=0;
	
	/*Enable 1nd Timer */
	TIM_Cmd(TIM4,ENABLE);	
}


//Timer 2 compute rmp

void TIM2_TIME(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
  TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef					NVIC_InitStructure;
	
	// TIM4 configuration 
	TIM_TimeBaseInitStructure.TIM_Period = 299; // delay 100ms/999  300ms/9     
	TIM_TimeBaseInitStructure.TIM_Prescaler= (8399);	 //10khz// do no dem tu 0
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	// Clear TIM2 update pending flags 
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	//Enable TIM2 Update interrupts 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	// Enable the TIM4 global Interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//TIM enable counter 
	TIM_Cmd(TIM2, ENABLE);	
	
}

