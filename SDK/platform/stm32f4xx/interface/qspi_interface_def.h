/*
 * qspi_interface_def.h
 *
 *  Created on: Nov 20, 2016
 *      Author: John Smith
 */

#ifndef STM32F4XX_INTERFACE_QSPI_INTERFACE_DEF_H_
#define STM32F4XX_INTERFACE_QSPI_INTERFACE_DEF_H_

#include "driver/stm32f4xx_hal_qspi.h"
/* QSPI Error codes */
#define QSPI_OK            ((unsigned char)0x00)
#define QSPI_ERROR         ((unsigned char)0x01)
#define QSPI_BUSY          ((unsigned char)0x02)
#define QSPI_NOT_SUPPORTED ((unsigned char)0x04)
#define QSPI_SUSPENDED     ((unsigned char)0x08)


/* Definition for QSPI clock resources */
#define QSPI_CLK_ENABLE()             __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_CLK_DISABLE()            __HAL_RCC_QSPI_CLK_DISABLE()
#define QSPI_CS_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_CS_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOB_CLK_DISABLE()
#define QSPI_DX_CLK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_DX_CLK_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOF_CLK_DISABLE()

#define QSPI_FORCE_RESET()            __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()          __HAL_RCC_QSPI_RELEASE_RESET()

/* Definition for QSPI Pins */
#define QSPI_CS_PIN                GPIO_PIN_6
#define QSPI_CS_GPIO_PORT          GPIOB
#define QSPI_CLK_PIN               GPIO_PIN_10
#define QSPI_CLK_GPIO_PORT         GPIOF
#define QSPI_D0_PIN                GPIO_PIN_8
#define QSPI_D1_PIN                GPIO_PIN_9
#define QSPI_D2_PIN                GPIO_PIN_7
#define QSPI_D3_PIN                GPIO_PIN_6
#define QSPI_DX_GPIO_PORT          GPIOF

#define QSPI_INTERFACE_COUNT	1




#define _QSPI_INSTRUCTION_1_LINE		QSPI_INSTRUCTION_1_LINE
#define _QUAD_INOUT_FAST_READ_CMD		QUAD_INOUT_FAST_READ_CMD
#define _QSPI_ADDRESS_4_LINES			QSPI_ADDRESS_4_LINES
#define _QSPI_ADDRESS_24_BITS			QSPI_ADDRESS_24_BITS
#define _QSPI_ALTERNATE_BYTES_NONE		QSPI_ALTERNATE_BYTES_NONE
#define _QSPI_DATA_4_LINES				QSPI_DATA_4_LINES
#define _QSPI_DDR_MODE_DISABLE			QSPI_DDR_MODE_DISABLE
#define _QSPI_DDR_HHC_ANALOG_DELAY		QSPI_DDR_HHC_ANALOG_DELAY
#define _QSPI_SIOO_INST_EVERY_CMD		QSPI_SIOO_INST_EVERY_CMD

#endif /* STM32F4XX_INTERFACE_QSPI_INTERFACE_DEF_H_ */
