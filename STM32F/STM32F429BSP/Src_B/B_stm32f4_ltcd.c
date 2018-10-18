#include "B_stm32f4_ltcd.h"
#define ILI9341_CS_SET				B_GPIO_SetPinHigh(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_FRAME_BUFFER		SDRAM_START_ADR
#define ILI9341_WIDTH 				240
#define ILI9341_HEIGHT				320
typedef enum {
	TM_ILI9341_Landscape,
	TM_ILI9341_Portrait
} TM_ILI9341_Orientation;

typedef struct {
	uint16_t Width;
	uint16_t Height;
	TM_ILI9341_Orientation Orientation; // 1 = portrait; 0 = landscape
	uint8_t CurrentLayer;
	uint32_t CurrentLayerOffset;
	uint8_t Layer1Opacity;
	uint8_t Layer2Opacity;
	TM_ILI9341_Orientation_t Orient;
} TM_ILI931_Options_t;


/* Private variables */
uint16_t ILI9341_x;
uint16_t ILI9341_y;
TM_ILI931_Options_t ILI9341_Opts;
/* Offset for Layer 2 */
#define ILI9341_FRAME_OFFSET		(uint32_t)ILI9341_PIXEL * 2
#define ILI9341_PIXEL				76800
static void B_SDRAM_InitPins(void);
static void TM_ILI9341_SendCommand(uint8_t data);
static void TM_ILI9341_SendData(uint8_t data);
static void TM_ILI9341_Delay(volatile unsigned int delay);
static __INLINE uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data);
static void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t *font, uint32_t foreground, uint32_t background);
static void TM_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color);
void LCD_Config(void)
{
	 LTDC_InitTypeDef LTDC_InitStruct;
	 LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
/* Enable Pizel Clock --------------------------------------*/
	//PLL_M default =16
	/*Configure PLLSAI prescalers for LCD */
	/*PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1Mhz*/
	/*PLLSAI_VCO Output= PLLSAI_VCO Input*PLLSAI_N = 192Mhz*/
	/*PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R=192/3 =64Mhz*/
	/*LTCD clock frequency =PLLLCDCLK / RCC_PLLSAIDivR=63/8=8 mhz*/
	RCC_PLLSAIConfig(192,7,3);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
	/*Enable PLLSAI Clock*/
	RCC_PLLSAICmd(ENABLE);
	/*Wait for PLLSAI activation*/
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY == RESET))
	{
		
	}
	
	/*Configure the LCD control pins----------------------------*/
	LCD_AF_GPIOConfig();
	
	/* LTDC Initialization -------------------------------------------------------*/

  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
  /* Initialize the vertical synchronization polarity as active low */  
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
  /* Initialize the data enable polarity as active low */ 
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
  /* Initialize the pixel clock polarity as input pixel clock */ 
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
  
  /* Timing configuration */
  /* Horizontal synchronization width = Hsync - 1 */     
  LTDC_InitStruct.LTDC_HorizontalSync = 40;
  /* Vertical synchronization height = Vsync - 1 */
  LTDC_InitStruct.LTDC_VerticalSync = 9;
  /* Accumulated horizontal back porch = Hsync + HBP - 1 */
  LTDC_InitStruct.LTDC_AccumulatedHBP = 42; 
  /* Accumulated vertical back porch = Vsync + VBP - 1 */
  LTDC_InitStruct.LTDC_AccumulatedVBP = 11;  
  /* Accumulated active width = Hsync + HBP + Active Width - 1 */  
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 522;
  /* Accumulated active height = Vsync + VBP + Active Heigh - 1 */
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 283;
  /* Total width = Hsync + HBP + Active Width + HFP - 1 */
  LTDC_InitStruct.LTDC_TotalWidth = 524; 
  /* Total height = Vsync + VBP + Active Heigh + VFP - 1 */
  LTDC_InitStruct.LTDC_TotalHeigh = 285;
 
  /* Configure R,G,B component values for LCD background color */                   
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0; 
  
  /* Initialize LTDC */          
  LTDC_Init(&LTDC_InitStruct);
  
