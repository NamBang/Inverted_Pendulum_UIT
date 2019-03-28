/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include <stdio.h>
#include "string.h"
#include "pid_controller.h"
#include "main.h"
#include "uart.h"
#include "timer.h"
#include "gpio.h"
#include "usb.h"

#define LPF_Beta 0.25
#define kp 900
#define ki 220
#define kd 90

#define e_kp 10
#define e_ki 1.5
#define e_kd 0

#define peak_pos 1200
#define peak_neg 64335
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
extern char USBDISKPath[4];
extern USBH_HandleTypeDef hUSBHost;
extern MSC_ApplicationTypeDef Appli_state;

char buffer1[20],buffer2[20];
char start_program[]= "Start program";
char Start_Write_USB[]="Start write USB";
char Close_USB[] = "Close_USB";
uint32_t count1=0,count2,temp,count_time=0,speed_percent;
__IO bool flag=FALSE;
uint32_t duty=0;
void speed(int32_t sp);
void swing_up();
void control_balance(int8_t delta_v);
void balance_right();
void balance_left();

void balance_test(uint32_t encoder);
/*khao bao bien*/
__IO int32_t count_recent1=0,count_update1=0,PPS_motor,output_encoder;
__IO  float PSP=0.0,PPS;
__IO float PSP_Update=0.0,PSP_encoder_Update=0.0;
PIDControl pid,e_pid;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
		HAL_UART_Transmit_IT(&huart1,(uint8_t *)start_program,strlen(start_program));
    HAL_Init();
    /* Configure LED3 and LED4 */
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    /* Configure the system clock to 168 MHz */
    SystemClock_Config();
    /* Initialize all configured peripherals */
    B_GPIO_Init();
    B_TIM3_Init();
    B_TIM4_Init();
    B_USART1_UART_Init();
    B_DMA_Init();
    B_TIM2_Init();
    /* Initialize interrupts */
    B_NVIC_Init_Tim2();
    B_TIM1_Init();
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT(&htim2);
    //TIM1->CCR1 = 8399;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
		speed(0);
		//HAL_Delay(1000);
		//last_speed=100;
		
		PIDInit(&pid,kp,ki,kd,0.01,0,8399,AUTOMATIC,DIRECT);
		PIDInit(&e_pid,e_kp,e_ki,e_kd,0.01,-100,100,AUTOMATIC,DIRECT);
		//speed(100);
		
    //PID_Init(&pid,20.0,100.0,5,1);
    /*##-1- Link the USB Host disk I/O driver ##################################*/
    if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
				//	HAL_UART_Transmit_IT(&huart1,(uint8_t *)Start_Read_USB,strlen(Start_Read_USB));
        /*##-2- Init Host Library ################################################*/
        USBH_Init(&hUSBHost, USBH_UserProcess, 0);
        /*##-3- Add Supported Class ##############################################*/
        USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
        /*##-4- Start Host Process ###############################################*/
        USBH_Start(&hUSBHost);
        /*##-5- Run Application (Blocking mode) ##################################*/
        while (1)
        {	
					//if((TIM4->CNT>1190 && TIM4->CNT <1216)||(TIM4->CNT>64326 && TIM4->CNT <64350))
//					if(TIM4->CNT>=1190 && TIM4->CNT<1195)
//					{
//						balance_test(1205);
//					}else if(TIM4->CNT>=1195 && TIM4->CNT<1200)
//					{
//						balance_test(1203);
//					}else if(TIM4->CNT>1200 && TIM4->CNT<=1205)
//					{
//						balance_test(1993);
//					}else if(TIM4->CNT>=1205 && TIM4->CNT<1210)
//					{
//						balance_test(1995);
//					}
					if((TIM4->CNT >1190 && TIM4->CNT <1210)||(TIM4->CNT >64325 && TIM4->CNT <64345))
						{
						if(TIM4->CNT >0xaaaa)
							PIDSetpointSet(&e_pid,peak_pos);
						else
							PIDSetpointSet(&e_pid,peak_neg);
						/*if(PPS_motor<0)
						{
							balance_right();
							temp=1;
						}
						else if(PPS_motor >0)
						{
							balance_left();
							temp=2;
						}*/
					}//else swing_up();
						
					//sprintf(buffer1,"%d;%d\r\n",PPS_motor,TIM4->CNT);
					//HAL_Delay(100);

					//HAL_UART_Transmit_IT(&huart1,(uint8_t *)buffer1,strlen(buffer1));
            /* USB Host Background task */
            USBH_Process(&hUSBHost);
            //	MSC_Application();
						//swing_up();
            /* Mass Storage Application State Machine */
					//sprintf(buffer1,"%f;%d\r\n",PPS,TIM4->CNT);
					
					//HAL_UART_Transmit_IT(&huart1,(uint8_t *)buffer1,strlen(buffer1));
					//HAL_Delay(10);
            switch(Appli_state)
            {
            case APPLICATION_START:
                MSC_Application();
                Appli_state = APPLICATION_IDLE;
                break;

            case APPLICATION_IDLE:
            default:
                break;
            }
        }
    }
		
		
		

}

