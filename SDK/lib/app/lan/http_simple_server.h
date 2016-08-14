/*
 * http_simple_server.h
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */

#ifndef HTTP_SIMPLE_SERVER_H_
#define HTTP_SIMPLE_SERVER_H_

#include "main.h"
#ifdef USE_LWIP
#include "../../lib/api/lan_api.h"
#include <string.h>

#define ECHO_SERVER_PORT                   80
/** The poll delay is X*500ms */
#define ECHO_POLL_INTERVAL                 4
/** Priority for tcp pcbs */
#define ECHO_TCP_PRIO                      TCP_PRIO_MAX

#define MAX_SIZE                           4096


#include "lwip/tcp.h"
/* This is the data for the actual web page. */
static char indexdata[] =
"HTTP/1.0 200 OK\r\n\
Content-type: text/html\r\n\
\r\n\
<html> \
<head><title>A test page</title></head> \
<body> \
This is a small test page. \
</body> \
</html>";

void http_simple_init(unsigned char instance, unsigned long ip);

#ifdef HEADER_INCLUDE_C_FILES
#include "http_simple_server.c"
#endif
#endif

#endif /* HTTP_SIMPLE_SERVER_H_ */