/* LTDC initialization end ---------------------------------------------------*/

/* Layer1 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  /* In this case all the active display area is used to display a picture then :
     Horizontal start = horizontal synchronization + Horizontal back porch = 43 
     Vertical start   = vertical synchronization + vertical back porch     = 12
     Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1 
     Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */ 
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 43;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (480 + 43 - 1); 
  LTDC_Layer_InitStruct.LTDC_VerticalStart = 12;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (272 + 12 - 1);
  
  /* Pixel Format configuration*/           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  
  /* Alpha constant (255 totally opaque) */
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
  
  /* Default Color configuration (configure A,R,G,B component values) */          
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;   

  /* Configure blending factors */       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
   
  /* Start Address configuration : frame buffer is located at FLASH memory */    
//  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)&RGB565_480x272;
  
  /* the length of one line of pixels in bytes + 3 then :
     Line Lenth = Active high width x number of bytes per pixel + 3 
     Active high width         = 480 
     number of bytes per pixel = 2    (pixel_format : RGB565) */
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((480 * 2) + 3);
  
  /*  the pitch is the increment from the start of one line of pixels to the 
      start of the next line in bytes, then :
      Pitch = Active high width x number of bytes per pixel     */
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (480 * 2);  
  
  /* Configure the number of lines */
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 272;
  
  /* Initializes the Layer */ 
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
  
/* Layer1 Configuration end --------------------------------------------------*/
	
}

void LCD_AF_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOJ | \
                         RCC_AHB1Periph_GPIOK,  ENABLE);
	/* GPIOs Configuration */
/*
 +------------------------+-----------------------+----------------------------+
 +                       LCD pins assignment                                   +
 +------------------------+-----------------------+----------------------------+
 |  LCD_TFT R0 <-> PI.15  |  LCD_TFT G0 <-> PJ.07 |  LCD_TFT B0 <-> PJ.12      |
 |  LCD_TFT R1 <-> PJ.00  |  LCD_TFT G1 <-> PJ.08 |  LCD_TFT B1 <-> PJ.13      |
 |  LCD_TFT R2 <-> PJ.01  |  LCD_TFT G2 <-> PJ.09 |  LCD_TFT B2 <-> PJ.14      |
 |  LCD_TFT R3 <-> PJ.02  |  LCD_TFT G3 <-> PJ.10 |  LCD_TFT B3 <-> PJ.15      |
 |  LCD_TFT R4 <-> PJ.03  |  LCD_TFT G4 <-> PJ.11 |  LCD_TFT B4 <-> PK.03      |
 |  LCD_TFT R5 <-> PJ.04  |  LCD_TFT G5 <-> PK.00 |  LCD_TFT B5 <-> PK.04      |
 |  LCD_TFT R6 <-> PJ.05  |  LCD_TFT G6 <-> PK.01 |  LCD_TFT B6 <-> PK.05      |
 |  LCD_TFT R7 <-> PJ.06  |  LCD_TFT G7 <-> PK.02 |  LCD_TFT B7 <-> PK.06      |
 -------------------------------------------------------------------------------
          |  LCD_TFT HSYNC <-> PI.12  | LCDTFT VSYNC <->  PI.13 |
          |  LCD_TFT CLK   <-> PI.14  | LCD_TFT DE   <->  PK.07 |
           -----------------------------------------------------         
*/

 /* GPIOI configuration */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource15, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStruct);
  
 /* GPIOJ configuration */  
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource2, GPIO_AF_LTDC); 
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource8, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource15, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                             GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                             GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                             GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  
  GPIO_Init(GPIOJ, &GPIO_InitStruct);

 /* GPIOI configuration */
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource7, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                             GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOK, &GPIO_InitStruct); 
	
}



