/*
 * lwip_api.h
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */

#ifndef LWIP_API_H_
#define LWIP_API_H_

#include "main.h"
#ifdef UseLwip
#include "api/uart_api.h"
#include "include/cpsw.h"
#include "locator.h"
//#include "httpd.h"
#include "lwiplib.h"
#include "lwipopts.h"

#include "ports/cpsw/lwiplib.c"

#define LEN_IP_ADDR                        (4u)

void lan_interface_init(void);

#include "lwip_api.c"

#endif

#endif /* LWIP_API_H_ */
