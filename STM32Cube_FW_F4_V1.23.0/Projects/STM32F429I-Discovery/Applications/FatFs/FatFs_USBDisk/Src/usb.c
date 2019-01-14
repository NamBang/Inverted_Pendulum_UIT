#include "usb.h"
#include <stdbool.h>

FATFS USBDISKFatFs;           /* File system object for USB disk logical drive */
FIL MyFile;                   /* File object */
char USBDISKPath[4];          /* USB Host logical drive path */
USBH_HandleTypeDef hUSBHost; /* USB Host handle */
MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
extern char buffer1[20],buffer2[20],Start_Write_USB[],Close_USB[];
extern bool flag;
extern UART_HandleTypeDef huart1;
void MSC_Application(void)
{
    FRESULT res;                                          /* FatFs function common result code */
    uint32_t byteswritten;                     /* File write/read counts */

    /* Register the file system object to the FatFs module */
    if(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
    {
        /* FatFs Initialization Error */
        Error_Handler();
    }
    else
    {
        /* Create and Open a new text file object with write access */
        if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        {
            /* 'STM32.TXT' file Open for write Error */
            Error_Handler();
        }
        else
        {
					while(flag){
						HAL_UART_Transmit_IT(&huart1,(uint8_t *)Start_Write_USB,strlen(Start_Write_USB));
            /* Write data to the text file */
            //res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
            res = f_write(&MyFile, buffer1, sizeof(buffer1), (void *)&byteswritten);

            if((byteswritten == 0) || (res != FR_OK))
            {
                /* 'STM32.TXT' file Write or EOF Error */
                Error_Handler();
            }
					}
            /* Close the open text file */
						//HAL_UART_Transmit_IT(&huart1,(uint8_t *)Close_USB,strlen(Close_USB));
            f_close(&MyFile);

        }
    }
    /* Unlink the USB disk I/O driver */
    FATFS_UnLinkDriver(USBDISKPath);
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id: Host Library user message ID
  * @retval None
  */
void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
    switch(id)
    {
    case HOST_USER_SELECT_CONFIGURATION:
        break;

    case HOST_USER_DISCONNECTION:
        Appli_state = APPLICATION_IDLE;
        BSP_LED_Off(LED3);
        BSP_LED_Off(LED4);
        f_mount(NULL, (TCHAR const*)"", 0);
        break;

    case HOST_USER_CLASS_ACTIVE:
        Appli_state = APPLICATION_START;
        break;

    default:
        break;
    }
}