void ILI9341_InitPins()
{
	/* Init WRX pin */
	B_GPIO_Init(ILI9341_WRX_PORT,ILI9341_WRX_PIN,RCC_AHB1Periph_GPIOA,GPIO_Mode_OUT);
	/* Init CS pin */
	B_GPIO_Init(ILI9341_CS_PORT,ILI9341_CS_PIN,RCC_AHB1Periph_GPIOA,GPIO_Mode_OUT);
		/* GPIOA  */
	B_GPIO_InitAlternate(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_12,RCC_AHB1Periph_GPIOA,GPIO_AF_LTDC);
	/* GPIOB */
	B_GPIO_InitAlternate(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12, RCC_AHB1Periph_GPIOB,0x09);
	/* GPIOB */
	B_GPIO_InitAlternate(GPIOB, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11,RCC_AHB1Periph_GPIOB,GPIO_AF_LTDC);
	/* GPIOC */
	B_GPIO_InitAlternate(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10,RCC_AHB1Periph_GPIOC , GPIO_AF_LTDC);
	/* GPIOD */
	B_GPIO_InitAlternate(GPIOD, GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_10,RCC_AHB1Periph_GPIOD , GPIO_AF_LTDC);
	/* GPIOF  */
	B_GPIO_InitAlternate(GPIOF, GPIO_Pin_10,RCC_AHB1Periph_GPIOF , GPIO_AF_LTDC);
	/* GPIOG  */
	B_GPIO_InitAlternate(GPIOG, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_11, RCC_AHB1Periph_GPIOG, GPIO_AF_LTDC);
	/* GPIOG  */                  
	B_GPIO_InitAlternate(GPIOG, GPIO_Pin_10 | GPIO_Pin_12, RCC_AHB1Periph_GPIOG, 0x09);

	
}

void B_SPI_Init()
{
	B_GPIO_InitAlternate(GPIOF, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, RCC_AHB1Periph_GPIOF, GPIO_AF_SPI5);
	//enable clock SPI5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;		
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	
	SPI_Init(SPI5, &SPI_InitStruct);			
	SPI_Cmd(SPI5, ENABLE);
	
}

void ILI9341_Init(void) {
	/* Initialize pins used */
	ILI9341_InitPins();
	/* SPI chip select high */
	ILI9341_CS_SET;
	/* Init SPI5 and pinpack1 */
	B_SPI_Init();
	/* Init SDRAM */
	B_SDRAM_Init();
	/* Initialize LCD for LTDC */
	TM_ILI9341_InitLCD();
	/* Initialize LTDC */
	TM_LCD9341_InitLTDC();
	/* Initialize LTDC layers */
	TM_ILI9341_InitLayers();
//	/* Set cursor X and Y */
//	ILI9341_x = ILI9341_y = 0;
//	
//	/* Set default settings */
//	ILI9341_Opts.Width = ILI9341_WIDTH;
//	ILI9341_Opts.Height = ILI9341_HEIGHT;
//	ILI9341_Opts.Orientation = TM_ILI9341_Portrait;
//	ILI9341_Opts.Orient = TM_ILI9341_Orientation_Portrait_1;
//	ILI9341_Opts.CurrentLayer = 0;
//	ILI9341_Opts.CurrentLayerOffset = 0;
//	ILI9341_Opts.Layer1Opacity = 255;
//	ILI9341_Opts.Layer2Opacity = 0;
	
	/* Fill both layers with default colors */
//	TM_ILI9341_SetLayer1();
//	TM_ILI9341_Fill(ILI9341_COLOR_WHITE);
//	TM_ILI9341_SetLayer2();
//	TM_ILI9341_Fill(ILI9341_COLOR_WHITE);
//	TM_ILI9341_SetLayer1();
}



