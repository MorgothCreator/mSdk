/*
 * int_rtc_interface.c
 *
 *  Created on: Sep 25, 2016
 *      Author: John Smith
 */

#include <stdbool.h>
#include "int_rtc_interface.h"

static bool rtc_init_status = true;

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

bool _rtc_int_rtc_init()
{
	return false;
}


bool _rtc_set_calendar(rtc_t *calendar, unsigned char HourFormat)
{
	return true;
}

bool _rtc_get_calendar(rtc_t *calendar)
{
	return true;
}


bool _rtc_set_register_8bit(unsigned int addr, unsigned char value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	return true;
}

bool _rtc_read_register_8bit(unsigned int addr, unsigned char *value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	return true;
}

bool _rtc_set_register_16bit(unsigned int addr, unsigned short value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	return true;
}

bool _rtc_read_register_16bit(unsigned int addr, unsigned short *value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	return true;
}

bool _rtc_set_register_32bit(unsigned int addr, unsigned long value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	return true;
}

bool _rtc_read_register_32bit(unsigned int addr, unsigned long *value)
{
	if(!rtc_init_status)
		return rtc_init_status;
	return true;
}

bool _rtc_setup_tamper()
{
	  return true;
}

bool _rtc_pool_tamper()
{
	//if(HAL_RTCEx_PollForTamper1Event(&RtcHandle, 10) == HAL_OK)
		//return true;
	return false;
}

