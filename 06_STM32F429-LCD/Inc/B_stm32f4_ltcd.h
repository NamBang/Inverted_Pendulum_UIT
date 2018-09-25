#include "stm32f4xx.h"
#include "stm32f4xx_ltdc.h"
#include "B_stm32f4_gpio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7
#define SDRAM_START_ADR             (uint32_t)0xD0000000

typedef enum {
	TM_ILI9341_Orientation_Portrait_1,  /*!< Portrait orientation mode 1 */
	TM_ILI9341_Orientation_Portrait_2,  /*!< Portrait orientation mode 2 */
	TM_ILI9341_Orientation_Landscape_1, /*!< Landscape orientation mode 1 */
	TM_ILI9341_Orientation_Landscape_2  /*!< Landscape orientation mode 2 */
} TM_ILI9341_Orientation_t;
/* Private defines */
/* Pin functions */
#define ILI9341_CS_SET				B_GPIO_SetPinHigh(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_CS_RESET			B_GPIO_SetPinLow(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_WRX_SET				B_GPIO_SetPinHigh(ILI9341_WRX_PORT, ILI9341_WRX_PIN)
#define ILI9341_WRX_RESET			B_GPIO_SetPinLow(ILI9341_WRX_PORT, ILI9341_WRX_PIN)
#define SPI_CHECK_ENABLED_RESP(SPIx, val)   if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return (val);}

#define SDRAM_START_ADR             (uint32_t)0xD0000000
#define SPI_IS_BUSY(SPIx) (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))
#define SPI_WAIT(SPIx)            while (SPI_IS_BUSY(SPIx))
#define B_SDRAM_Write8(address, value)		(*(__IO uint8_t *) (SDRAM_START_ADR + (address)) = (value))
#define B_SDRAM_Read8(address)				(*(__IO uint8_t *) (SDRAM_START_ADR + (address)))
#ifndef ILI9341_CS_PIN
#define ILI9341_CS_PORT				GPIOC
#define ILI9341_CS_PIN				GPIO_Pin_2
#endif
#define SDRAM_TIMEOUT                   ((uint32_t)0xFFFF) 
/**
 * @brief  WRX PIN for data/command, used as on STM32F429-Discovery board
 */
#ifndef ILI9341_WRX_PIN
#define ILI9341_WRX_PORT			GPIOD
#define ILI9341_WRX_PIN				GPIO_Pin_13
#endif
#define ILI9341_TRANSPARENT			0x80000000
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} TM_FontDef_t;
#ifndef B_LTCD_H
#define B_LTCD_H
void LCD_Config(void);
void LCD_AF_GPIOConfig(void);
/*Initializes ILI9341 LCD with LTDC peripheral
	It also initializes external SDRAM*/
void ILI9341_Init(void);
void B_SPI_Init(void);

uint8_t B_SDRAM_Init(void);
uint8_t B_SDRAM_InitCustomPinsCallback(void);

void TM_ILI9341_InitLCD(void);
void TM_LCD9341_InitLTDC(void);
void TM_ILI9341_InitLayers(void);
void TM_ILI9341_Rotate(TM_ILI9341_Orientation_t orientation);
void TM_ILI9341_SetLayer1(void);
void TM_ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, uint32_t foreground, uint32_t background);
#endif
