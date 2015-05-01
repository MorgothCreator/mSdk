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
#include "ports/cpsw/include/locator.h"
//#include "httpd.h"
#include "ports/cpsw/include/lwiplib.h"
#include "lwipopts.h"

//#include "ports/cpsw/lwiplib.c"

#define LEN_IP_ADDR                        (4u)

#ifdef CPSW_SWITCH_CONFIG
unsigned int IsEthAddrValid(unsigned char *ethAddr, unsigned int addrTyp);
unsigned int UserEthAddrInfoGet(unsigned char *ethAddr, unsigned int typ);
void UserALEEntryIndexInfoPrint(unsigned int *aleEntry);
#endif
void CPSWCore0RxIsr(void);
void CPSWCore0TxIsr(void);

void lan_interface_init(void);


#endif

#endif /* LWIP_API_H_ */
