/*
 * int_rtc_interface.c
 *
 *  Created on: Nov 6, 2016
 *      Author: John Smith
 */

#include "int_rtc_interface.h"

bool _rtc_int_rtc_init()
{
	return false;
}

bool _rtc_set_calendar(rtc_t *calendar, unsigned char HourFormat)
{
	return false;
}

bool _rtc_get_calendar(rtc_t *calendar)
{
	return false;
}

bool _rtc_set_register_8bit(unsigned int addr, unsigned char value)
{
	return false;
}

bool _rtc_read_register_8bit(unsigned int addr, unsigned char *value)
{
	return false;
}

bool _rtc_set_register_16bit(unsigned int addr, unsigned short value)
{
	return false;
}

bool _rtc_read_register_16bit(unsigned int addr, unsigned short *value)
{
	return false;
}

bool _rtc_set_register_32bit(unsigned int addr, unsigned long value)
{
	return false;
}

bool _rtc_read_register_32bit(unsigned int addr, unsigned long *value)
{
	return false;
}

bool _rtc_setup_tamper()
{
	return false;
}


