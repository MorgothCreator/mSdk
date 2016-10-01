/*
 * rtc_def.h
 *
 *  Created on: Sep 25, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_RTC_DEF_H_
#define LIB_API_RTC_DEF_H_

typedef struct rtc_s
{
	unsigned char Year;
	unsigned char Month;
	unsigned char Date;
	unsigned char WeekDay;
	unsigned char Hours;
	unsigned char Minutes;
	unsigned char Seconds;
	unsigned char TimeFormat;
}rtc_t;

#endif /* LIB_API_RTC_DEF_H_ */