uint8_t B_SDRAM_Init(void) {
	FMC_SDRAMInitTypeDef FMC_SDRAMInitStructure;
	FMC_SDRAMTimingInitTypeDef FMC_SDRAMTimingInitStructure;
	FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
	volatile uint32_t timeout = SDRAM_TIMEOUT;
	static uint8_t initialized = 0;
	
	/* Already initialized */
	if (initialized) {
		return 1;
	}
	
		/* Initialize FMC pins */
	B_SDRAM_InitPins();
	
	/* Enable FMC clock */
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
	
	/* FMC SDRAM device initialization sequence --------------------------------*/ 
	/* Step 1 ----------------------------------------------------*/ 
	/* Timing configuration for 90 Mhz of SD clock frequency (180Mhz/2) */
	/* TMRD: 2 Clock cycles */
	/* 1 clock cycle = 1 / 90MHz = 11.1ns */
	FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay		= 2;      
	/* TXSR: min=70ns (7x11.10ns) */
	FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay	= 7;
	/* TRAS: min=42ns (4x11.10ns) max=120k (ns) */
	FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime		= 4;
	/* TRC:  min=70 (7x11.10ns) */        
	FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay			= 7;         
	/* TWR:  min=1+ 7ns (1+1x11.10ns) */
	FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime		= 2;      
	/* TRP:  20ns => 2x11.10ns */
	FMC_SDRAMTimingInitStructure.FMC_RPDelay				= 2;                
	/* TRCD: 20ns => 2x11.10ns */
	FMC_SDRAMTimingInitStructure.FMC_RCDDelay				= 2;
	
	
	/* FMC SDRAM control configuration */
	FMC_SDRAMInitStructure.FMC_Bank 						= FMC_Bank2_SDRAM;
	/* Row addressing: [7:0] */
	FMC_SDRAMInitStructure.FMC_ColumnBitsNumber 			= FMC_ColumnBits_Number_8b;
	/* Column addressing: [11:0] */
	FMC_SDRAMInitStructure.FMC_RowBitsNumber      			= FMC_RowBits_Number_12b;
	FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth  			= FMC_SDMemory_Width_16b;
	FMC_SDRAMInitStructure.FMC_InternalBankNumber 			= FMC_InternalBank_Number_4;
	/* CL: Cas Latency = 3 clock cycles */
	FMC_SDRAMInitStructure.FMC_CASLatency              	    = FMC_CAS_Latency_3;
	FMC_SDRAMInitStructure.FMC_WriteProtection 				= FMC_Write_Protection_Disable;
	FMC_SDRAMInitStructure.FMC_SDClockPeriod 				= FMC_SDClock_Period_2;
	FMC_SDRAMInitStructure.FMC_ReadBurst 					= FMC_Read_Burst_Disable;
	FMC_SDRAMInitStructure.FMC_ReadPipeDelay 				= FMC_ReadPipe_Delay_1;
	FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct 			= &FMC_SDRAMTimingInitStructure;
	
	/* FMC SDRAM bank initialization */
	FMC_SDRAMInit(&FMC_SDRAMInitStructure);
	
	/* SDRAM Init sequence */
	
	/* Configure a clock configuration enable command */
	FMC_SDRAMCommandStructure.FMC_CommandMode				= FMC_Command_Mode_CLK_Enabled;
	FMC_SDRAMCommandStructure.FMC_CommandTarget 			= FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber 		= 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Little delay */
	timeout = SDRAM_TIMEOUT * 0x110;
	while (timeout--);
	
	/* Configure a PALL (precharge all) command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode          		= FMC_Command_Mode_PALL;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          	= FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      	= 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Configure a Auto-Refresh command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode            	= FMC_Command_Mode_AutoRefresh;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          	= FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      	= 8;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= 0;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	
	/* Configure a load Mode register command */
	FMC_SDRAMCommandStructure.FMC_CommandMode            	= FMC_Command_Mode_LoadMode;
	FMC_SDRAMCommandStructure.FMC_CommandTarget          	= FMC_Command_Target_bank2;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      	= 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition 	= (uint32_t)0x0231;
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Set the refresh rate counter */
	/* (7.81 us x Freq) - 20 = (7.81 * 90MHz) - 20 = 683 */
	/* Set the device refresh counter */
	FMC_SetRefreshCount(680);
	
	/* Wait until the SDRAM controller is ready */
	timeout = SDRAM_TIMEOUT;
	while (FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET && timeout) {
		timeout--;
	}
	
	/* Check if everything goes right */
	/* Write 0x45 at location 0x50 and check if result is the same on read operation */
	B_SDRAM_Write8(0x50, 0x45);
	
	/* Read and check */
	if (B_SDRAM_Read8(0x50) == 0x45) {
		/* Initialized OK */
		initialized = 1;
		/* Initialized OK */
		return 1;
	}
	
	/* Not initialized OK */
	initialized = 0;
	
	/* Not ok */
	return 0;
	
}