void speed(int32_t sp)
{
	if(sp>100)
		sp=100;
	if(sp <-100)
		sp=-100;
	if(sp>0)
	{
		//PID_Init(&pid,sp/4,kp,ki,kd);
		PIDSetpointSet(&pid,sp/4);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_SET);
	}else{
		//PID_Init(&pid,-sp/4,kp,ki,kd);
		PIDSetpointSet(&pid,-sp/4);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	}
}

void swing_up()
{
	if(PPS<0){
		speed(-100);
	//sprintf(buffer1,"%d\t%d\t%d\n",PPS_motor,-100,count_time);
	}
	else{ speed(100);
	//sprintf(buffer1,"%d\t%d\t%d\n",PPS_motor,100,count_time);
	}
}


void balance_left()
{
		switch (TIM4->CNT)
		{
			case 1222:
			case 1223:
			case 64348:
			case 64347:
				speed(-100);
				break;
			case 1221:
			case 1220:
			case 64346:
			case 64345:
				speed(-90);
				break;
			case 1219:
			case 1218:
			case 64344:
			case 64343:
				speed(-80);
				break;
			case 1217:
			case 1216:
			case 64342:
			case 64341:
				speed(-70);
				break;
			case 1215:
			case 1214:
			case 64340:
			case 64339:
				speed(-60);
				break;
			case 1213:
			case 1212:
			case 64338:
			case 64337:
				speed(-50);
				break;
			case 1211:
			case 1210:
			case 64336:
			case 64335:
				speed(-50);
				break;
			case 1209:
			case 1208:
			case 64334:
			case 64333:
				speed(0);
				break;
			case 1207:
			case 1206:
			case 64332:
			case 64331:
				speed(50);
				break;
			case 1205:
			case 1204:
			case 64330:
			case 64329:
				speed(60);
				break;
			case 1203:
			case 1202:
			case 64328:
			case 64327:
				speed(70);
				break;
			case 1201:
			case 1200:
			case 1199:
			case 64326:
			case 64325:
			case 64324:
				//speed(0);
				//HAL_Delay(2);
				speed(80);
				break;
			case 1198:
			case 64323:
				speed(90);
				break;
			case 1197:
			case 1196:
			case 64322:
			case 64321:
				speed(100);
				break;
//			case 1195:
//			case 1194:
//			case 64330:
//			case 64329:
//				speed(95);
//				break;
//			case 1193:
//			case 1192:
//			case 64328:
//			case 64327:
//				speed(100);
//				break;
		}
}

void balance_right()
{
		switch (TIM4->CNT)
		{
			case 1219:
			case 1218:
			case 64342:
			case 64341:
				speed(-100);
				break;
			case 1217:
			case 1216:
			case 64340:
			case 64339:
				speed(-100);
				break;
			case 1215:
			case 1214:
			case 64338:
			case 64337:
				speed(-90);
				break;
			case 1213:
			case 1212:
			case 64336:
			case 64335:
				speed(-80);
				break;
			case 1211:
			case 1210:
			case 64334:
			case 64333:
				speed(-70);
				break;
			case 1209:
			case 1208:
			case 64332:
			case 64331:
				speed(-70);
				break;
			case 1207:
			case 1206:
			case 64330:
			case 64329:
				speed(-60);
				break;
			case 1205:
			case 1204:
			case 64328:
			case 64327:
				speed(-50);
				break;
			case 1203:
			case 1202:
			case 64326:
			case 64325:
				speed(0);
				break;
			case 1201:
			case 1200:
			case 1199:
			case 64324:
			case 64323:
			case 64322:
				speed(50);
				break;
			case 1198:
			case 64321:
				speed(60);
				break;
			case 1197:
			case 1196:
			case 64320:
			case 64319:
				speed(70);
				break;
			case 1195:
			case 1194:
			case 64318:
			case 64317:
				speed(80);
				break;
			case 1193:
			case 1192:
			case 64316:
			case 64315:
				speed(90);
				break;
		}
}

