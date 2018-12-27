/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_H
#define __usb_H
#include "main.h"

typedef enum
{
    APPLICATION_IDLE = 0,
    APPLICATION_START,
    APPLICATION_RUNNING,
} MSC_ApplicationTypeDef;

void SystemClock_Config(void);
void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
void MSC_Application(void);
#endif /* __TIM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
