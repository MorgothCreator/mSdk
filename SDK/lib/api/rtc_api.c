/*
 * rtc_api.c
 *
 *  Created on: Sep 25, 2016
 *      Author: John Smith
 */

#include <stdbool.h>
#include "api/rtc_def.h"
#include "api/rtc_api.h"
#include "interface/int_rtc_interface.h"

bool rtc_int_rtc_init()
{
	return _rtc_int_rtc_init();
}

bool rtc_set_calendar(rtc_t *calendar, unsigned char HourFormat)
{
	return _rtc_set_calendar(calendar, HourFormat);
}

bool rtc_get_calendar(rtc_t *calendar)
{
	return _rtc_get_calendar(calendar);
}

bool rtc_set_register_8bit(unsigned int addr, unsigned char value)
{
	return _rtc_set_register_8bit(addr, value);
}

bool rtc_read_register_8bit(unsigned int addr, unsigned char *value)
{
	return _rtc_read_register_8bit(addr, value);
}

bool rtc_set_register_16bit(unsigned int addr, unsigned short value)
{
	return _rtc_set_register_16bit(addr, value);
}

bool rtc_read_register_16bit(unsigned int addr, unsigned short *value)
{
	return _rtc_read_register_16bit(addr, value);
}

bool rtc_set_register_32bit(unsigned int addr, unsigned long value)
{
	return _rtc_set_register_32bit(addr, value);
}

bool rtc_read_register_32bit(unsigned int addr, unsigned long *value)
{
	return _rtc_read_register_32bit(addr, value);
}

bool rtc_setup_tamper()
{
	return _rtc_setup_tamper();
}



