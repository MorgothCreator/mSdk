/*
 * sntp.h
 *
 *  Created on: Dec 1, 2016
 *      Author: John Smith
 */

#ifndef LIB_APP_SNTP_SNTP_H_
#define LIB_APP_SNTP_SNTP_H_

#include "api/uart_api.h"

#ifdef __cplusplus
extern "C" {
#endif

//Custom configuration
#define SNTP_SERVER_ADDRESS "82.197.164.46" /* swisstime.ethz.ch */
#define SNTP_UPDATE_DELAY 90000 /* SNTP update every 90 seconds */
//ChibiOS RTC drivers
#define SNTP_SET_SYSTEM_TIME(sec) uart.printf(DebugCom, "Time id %u\n\r", sec)
#define SNTP_GET_SYSTEM_TIME(sec, us) \
    do{uint64_t time = rtcGetTimeUnixUsec(&RTCD1);\
       (sec) = time / 1000000;\
       (us) = time % 1000000;}while(0)

void sntp_init(void);
void sntp_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* LIB_APP_SNTP_SNTP_H_ */
