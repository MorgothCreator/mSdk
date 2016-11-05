/*
 * int_rtc_interface.h
 *
 *  Created on: Sep 25, 2016
 *      Author: John Smith
 */

#ifndef INT_RTC_INTERFACE_H_
#define INT_RTC_INTERFACE_H_

#include "api/rtc_def.h"

/* Defines related to Clock configuration */
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */

bool _rtc_int_rtc_init();
bool _rtc_set_calendar(rtc_t *calendar, unsigned char HourFormat);
bool _rtc_get_calendar(rtc_t *calendar);
bool _rtc_set_register_8bit(unsigned int addr, unsigned char value);
bool _rtc_read_register_8bit(unsigned int addr, unsigned char *value);
bool _rtc_set_register_16bit(unsigned int addr, unsigned short value);
bool _rtc_read_register_16bit(unsigned int addr, unsigned short *value);
bool _rtc_set_register_32bit(unsigned int addr, unsigned long value);
bool _rtc_read_register_32bit(unsigned int addr, unsigned long *value);
bool _rtc_setup_tamper();

#endif /* INT_RTC_INTERFACE_H_ */
