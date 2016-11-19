/*
 * core_init.c
 *
 * Created: 2/8/2013 12:30:34 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include "core_init.h"
//#include "board_init.h"
//#include "board_properties.h"
#include "sys/system_stm32f4xx.h"
#include "api/gpio_api.h"
#include "sys/system_stm32f4xx.h"
#include "driver/stm32f4xx_hal.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_pwr_ex.h"
#include "driver/stm32f4xx_hal_flash_ex.h"
//#include "driver/core.h"

/** @addtogroup STM32F4xx_System_Private_Variables
  * @{
  */

#if defined (STM32F410Tx) || defined (STM32F410Cx) || defined (STM32F410Rx)  || defined (STM32F411xE)  || defined (STM32F405xx)  || defined (STM32F415xx)  || defined (STM32F407xx)  || defined (STM32F417xx)
unsigned long FCPU = 168000000;
#endif /* STM32F40_41xxx */

#if defined (STM32F427xx) || defined (STM32F437xx) || defined (STM32F429xx) || defined (STM32F439xx) || defined (STM32F469xx) || defined(STM32F479xx)
unsigned long FCPU = 180000000;
#endif /* STM32F427_437x || STM32F429_439xx */

#if defined (STM32F401xC) || defined (STM32F401xE)
unsigned long FCPU = 84000000;
#endif /* STM32F401xx */

unsigned long PLL_REF_CLK =	HSE_VALUE;

  /**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE or HSI)
  *            SYSCLK(Hz)                     = XPLL
  *            HCLK(Hz)                       = XPLL
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = XHSI
  *            PLL_M                          = 8
  *            PLL_N                          = XPLL * 2
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(unsigned long int_osc_freq, unsigned long ext_osc_freq, unsigned long core_freq)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
	if(ext_osc_freq == 0)
	{
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
		RCC_OscInitStruct.HSIState = RCC_HSI_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		RCC_OscInitStruct.PLL.PLLM = int_osc_freq /1000000;
	}
	else
	{
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
		RCC_OscInitStruct.PLL.PLLM = 25;
#else
		RCC_OscInitStruct.PLL.PLLM = ext_osc_freq /1000000;
#endif /* USE_STM32469I_DISCO_REVA */
	}
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLN = (core_freq * 2) / 1000000;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
#if defined(STM32F469xx) || defined(STM32F479xx)
  RCC_OscInitStruct.PLL.PLLR = 6;
#endif

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

#if defined(STM32F469xx) || defined(STM32F479xx)
  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
#endif

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  PLL_REF_CLK =	RCC_OscInitStruct.PLL.PLLM * 1000000;
}

void _core_init(void)
{
	//SystemInit();
	//SystemCoreClockUpdate();
	SystemClock_Config(HSI_VALUE, HSE_VALUE, FCPU);
	HAL_Init();
#if (defined(STM32F469xx) || defined(STM32F479xx)) && defined(STM32F469I_DISCO)
#include "driver/stm32469i_discovery_sdram.h"
	BSP_STM32469I_DISCOVERY_SDRAM_Init();
#endif
	//SystemCoreClockUpdate();
	//RCC_PCLK1Config(RCC_HCLK_Div4);
	//RCC_PCLK2Config(RCC_HCLK_Div2);
	gpio.init(0);
	gpio.init(1);
	gpio.init(2);
	gpio.init(3);
	gpio.init(4);
	gpio.init(5);
	gpio.init(6);
	gpio.init(7);
	gpio.init(8);
	gpio.init(9);
	gpio.init(10);
}