static void B_SDRAM_InitPins(void) {
	/* Try to initialize from user */
	if (B_SDRAM_InitCustomPinsCallback()) {
		/* User has initialized pins by itself */
		return;
	}
#if defined(SDRAM_USE_STM324x9_EVAL)
	/* GPIOD pins */
	TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOE pins */
	TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOF pins */
	TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOG pins */
	TM_GPIO_InitAlternate(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOH pins */
	TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
	/* GPIOI pins */
	TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_FMC);
#else
	/* GPIOB pins */
	B_GPIO_InitAlternate(GPIOB, GPIO_Pin_5 | GPIO_Pin_6,RCC_AHB1Periph_GPIOB , GPIO_AF_FMC);
	/* GPIOC pins */
	B_GPIO_InitAlternate(GPIOC, GPIO_Pin_0, RCC_AHB1Periph_GPIOC, GPIO_AF_FMC);
	/* GPIOD pins */
	B_GPIO_InitAlternate(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15,RCC_AHB1Periph_GPIOD, GPIO_AF_FMC);
	/* GPIOE pins */
	B_GPIO_InitAlternate(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, RCC_AHB1Periph_GPIOE, GPIO_AF_FMC);
	/* GPIOF pins */
	B_GPIO_InitAlternate(GPIOF, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, RCC_AHB1Periph_GPIOF, GPIO_AF_FMC);
	/* GPIOG pins */
	B_GPIO_InitAlternate(GPIOG, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_15,RCC_AHB1Periph_GPIOG, GPIO_AF_FMC);
#endif
}

__weak uint8_t B_SDRAM_InitCustomPinsCallback(void) {
	/* If you need custom implementation, then create this function externally. */
	/* This function here should not be modified */
	
	/* Return 0, lib will use default pins */
	return 0;
}

