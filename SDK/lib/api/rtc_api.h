/*
 * rtc_api.h
 *
 *  Created on: Sep 25, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_RTC_API_H_
#define LIB_API_RTC_API_H_

#include <stdbool.h>
#include "rtc_def.h"

bool rtc_int_rtc_init();
bool rtc_set_calendar(rtc_t *calendar, unsigned char HourFormat);
bool rtc_get_calendar(rtc_t *calendar);
extern const char *rtc_weekdayname[7];
unsigned char rtc_weekday_convert(int year, int month, int day);
bool rtc_set_register_8bit(unsigned int addr, unsigned char value);
bool rtc_read_register_8bit(unsigned int addr, unsigned char *value);
bool rtc_set_register_16bit(unsigned int addr, unsigned short value);
bool rtc_read_register_16bit(unsigned int addr, unsigned short *value);
bool rtc_set_register_32bit(unsigned int addr, unsigned long value);
bool rtc_read_register_32bit(unsigned int addr, unsigned long *value);
bool rtc_setup_tamper();


#endif /* LIB_API_RTC_API_H_ */
