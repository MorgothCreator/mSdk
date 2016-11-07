/*
 * int_rtc_interface.h
 *
 *  Created on: Nov 6, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_INT_RTC_INTERFACE_H_
#define LIB_API_INT_RTC_INTERFACE_H_

#include <stdbool.h>
#include "api/rtc_def.h"

bool _rtc_int_rtc_init();
bool _rtc_set_calendar(rtc_t *calendar, unsigned char HourFormat);
bool _rtc_get_calendar(rtc_t *calendar);
bool _rtc_read_register_8bit(unsigned int addr, unsigned char *value);
bool _rtc_set_register_16bit(unsigned int addr, unsigned short value);
bool _rtc_read_register_16bit(unsigned int addr, unsigned short *value);
bool _rtc_set_register_32bit(unsigned int addr, unsigned long value);
bool _rtc_read_register_32bit(unsigned int addr, unsigned long *value);
bool _rtc_set_register_8bit(unsigned int addr, unsigned char value);
bool _rtc_setup_tamper();

#endif /* LIB_API_INT_RTC_INTERFACE_H_ */