void TM_ILI9341_InitLCD(void) {	
	TM_ILI9341_SendCommand(0xCA);
	TM_ILI9341_SendData(0xC3);
	TM_ILI9341_SendData(0x08);
	TM_ILI9341_SendData(0x50);
	TM_ILI9341_SendCommand(ILI9341_POWERB);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0xC1);
	TM_ILI9341_SendData(0x30);
	TM_ILI9341_SendCommand(ILI9341_POWER_SEQ);
	TM_ILI9341_SendData(0x64);
	TM_ILI9341_SendData(0x03);
	TM_ILI9341_SendData(0x12);
	TM_ILI9341_SendData(0x81);
	TM_ILI9341_SendCommand(ILI9341_DTCA);
	TM_ILI9341_SendData(0x85);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x78);
	TM_ILI9341_SendCommand(ILI9341_POWERA);
	TM_ILI9341_SendData(0x39);
	TM_ILI9341_SendData(0x2C);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x34);
	TM_ILI9341_SendData(0x02);
	TM_ILI9341_SendCommand(ILI9341_PRC);
	TM_ILI9341_SendData(0x20);
	TM_ILI9341_SendCommand(ILI9341_DTCB);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendCommand(ILI9341_FRC);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x1B);
	TM_ILI9341_SendCommand(ILI9341_DFC);
	TM_ILI9341_SendData(0x0A);
	TM_ILI9341_SendData(0xA2);
	TM_ILI9341_SendCommand(ILI9341_POWER1);
	TM_ILI9341_SendData(0x10);
	TM_ILI9341_SendCommand(ILI9341_POWER2);
	TM_ILI9341_SendData(0x10);
	TM_ILI9341_SendCommand(ILI9341_VCOM1);
	TM_ILI9341_SendData(0x45);
	TM_ILI9341_SendData(0x15);
	TM_ILI9341_SendCommand(ILI9341_VCOM2);
	TM_ILI9341_SendData(0x90);
	TM_ILI9341_SendCommand(ILI9341_MAC);
	TM_ILI9341_SendData(0xC8);
	TM_ILI9341_SendCommand(ILI9341_3GAMMA_EN);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendCommand(ILI9341_RGB_INTERFACE);
	TM_ILI9341_SendData(0xC2);
	TM_ILI9341_SendCommand(ILI9341_DFC);
	TM_ILI9341_SendData(0x0A);
	TM_ILI9341_SendData(0xA7);
	TM_ILI9341_SendData(0x27);
	TM_ILI9341_SendData(0x04);

	TM_ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0xEF);

	TM_ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x01);
	TM_ILI9341_SendData(0x3F);
	TM_ILI9341_SendCommand(ILI9341_INTERFACE);
	TM_ILI9341_SendData(0x01);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x06);

	TM_ILI9341_SendCommand(ILI9341_GRAM);
	TM_ILI9341_Delay(1000000);

	TM_ILI9341_SendCommand(ILI9341_GAMMA);
	TM_ILI9341_SendData(0x01);

	TM_ILI9341_SendCommand(ILI9341_PGAMMA);
	TM_ILI9341_SendData(0x0F);
	TM_ILI9341_SendData(0x29);
	TM_ILI9341_SendData(0x24);
	TM_ILI9341_SendData(0x0C);
	TM_ILI9341_SendData(0x0E);
	TM_ILI9341_SendData(0x09);
	TM_ILI9341_SendData(0x4E);
	TM_ILI9341_SendData(0x78);
	TM_ILI9341_SendData(0x3C);
	TM_ILI9341_SendData(0x09);
	TM_ILI9341_SendData(0x13);
	TM_ILI9341_SendData(0x05);
	TM_ILI9341_SendData(0x17);
	TM_ILI9341_SendData(0x11);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendCommand(ILI9341_NGAMMA);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x16);
	TM_ILI9341_SendData(0x1B);
	TM_ILI9341_SendData(0x04);
	TM_ILI9341_SendData(0x11);
	TM_ILI9341_SendData(0x07);
	TM_ILI9341_SendData(0x31);
	TM_ILI9341_SendData(0x33);
	TM_ILI9341_SendData(0x42);
	TM_ILI9341_SendData(0x05);
	TM_ILI9341_SendData(0x0C);
	TM_ILI9341_SendData(0x0A);
	TM_ILI9341_SendData(0x28);
	TM_ILI9341_SendData(0x2F);
	TM_ILI9341_SendData(0x0F);

	TM_ILI9341_SendCommand(ILI9341_SLEEP_OUT);
	TM_ILI9341_Delay(1000000);
	TM_ILI9341_SendCommand(ILI9341_DISPLAY_ON);

	TM_ILI9341_SendCommand(ILI9341_GRAM);
}
static void TM_ILI9341_SendCommand(uint8_t data) {
	ILI9341_WRX_RESET;
	ILI9341_CS_RESET;
	TM_SPI_Send(SPI5, data);
	ILI9341_CS_SET;
}

static void TM_ILI9341_SendData(uint8_t data) {
	ILI9341_WRX_SET;
	ILI9341_CS_RESET;
	TM_SPI_Send(SPI5, data);
	ILI9341_CS_SET;
}
static void TM_ILI9341_Delay(volatile unsigned int delay) {
	for (; delay != 0; delay--); 
}
/**
 * @brief  Sends single byte over SPI
 * @param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * @param  data: 8-bit data size to send over SPI
 * @retval Received byte from slave device
 */
static  uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED_RESP(SPIx, 0);
	
	/* Wait for previous transmissions to complete if DMA TX enabled for SPI */
	SPI_WAIT(SPIx);
	
	/* Fill output buffer with data */
	SPIx->DR = data;
	
	/* Wait for transmission to complete */
	SPI_WAIT(SPIx);
	
	/* Return data from buffer */
	return SPIx->DR;
}


