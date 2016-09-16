/*
 * lwip_api.h
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */

#ifndef LWIP_API_H_
#define LWIP_API_H_

#include "main.h"


extern void lan_init(unsigned char instance, unsigned long ip);

#ifdef HEADER_INCLUDE_C_FILES
#include "lwip_api.c"
#include "interface/lwip/ports/cpsw/lwiplib.c"
#endif

#endif /* LWIP_API_H_ */
