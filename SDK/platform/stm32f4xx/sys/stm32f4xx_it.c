/**
  ******************************************************************************
  * @file    FLASH_Program/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <interface/mmcsd_interface.h>
#include "main.h"
#include "stm32f4xx_it.h"
#include "sys/sysdelay.h"
#include "interface/usb_host_msc_interface.h"
#include "driver/USBD/Class/CDC/usbd_cdc_interface.h"
#include "driver/stm32f4xx_hal_dma.h"
#include "driver/stm32f4xx_hal.h"
#include "driver/stm32f4xx_hal_hcd.h"
#include "driver/stm32f4xx_hal_pcd.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_adc.h"
#include "driver/stm32f4xx_hal_tim.h"
#include "driver/stm32f4xx_hal_ltdc.h"
#include "driver/stm32f4xx_hal_sai.h"
#include "driver/stm32f4xx_hal_i2s.h"
#include "lwipopts.h"
/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

extern ADC_HandleTypeDef    AdcHandle;
#ifdef USE_USB_DEV
extern PCD_HandleTypeDef _hpcd[];
#endif

/* TIM handler declared in "usbd_cdc_interface.c" file */
extern TIM_HandleTypeDef USBCDCTimHandle;

//extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#ifdef LCD_TYPE_DSI
extern DMA2D_HandleTypeDef Dma2dHandle;
extern LTDC_HandleTypeDef  hltdc_eval;
extern DSI_HandleTypeDef hdsi_eval;
#endif
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)
extern SAI_HandleTypeDef         haudio_out_sai;
#endif
extern I2S_HandleTypeDef         haudio_in_i2s;
/** @addtogroup FLASH_Program
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimerCnt_Isr_Increment();
	HAL_IncTick();

}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles DMA2 Stream 3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
  BSP_SD_DMA_Rx_IRQHandler();
}

/**
  * @brief  This function handles DMA2 Stream 6 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
  BSP_SD_DMA_Tx_IRQHandler();
}

/**
  * @brief  This function handles SDIO interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  BSP_SD_IRQHandler();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles USB-On-The-Go FS global interrupt requests.
  * @param  None
  * @retval None
  */
#ifndef USE_USB_DEV
void OTG_FS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&hhcd);
}

void OTG_HS_IRQHandler(void)
{
  HAL_HCD_IRQHandler(&hhcd);
}
#else
/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  * @param  None
  * @retval None
  */
void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&_hpcd[0]);
}

void OTG_HS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&_hpcd[0]);
}
#endif

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles External line 8 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MFX_IRQOUT_PIN);
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
}
/**
  * @brief  This function handles ADC interrupt request.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&AdcHandle);
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void USBCDCTIMx_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&USBCDCTimHandle);
}

#ifdef LCD_TYPE_DSI
/**
  * @brief  This function handles DMA2D Handler.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&Dma2dHandle);
}
/**
  * @brief  This function handles LTDC interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hltdc_eval);
}

void LTDC_ER_IRQHandler(void)
{
  /* Check the interrupt and clear flag */
  HAL_LTDC_IRQHandler(&hltdc_eval);

//  Interrupt_Error_Detected = 1;
}
/**
  * @brief  This function handles DSI Handler.
  * @param  None
  * @retval None
  */
void DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&hdsi_eval);
}
#endif
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)
/**
  * @brief This function handles DMA2 Stream 5 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}
#endif
/**
  * @brief This function handles DMA1 Stream 2 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_I2Sx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_in_i2s.hdmarx);
}
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