void TM_LCD9341_InitLTDC(void) {
	LTDC_InitTypeDef LTDC_InitStruct;
	
	/* Enable the LTDC Clock */
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;

	/* Enable the DMA2D Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
	  
	/* Polarity configuration */
	/* Initialize the horizontal synchronization polarity as active low */
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
	/* Initialize the vertical synchronization polarity as active low */  
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
	/* Initialize the data enable polarity as active low */
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
	/* Initialize the pixel clock polarity as input pixel clock */ 
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

	/* Configure R,G,B component values for LCD background color */                   
	LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
	LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
	LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;  

	/* Configure PLLSAI prescalers for LCD */
	/* Enable Pixel Clock */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/4 = 96 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 96/4 = 24 Mhz */
	RCC_PLLSAIConfig(192, 7, 4);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);

	/* Enable PLLSAI Clock */
    RCC_PLLSAICmd(ENABLE);
	
    /* Wait for PLLSAI activation */
	while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET) {
	
	}
	
    /* Timing configuration */  
    /* Configure horizontal synchronization width */     
    LTDC_InitStruct.LTDC_HorizontalSync = 9;
    /* Configure vertical synchronization height */
    LTDC_InitStruct.LTDC_VerticalSync = 1;
    /* Configure accumulated horizontal back porch */
    LTDC_InitStruct.LTDC_AccumulatedHBP = 29; 
    /* Configure accumulated vertical back porch */
    LTDC_InitStruct.LTDC_AccumulatedVBP = 3;  
    /* Configure accumulated active width */  
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
    /* Configure accumulated active height */
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
    /* Configure total width */
    LTDC_InitStruct.LTDC_TotalWidth = 279; 
    /* Configure total height */
    LTDC_InitStruct.LTDC_TotalHeigh = 327;

	/* Init LTDC */
	LTDC_Init(&LTDC_InitStruct);
}

void TM_ILI9341_InitLayers(void) {
	LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

	/* 	Windowing configuration */
	/* 	Horizontal start = horizontal synchronization + Horizontal back porch = 43 
		Vertical start   = vertical synchronization + vertical back porch     = 12
		Horizontal stop = Horizontal start + LCD_PIXEL_WIDTH - 1 
		Vertical stop   = Vertical start + LCD_PIXEL_HEIGHT - 1        
	*/      
	LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = 269;
	LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
	LTDC_Layer_InitStruct.LTDC_VerticalStop = 323;

	/* Pixel Format configuration*/
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
    /* Alpha constant (255 totally opaque) */
    LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
    /* Default Color configuration (configure A,R,G,B component values) */          
    LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
    LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
    LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
    LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
	
    /* Configure blending factors */       
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

    /* the length of one line of pixels in bytes + 3 then :
    Line Lenth = Active high width x number of bytes per pixel + 3 
    Active high width         = LCD_PIXEL_WIDTH 
    number of bytes per pixel = 2    (pixel_format : RGB565) 
    */
	LTDC_Layer_InitStruct.LTDC_CFBLineLength = 240 * 2 + 3;
	
	/* the pitch is the increment from the start of one line of pixels to the 
    start of the next line in bytes, then :
    Pitch = Active high width x number of bytes per pixel     
    */ 
	LTDC_Layer_InitStruct.LTDC_CFBPitch = 240 * 2;
	/* Configure the number of lines */ 
	LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 320;
	
	/* Start Address configuration : the LCD Frame buffer is defined on SDRAM */   
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = ILI9341_FRAME_BUFFER;
	/* Initialize Layer 1 */
	LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

    /* Configure Layer2 */
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */  
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = ILI9341_FRAME_BUFFER + ILI9341_FRAME_OFFSET;
	
	/* Configure blending factors */ 
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
	/* Initialize Layer 2 */
	LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

	LTDC_ReloadConfig(LTDC_IMReload);
	
	/* Enable foreground & background Layers */
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);
	
	/* Reload immediate */
	LTDC_ReloadConfig(LTDC_IMReload);

	LTDC_DitherCmd(ENABLE);
	
	/* Display On */
	LTDC_Cmd(ENABLE);
	
	/* Set opacity */
	LTDC_LayerAlpha(LTDC_Layer1, 255);
	LTDC_LayerAlpha(LTDC_Layer2, 0);
	
	/* Immediate reload */
	LTDC_ReloadConfig(LTDC_IMReload);
}

