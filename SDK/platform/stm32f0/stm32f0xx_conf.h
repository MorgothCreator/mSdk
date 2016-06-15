/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_conf.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-May-2012
  * @brief   Library configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0XX_CONF_H
#define __STM32F0XX_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Comment the line below to disable peripheral header file inclusion */
#include "driver/stm32f0xx_hal_def.h"
#include "driver/stm32f0xx_hal_adc_ex.h"
#include "driver/stm32f0xx_hal_adc.h"
#include "driver/stm32f0xx_hal_can.h"
#include "driver/stm32f0xx_hal_cec.h"
#include "driver/stm32f0xx_hal_comp.h"
#include "driver/stm32f0xx_hal_cortex.h"
#include "driver/stm32f0xx_hal_crc_ex.h"
#include "driver/stm32f0xx_hal_crc.h"
#include "driver/stm32f0xx_hal_dac_ex.h"
#include "driver/stm32f0xx_hal_dac.h"
//#include "stm32f0xx_hal_dbgmcu.h"
#include "driver/stm32f0xx_hal_dma_ex.h"
#include "driver/stm32f0xx_hal_dma.h"
#include "driver/stm32f0xx_hal_flash_ex.h"
#include "driver/stm32f0xx_hal_flash.h"
//#include "stm32f0xx_hal_exti.h"
#include "driver/stm32f0xx_hal_gpio_ex.h"
#include "driver/stm32f0xx_hal_gpio.h"
//#include "stm32f0xx_hal_syscfg.h"
#include "driver/stm32f0xx_hal_i2c_ex.h"
#include "driver/stm32f0xx_hal_i2c.h"
#include "driver/stm32f0xx_hal_i2s.h"
#include "driver/stm32f0xx_hal_irda_ex.h"
#include "driver/stm32f0xx_hal_irda.h"
#include "driver/stm32f0xx_hal_iwdg.h"
#include "driver/stm32f0xx_hal_pcd_ex.h"
#include "driver/stm32f0xx_hal_pcd.h"
#include "driver/stm32f0xx_hal_pwr_ex.h"
#include "driver/stm32f0xx_hal_pwr.h"
#include "driver/stm32f0xx_hal_rcc_ex.h"
#include "driver/stm32f0xx_hal_rcc.h"
#include "driver/stm32f0xx_hal_rtc_ex.h"
#include "driver/stm32f0xx_hal_rtc.h"
#include "driver/stm32f0xx_hal_smartcard_ex.h"
#include "driver/stm32f0xx_hal_smartcard.h"
#include "driver/stm32f0xx_hal_smbus.h"
#include "driver/stm32f0xx_hal_spi_ex.h"
#include "driver/stm32f0xx_hal_spi.h"
#include "driver/stm32f0xx_hal_tim_ex.h"
#include "driver/stm32f0xx_hal_tim.h"
#include "driver/stm32f0xx_hal_tsc.h"
#include "driver/stm32f0xx_hal_uart.h"
#include "driver/stm32f0xx_hal_usart_ex.h"
#include "driver/stm32f0xx_hal_usart.h"
#include "driver/stm32f0xx_hal_wwdg.h"
#include "driver/stm32f0xx_hal.h"
//#include "stm32f0xx_hal_misc.h"  /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32F0XX_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
