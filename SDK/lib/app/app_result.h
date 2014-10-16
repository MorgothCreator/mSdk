/*
 * app_result.h
 *
 *  Created on: Oct 6, 2014
 *      Author: Morgoth
 */

#ifndef APP_RESULT_H_
#define APP_RESULT_H_


typedef enum
{
	APP_RESULT_OK = 0,
	APP_RESULT_OUT_DEV_NOT_SPECIFIED,
	APP_RESULT_UNKNOWN_ERR,
	APP_RESULT_INVALID_ARGUMENT,
	APP_RESULT_OUT_OF_MEMORY,
	APP_RESULT_OUT_OF_RANGE,
	APP_RESULT_NO_FILE,
	APP_RESULT_NOT_ALLOCATED,
	APP_RESULT_NO_PATH,
	APP_RESULT_DISK_ERR,
	APP_RESULT_INVALID_NAME,
	APP_RESULT_DENIED,
	APP_RESULT_WRITE_PROTECTED,
	APP_RESULT_TIMEOUT
}APPRESULT;


#endif /* APP_RESULT_H_ */
