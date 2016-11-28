/*
 * qspi_interface.h
 *
 *  Created on: Nov 20, 2016
 *      Author: John Smith
 */

#ifndef STM32F4XX_INTERFACE_QSPI_INTERFACE_H_
#define STM32F4XX_INTERFACE_QSPI_INTERFACE_H_

#include "driver/stm32f7xx_hal_qspi.h"
#include "driver/stm32f7xx_hal_gpio.h"
#include "driver/stm32f7xx_hal_rcc.h"
/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32446E_Discovery_QSPI_Exported_Constants Exported Constants
  * @{
  */


/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32446E_Discovery_QSPI_Exported_Types Exported Types
  * @{
  */

/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @addtogroup STM32446E_Discovery_QSPI_Exported_Functions
  * @{
  */
unsigned char QSPI_N24Q_Init       (Qspi_Mem_t *param);
unsigned char QSPI_N24Q_DeInit     (Qspi_Mem_t *param);
unsigned char QSPI_N24Q_Read       (Qspi_Mem_t *param, unsigned char* pData, unsigned long ReadAddr, unsigned long Size);
unsigned char QSPI_N24Q_Write      (Qspi_Mem_t *param, unsigned char* pData, unsigned long WriteAddr, unsigned long Size);
unsigned char QSPI_N24Q_Erase_Block(Qspi_Mem_t *param, unsigned long BlockAddress);
unsigned char QSPI_N24Q_Erase_Chip (Qspi_Mem_t *param);
unsigned char QSPI_N24Q_GetStatus  (Qspi_Mem_t *param);
//unsigned char QSPI_N24Q_GetInfo    (Qspi_Mem_t *param, QSPI_InfoTypeDef* pInfo);
unsigned char QSPI_N24Q_MemoryMappedMode(Qspi_Mem_t *param);

/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void QSPI_N24Q_MspInit(void *Params);
void QSPI_N24Q_MspDeInit(void *Params);

#endif /* STM32F4XX_INTERFACE_QSPI_INTERFACE_H_ */
