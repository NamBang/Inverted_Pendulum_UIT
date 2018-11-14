#include "B_stm32f4_gpio.h"

void Clock_HSE(void)
{
	 // Resets the clock configuration to the default reset state
    RCC_DeInit();

    // Enable external crystal (HSE)
    RCC_HSEConfig(RCC_HSE_ON);
    // Wait until HSE ready to use or not
    ErrorStatus errorStatus = RCC_WaitForHSEStartUp();

    if (errorStatus == SUCCESS)
    {
        // Configure the PLL for 168MHz SysClk and 48MHz for USB OTG, SDIO
        RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
        // Enable PLL
        RCC_PLLCmd(ENABLE);
        // Wait until main PLL clock ready
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

        // Set flash latency
        FLASH_SetLatency(FLASH_Latency_5);

        // AHB 168MHz
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        // APB1 42MHz
        RCC_PCLK1Config(RCC_HCLK_Div4);
        // APB2 84 MHz
        RCC_PCLK2Config(RCC_HCLK_Div2);

        // Set SysClk using PLL
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    }
    else
    {
        // Do something to indicate that error clock configuration
        while (1);
    }

    SystemCoreClockUpdate();
}

void Config_Button_Led(void)
{
		// /******************* User button**********************/
	GPIO_InitTypeDef GPIO_InitDef;
	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	//Pin 0
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	//Mode output
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	//Output type Push pull
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	//with pull dow resistor
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	//50MHZ pin speed
	GPIO_InitDef.GPIO_Speed =GPIO_Speed_50MHz;
	//Initialize pin on GPIOA port
	GPIO_Init(GPIOA,&GPIO_InitDef);
	//Returs Pin state(1 if HIGH, o if LOW). Read value on pin
	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
}

//Thêm câu lệnh này trước lúc khởi tạo 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
void B_GPIO_Init(GPIO_TypeDef* GPIOx,uint16_t B_GPIO_Pin,uint32_t RCC_AHBxPeriph_GPIOx,uint16_t GPIO_Modex )
{	
	GPIO_InitTypeDef GPIO_InitDef;
	//Enable clock for GPOIG
	RCC_AHB1PeriphClockCmd(RCC_AHBxPeriph_GPIOx , ENABLE);
	//pin
	GPIO_InitDef.GPIO_Pin = B_GPIO_Pin;
	//Mode output 
	GPIO_InitDef.GPIO_Mode = GPIO_Modex;
	//ouput type push-pull
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	//without pull resistor
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL
	//50MHZ pin speed
	GPIO_InitDef.GPIO_Speed =GPIO_Speed_50MHz;
	//Initialze pín on GPIOG p?t
	GPIO_Init(GPIOx,&GPIO_InitDef);
}
void B_GPIO_InitAlternate(GPIO_TypeDef* GPIOx,uint16_t B_GPIO_Pin,uint32_t RCC_AHB1Periph_GPIOx,uint8_t Alternate)
{
	uint32_t pinpos;

	/* Check input */
	if (B_GPIO_Pin == 0x00) {
		return;
	}
	//Enable clock for GPOIx
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx , ENABLE);
	
	/* Set alternate functions for all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check pin */
		if ((B_GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}
		
		/* Set alternate function */
		GPIOx->AFR[pinpos >> 0x03] = (GPIOx->AFR[pinpos >> 0x03] & ~(0x0F << (4 * (pinpos & 0x07)))) | (Alternate << (4 * (pinpos & 0x07)));
	}
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = B_GPIO_Pin;//change GPIO at "B_stm32f4_usart.h"
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}
