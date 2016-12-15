/*
 * lwip_api.c
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */

#include "main.h"
#include "api/lan_def.h"
#include "api/lan_api.h"
#include "api/uart_api.h"


#ifdef USE_LWIP
#include "interface/lan_interface.h"
//extern new_uart* DebugCom;

lan_t lan = {
		_lan_init,
		_lan_idle,
};

#endif