void TM_ILI9341_Rotate(TM_ILI9341_Orientation_t orientation) {
	ILI9341_Opts.Orient = orientation;
	if (orientation == TM_ILI9341_Orientation_Portrait_1 || orientation == TM_ILI9341_Orientation_Portrait_2) {
		ILI9341_Opts.Width = ILI9341_WIDTH;
		ILI9341_Opts.Height = ILI9341_HEIGHT;
		ILI9341_Opts.Orientation = TM_ILI9341_Portrait;
	} else {
		ILI9341_Opts.Width = ILI9341_HEIGHT;
		ILI9341_Opts.Height = ILI9341_WIDTH;
		ILI9341_Opts.Orientation = TM_ILI9341_Landscape;
	}
}

void TM_ILI9341_SetLayer1(void) {
	ILI9341_Opts.CurrentLayerOffset = 0;
	ILI9341_Opts.CurrentLayer = 0;
}
void TM_ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, uint32_t foreground, uint32_t background) {
	uint16_t startX = x;
	
	/* Set X and Y coordinates */
	ILI9341_x = x;
	ILI9341_y = y;
	
	while (*str) {
		/* New line */
		if (*str == '\n') {
			ILI9341_y += font->FontHeight + 1;
			/* if after \n is also \r, than go to the left of the screen */
			if (*(str + 1) == '\r') {
				ILI9341_x = 0;
				str++;
			} else {
				ILI9341_x = startX;
			}
			str++;
			continue;
		} else if (*str == '\r') {
			str++;
			continue;
		}
		
		/* Put character */
		TM_ILI9341_Putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
	}
}

static void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t *font, uint32_t foreground, uint32_t background) {
	uint32_t i, b, j;
	/* Set coordinates */
	ILI9341_x = x;
	ILI9341_y = y;
	if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.Width) {
		/* If at the end of a line of display, go to new line and set x to 0 position */
		ILI9341_y += font->FontHeight;
		ILI9341_x = 0;
	}
	for (i = 0; i < font->FontHeight; i++) {
		b = font->data[(c - 32) * font->FontHeight + i];
		for (j = 0; j < font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				TM_ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
			} else if ((background & ILI9341_TRANSPARENT) == 0) {
				TM_ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), background);
			}
		}
	}
	/* Go to new X location */
	ILI9341_x += font->FontWidth;
}
static void TM_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color) {
	uint16_t tmp;
	if (x >= ILI9341_Opts.Width) {
		return;
	}
	if (y >= ILI9341_Opts.Height) {
		return;
	}
	if (ILI9341_Opts.Orient == TM_ILI9341_Orientation_Portrait_1) {
		/* Portrait1 */
		*(uint16_t *) (ILI9341_FRAME_BUFFER + ILI9341_Opts.CurrentLayerOffset + 2 * (ILI9341_PIXEL - x - ILI9341_Opts.Width * y)) = color;
	} else if (ILI9341_Opts.Orient == TM_ILI9341_Orientation_Portrait_2) {
		/* Portrait2, original */
		*(uint16_t *) (ILI9341_FRAME_BUFFER + ILI9341_Opts.CurrentLayerOffset + 2 * (x + ILI9341_Opts.Width * y)) = color;
	} else if (ILI9341_Opts.Orient == TM_ILI9341_Orientation_Landscape_1) {
		/* L andscape 1 */
		tmp = x;
		x = y;
		y = ILI9341_HEIGHT - 1 - tmp;
		*(uint16_t *) (ILI9341_FRAME_BUFFER + ILI9341_Opts.CurrentLayerOffset + 2 * (x + ILI9341_WIDTH * y)) = color;
	} else {
		/* Landscape2 */
		tmp = y;
		y = x;
		x = ILI9341_WIDTH - 1 - tmp;
		*(uint16_t *) (ILI9341_FRAME_BUFFER + ILI9341_Opts.CurrentLayerOffset + 2 * (x + ILI9341_WIDTH * y)) = color;
	}
	return;
}