void control_balance(int8_t delta_v)
{
			if(delta_v<0){
				if(PPS_motor>0)
					speed(PPS_motor*3-delta_v);
				else 
					speed(PPS_motor*3+delta_v);
			}else
			{
				if(PPS_motor>0)
					speed(PPS_motor*3+delta_v);
				else 
					speed(PPS_motor*3-delta_v);
			}
			//sprintf(buffer1,"%d;%d\n",delta_v,PPS_motor);
			//HAL_UART_Transmit_IT(&huart1,(uint8_t *)buffer1,strlen(buffer1));
}

/* function user define*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim2.Instance)
    {
        //BSP_LED_Toggle(LED4);
			count_recent1 = temp+TIM4->CNT;
			if(count_recent1 < 0xAAAA && count_update1 < 0xAAAA)
				PPS=(int32_t)(count_recent1 - count_update1);
			if(count_recent1 > 0xAAAA&& count_update1 > 0xAAAA)
				PPS=(int32_t)(count_recent1 - count_update1);
			if(TIM3->CNT<0xAAAA)
				PPS_motor = TIM3->CNT;
			else PPS_motor=-(0xFFFF-TIM3->CNT);
			count1=0;

		/*compute PID*/
		PPS_motor = (float)(PSP_Update-(float)LPF_Beta*(PSP_Update-PPS_motor));//lowpass filter
		if (PPS_motor < 0) PSP = -PPS_motor ;
			else PSP = PPS_motor ;
		PIDInputSet(&pid,PSP);
		PIDCompute(&pid);
		duty=(uint32_t)PIDOutputGet(&pid);
		
		if((TIM4->CNT >1190 && TIM4->CNT <1210)||(TIM4->CNT >64325 && TIM4->CNT <64345))
		{
			PIDInputSet(&e_pid,TIM4->CNT);
			PIDCompute(&e_pid);
			speed((uint32_t)PIDOutputGet(&e_pid));
			sprintf(buffer1,"%d\n",(uint32_t)PIDOutputGet(&e_pid));
			HAL_UART_Transmit_IT(&huart1,(uint8_t *)buffer1,strlen(buffer1));
		}
//		PPS = (float)(PSP_encoder_Update-(float)LPF_Beta*(PSP_encoder_Update-PPS));//lowpass filter
//		if (PPS < 0) PSP = -PPS ;
//			else PSP = PPS ;
//		PIDInputSet(&pid,PSP);
//		PIDCompute(&pid);
//		output_encoder=(uint32_t)PIDOutputGet(&pid);
		
			/*PWM*/
		TIM1->CCR1 = duty;
		//sprintf(buffer1,"%d;%d\n",duty,PPS_motor);
		PSP_Update=PPS_motor;
		PSP_encoder_Update=PPS;
		count_update1 =  temp+TIM4->CNT;
		TIM3->CNT=0;
		count_time++;
        //HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_13);
//		__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    }
    if(htim->Instance == htim3.Instance)
    {
			if(TIM3->CR1 & 0x10) // check direction
			{
				temp -= 0xFFFF;
			}
			else
			{
				temp += 0xFFFF;
			}
        //HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_13);
//		__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    }
    if(htim->Instance == htim4.Instance)
    {
        //HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_13);
//		__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		flag = !flag;
	BSP_LED_Toggle(LED3);
	/*doi khi nao nha nut*/
	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0));
//		if(flag)
//		{
//			speed(100);
//		}else speed(-100);
	
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* Turn LED4 on */
    BSP_LED_On(LED4);

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
