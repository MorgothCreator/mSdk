/*
 * http_client.h
 *
 *  Created on: Dec 2, 2016
 *      Author: John Smith
 */

#ifndef LIB_APP_LWIP_HTTP_CLIENT_HTTP_CLIENT_H_
#define LIB_APP_LWIP_HTTP_CLIENT_HTTP_CLIENT_H_

#include "board_init.h"

#define TCP_CLIENT_DEBUG(message)	uart.printf(DebugCom, message)

void tcp_setup(char *ip_addr, char *mesage);

#endif /* LIB_APP_LWIP_HTTP_CLIENT_HTTP_CLIENT_H_ */
