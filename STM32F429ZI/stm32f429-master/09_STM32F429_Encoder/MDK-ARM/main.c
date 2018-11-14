#include <stdio.h>
#include"stm32f4xx.h"
/*PIN
PB6 CH1
PB7 CH2
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

__IO int32_t count_temp1=0,count_temp2=0;

int main()
{
  char data_Buffer[50];
  char data_Buffer1[50];
	//xai xung clock ngoai
	Clock_HSE();
	/* Initialize Encoder */
  encoder1();
	encoder2();
	USART1_Init();
	TIM1_PWM(2999);
	while (1) {

		count_temp1 = (int32_t)TIM3->CNT;
		if(count_temp1 > 32767)
			count_temp1 -= 65535;
		else if(count_temp1 < -32767)
			count_temp1 += 65535;
		
		sprintf(data_Buffer, "count_temp1: %d\r\n",count_temp1);//338
		USART_PutString(data_Buffer);
		Delay_ms(1000);
		
		count_temp2 = (int32_t)TIM4->CNT;
		if(count_temp2 > 32767)
			count_temp2 -= 65535;
		else if(count_temp1 < -32767)
			count_temp2 += 65535;
		
		sprintf(data_Buffer1, "count_temp2: %d\r\n",count_temp2);//338
		USART_PutString(data_Buffer1);
		Delay_ms(1000);
		//control motor
//		GPIO_ToggleBits(GPIOA,GPIO_Pin_12);
//		Delay_ms(100);
//		GPIO_ToggleBits(GPIOA,GPIO_Pin_14  );
//		Delay_ms(1000);
		
	}
}
