#ifndef B_ILI9341_H
#define B_ILI9341_H 130

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "B_stm32f4_fonts.h"
#include "B_stm32f4_gpio.h"
#include "B_stm32f4_spi.h"
#include "B_stm32f4_dma.h"
#include "B_stm32f4_spi_dma.h"
#include "tm_stm32f4_delay.h"
//#include "B_stm32f4_spi_dma.h"

/**
 * @brief  CS PIN for SPI, used as on STM32F429-Discovery board
 */
#ifndef ILI9341_CS_PIN
#define ILI9341_CS_PORT       GPIOC
#define ILI9341_CS_PIN        GPIO_Pin_2
#endif

/**
 * @brief  WRX PIN for data/command, used as on STM32F429-Discovery board
 */
#ifndef ILI9341_WRX_PIN
#define ILI9341_WRX_PORT      GPIOD
#define ILI9341_WRX_PIN       GPIO_Pin_13
#endif

/**
 * @brief  RESET for LCD
 */
#ifndef ILI9341_RST_PIN
#define ILI9341_RST_PORT      GPIOD
#define ILI9341_RST_PIN       GPIO_Pin_12
#endif

/* LCD settings */
#define ILI9341_WIDTH        240
#define ILI9341_HEIGHT       320
#define ILI9341_PIXEL        76800

/* Colors */
#define ILI9341_COLOR_WHITE			0xFFFF
#define ILI9341_COLOR_BLACK			0x0000
#define ILI9341_COLOR_RED       0xF800
#define ILI9341_COLOR_GREEN			0x07E0
#define ILI9341_COLOR_GREEN2		0xB723
#define ILI9341_COLOR_BLUE			0x001F
#define ILI9341_COLOR_BLUE2			0x051D
#define ILI9341_COLOR_YELLOW		0xFFE0
#define ILI9341_COLOR_ORANGE		0xFBE4
#define ILI9341_COLOR_CYAN			0x07FF
#define ILI9341_COLOR_MAGENTA		0xA254
#define ILI9341_COLOR_GRAY			0x7BEF
#define ILI9341_COLOR_BROWN			0xBBCA

/* Transparent background, only for strings and chars */
#define ILI9341_TRANSPARENT			0x80000000

/**
 * @}
 */
 
/**
 * @defgroup TM_ILI9341_Typedefs
 * @brief    Library Typedefs
 * @{
 */


/**
 * @brief  Possible orientations for LCD
 */
typedef enum {
	ILI9341_Orientation_Portrait_1,  /*!< Portrait orientation mode 1 */
	ILI9341_Orientation_Portrait_2,  /*!< Portrait orientation mode 2 */
	ILI9341_Orientation_Landscape_1, /*!< Landscape orientation mode 1 */
	ILI9341_Orientation_Landscape_2  /*!< Landscape orientation mode 2 */
}ILI9341_Orientation_t;


void ILI9341_Init(void);
void ILI9341_InitLCD(void);
void TM_ILI9341_Fill(uint32_t color);
void TM_ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void TM_ILI9341_Rotate(ILI9341_Orientation_t orientation);
void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t *font, uint32_t foreground, uint32_t background);
void TM_ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, uint32_t foreground, uint32_t background);
void TM_ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void TM_ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void TM_ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
#endif
