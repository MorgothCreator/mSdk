/*
 * lan_interface.h
 *
 *  Created on: Jul 3, 2016
 *      Author: John Smith
 */

#ifndef AM33_INTERFACE_LAN_INTERFACE_H_
#define AM33_INTERFACE_LAN_INTERFACE_H_

#include "api/uart_api.h"
#include "include/cpsw.h"
#include "locator.h"
//#include "httpd.h"
#include "lwiplib.h"
#include "lwipopts.h"

#define LEN_IP_ADDR                        (4u)

void CPSWCore0RxIsr(void);
void CPSWCore0TxIsr(void);
void _lan_get_mac_addr(unsigned int addrIdx, unsigned char *macAddr);
void _lan_ip_addr_display(unsigned int ipAddr);
void _lan_interface_init(unsigned char instance, unsigned long ip);

#endif /* AM33_INTERFACE_LAN_INTERFACE_H_ */
