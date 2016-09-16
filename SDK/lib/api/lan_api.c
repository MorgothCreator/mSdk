/*
 * lwip_api.c
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */
#include <api/lan_api.h>
#include "api/uart_api.h"
//#include "interface/lan_interface.h"


#ifdef USE_LWIP
extern new_uart* DebugCom;



void lan_init(unsigned char instance, unsigned long ip)
{
	_lan_interface_init(instance, ip);
}

#endif

