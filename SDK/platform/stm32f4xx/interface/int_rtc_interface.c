/*
 * int_rtc_interface.c
 *
 *  Created on: Sep 25, 2016
 *      Author: John Smith
 */

#include <stdbool.h>
#include "int_rtc_interface.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_pwr.h"
#include "driver/stm32f4xx_hal_rtc.h"
#include "driver/stm32f4xx_hal_rtc_ex.h"

static bool rtc_init_status = true;
static RTC_HandleTypeDef RtcHandle;

extern unsigned char rtc_weekday_convert(int year, int month, int day);

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  *
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTc clock source */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /*##-2- Configure LSE as RTC clock source ###################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  rtc_init_status = false;
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
	  rtc_init_status = false;
  }

  /*##-3- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}


bool _rtc_int_rtc_init()
{
	RtcHandle.Instance = RTC;
	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
	{
	  /* Initialization Error */
		  rtc_init_status = false;
	}
	return rtc_init_status;
}


bool _rtc_set_calendar(rtc_t *calendar, unsigned char HourFormat)
{
	if(!rtc_init_status)
		return rtc_init_status;
	RTC_DateTypeDef sdatestructure;
	RTC_TimeTypeDef stimestructure;

	/*##-1- Configure the Date #################################################*/
	/* Set Date: Tuesday April 14th 2015 */
	sdatestructure.Year = calendar->Year;
	sdatestructure.Month = calendar->Month;
	sdatestructure.Date = calendar->Date;
	sdatestructure.WeekDay = rtc_weekday_convert(calendar->Year, calendar->Month, calendar->Date);

	if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BIN) != HAL_OK)
	{
	    /* Initialization Error */
		return false;
	}

	/*##-2- Configure the Time #################################################*/
	/* Set Time: 02:00:00 */
	stimestructure.Hours = calendar->Hours;
	stimestructure.Minutes = calendar->Minutes;
	stimestructure.Seconds = calendar->Seconds;
	stimestructure.TimeFormat = HourFormat & 0x01;
	stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

	if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BIN) != HAL_OK)
	{
	    /* Initialization Error */
		return false;
	  }

	/*##-3- Writes a data in a RTC Backup data Register1 #######################*/
	//HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
	return true;
}

bool _rtc_get_calendar(rtc_t *calendar)
{
	if(!rtc_init_status)
		return rtc_init_status;
	RTC_DateTypeDef sdatestructureget;
	RTC_TimeTypeDef stimestructureget;

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	calendar->Year = sdatestructureget.Year;
	calendar->Month = sdatestructureget.Month;
	calendar->Date = sdatestructureget.Date;
	calendar->WeekDay = sdatestructureget.WeekDay;

	calendar->Hours = stimestructureget.Hours;
	calendar->Minutes = stimestructureget.Minutes;
	calendar->Seconds = stimestructureget.Seconds;
	calendar->TimeFormat = stimestructureget.TimeFormat;
	return true;
}


bool _rtc_set_register_8bit(unsigned int addr, unsigned char value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	HAL_RTCEx_BKUPWrite(&RtcHandle, addr >> 2, HAL_RTCEx_BKUPRead(&RtcHandle, (addr >> 2) & (0xFF << ((addr & 0x03) << 3))) + (value << ((addr & 0x03) << 3)));
	return true;
}

bool _rtc_read_register_8bit(unsigned int addr, unsigned char *value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	unsigned long tmp = HAL_RTCEx_BKUPRead(&RtcHandle, (addr >> 2));
	*value = (tmp >> ((addr & 0x03) << 3));
	return true;
}

bool _rtc_set_register_16bit(unsigned int addr, unsigned short value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	HAL_RTCEx_BKUPWrite(&RtcHandle, addr >> 1, HAL_RTCEx_BKUPRead(&RtcHandle, (addr >> 1) & (0xFFFF << ((addr & 0x01) << 4))) + (value << ((addr & 0x01) << 4)));
	return true;
}

bool _rtc_read_register_16bit(unsigned int addr, unsigned short *value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	unsigned long tmp = HAL_RTCEx_BKUPRead(&RtcHandle, (addr >> 1));
	*value = (tmp >> ((addr & 0x01) << 4));
	return true;
}

bool _rtc_set_register_32bit(unsigned int addr, unsigned long value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	HAL_RTCEx_BKUPWrite(&RtcHandle, addr >> 1, value);
	return true;
}

bool _rtc_read_register_32bit(unsigned int addr, unsigned long *value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	*value = HAL_RTCEx_BKUPRead(&RtcHandle, (addr >> 1));
	return true;
}

bool _rtc_setup_tamper()
{
	RTC_TamperTypeDef  stamperstructure;

	/*##-1- Configure the Tamper ###############################################*/
	HAL_RTCEx_DeactivateTamper(&RtcHandle, RTC_TAMPER_1);

	/* Use PC13 as Tamper 1 with interrupt mode */
	stamperstructure.Filter = RTC_TAMPERFILTER_DISABLE;
	stamperstructure.PinSelection = RTC_TAMPERPIN_PC13;
	stamperstructure.Tamper = RTC_TAMPER_1;
	stamperstructure.Trigger = RTC_TAMPERTRIGGER_RISINGEDGE;
	stamperstructure.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV256;
	stamperstructure.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
	stamperstructure.TamperPullUp = RTC_TAMPER_PULLUP_DISABLE;
	stamperstructure.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE;

	if(HAL_RTCEx_SetTamper(&RtcHandle, &stamperstructure) != HAL_OK)
	{
	  /* Initialization Error */
		return false;
	}

	/* Clear the Tamper Flag */
	  __HAL_RTC_TAMPER_CLEAR_FLAG(&RtcHandle, RTC_FLAG_TAMP1F);
	  return true;
}

bool _rtc_pool_tamper()
{
	if(HAL_RTCEx_PollForTamper1Event(&RtcHandle, 10) == HAL_OK)
		return true;
	return false;
}

