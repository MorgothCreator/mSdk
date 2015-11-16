/*
 * hs_mmcsd_interface.h
 *
 * Created: 2/11/2013 3:10:12 PM
 *  Author: XxXx
 */
/*#####################################################*/
#ifndef HS_MMCSD_INTERFACE_H_
#define HS_MMCSD_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "driver/stm32f4xx_hal_sd.h"
/*#####################################################*/
/**
  ******************************************************************************
  * @file    stm32f4_discovery_sdio_sd.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   This file contains all the functions prototypes for the SD Card
  *          stm32f4_discovery_sdio_sd driver firmware library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    stm32f4_discovery_sdio_sd.h
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file contains all the functions prototypes for the SD Card
  *          stm32f4_discovery_sdio_sd driver firmware library.
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB, STM32F4DIS-CAM
  *          and STM32F4DIS-LCD modules.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM324xG_EVAL_SDIO_SD_H
#define __STM324xG_EVAL_SDIO_SD_H

#include "driver/stm32f4xx_hal_sd.h"
#include "driver/stm32f4xx_hal_dma.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "stm32f4_discovery.h"

/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __HAL_RCC_DMA2_CLK_ENABLE
#define SD_DMAx_Tx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Rx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Tx_STREAM                 DMA2_Stream6
#define SD_DMAx_Rx_STREAM                 DMA2_Stream3
#define SD_DMAx_Tx_IRQn                   DMA2_Stream6_IRQn
#define SD_DMAx_Rx_IRQn                   DMA2_Stream3_IRQn
#define SD_DMAx_Tx_IRQHandler             DMA2_Stream6_IRQHandler
#define SD_DMAx_Rx_IRQHandler             DMA2_Stream3_IRQHandler
#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8)

#define SDIO_FIFO_ADDRESS                ((unsigned long)0x40012C80)
//extern HAL_SD_CardInfoTypedef SDCardInfo;
/*#####################################################*/
#define IsNoCard						0
#define IsSd							1
#define IsSdhc							2

/**
  * @brief SD Card information structure
  */
#define HAL_SD_CardInfoTypedef HAL_SD_CardInfoTypedef
/**
  * @}
  */

/**
  * @brief  SD status structure definition
  */
#define   MSD_OK                        ((unsigned char)0x00)
#define   MSD_ERROR                     ((unsigned char)0x01)
#define   MSD_ERROR_SD_NOT_PRESENT      ((unsigned char)0x02)

/** @defgroup STM32469I-Discovery_SD_Exported_Constants SD Exported Constants
  * @{
  */
#define SD_PRESENT               ((unsigned char)0x01)
#define SD_NOT_PRESENT           ((unsigned char)0x00)

#define SD_DATATIMEOUT           ((unsigned long)100000000)


/**
  * @}
  */

/** @defgroup STM324xG_EVAL_SDIO_SD_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM324xG_EVAL_SDIO_SD_Exported_Functions
  * @{
  */

unsigned char BSP_SD_Init(void);
unsigned char BSP_SD_DeInit(void);
unsigned char BSP_SD_ITConfig(void);


unsigned char BSP_SD_ReadBlocks(unsigned long *pData, unsigned long long ReadAddr, unsigned long BlockSize, unsigned long NumOfBlocks);
unsigned char BSP_SD_WriteBlocks(unsigned long *pData, unsigned long long WriteAddr, unsigned long BlockSize, unsigned long NumOfBlocks);
unsigned char BSP_SD_ReadBlocks_DMA(unsigned long *pData, unsigned long long ReadAddr, unsigned long BlockSize, unsigned long NumOfBlocks);
unsigned char BSP_SD_WriteBlocks_DMA(unsigned long *pData, unsigned long long WriteAddr, unsigned long BlockSize, unsigned long NumOfBlocks);
unsigned char BSP_SD_Erase(unsigned long long StartAddr, unsigned long long EndAddr);
void    BSP_SD_IRQHandler(void);
void    BSP_SD_DMA_Tx_IRQHandler(void);
void    BSP_SD_DMA_Rx_IRQHandler(void);
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void);
void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef *CardInfo);
unsigned char BSP_SD_IsDetected(void);

/* These __weak function can be surcharged by application code in case the current settings (e.g. DMA stream)
   need to be changed for specific needs */
void    BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params);
void    BSP_SD_Detect_MspInit(SD_HandleTypeDef *hsd, void *Params);
void    BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params);


unsigned int MMCSDReadCmdSend(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks);
unsigned int MMCSDWriteCmdSend(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks);

void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
void _mmcsd_idle(unsigned int unit_nr);
void _mmcsd_init(unsigned int unit_nr, new_gpio* CardDetect, new_gpio* StatusLed);
#ifdef __cplusplus
}
#endif

#endif /* __STM324xG_EVAL_SDIO_SD_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
/*#####################################################*/
timer(sd_timer_delay);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsd_interface.c"
#endif
/*#####################################################*/
#endif /* HS_MMCSD_INTERFACE_H_ */
/*#####################################################*/
