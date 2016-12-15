/*
 * time.h
 *
 *  Created on: Dec 2, 2016
 *      Author: John Smith
 */

#ifndef LIB_LIB_UTIL_TIME_H_
#define LIB_LIB_UTIL_TIME_H_

typedef struct time_s
{
	unsigned short ms;
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	unsigned char day;
	unsigned char weekday;
	unsigned char month;
	unsigned short year;
}time_t;

#endif /* LIB_LIB_UTIL_TIME_H_ */
