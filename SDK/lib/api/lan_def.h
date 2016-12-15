/*
 * lwip_def.h
 *
 *  Created on: Dec 26, 2014
 *      Author: Iulian Gheorghiu
 */
/*#####################################################*/
#ifndef LWIP_DEF_H_
#define LWIP_DEF_H_
/*#####################################################*/
#include <stdbool.h>
/*#####################################################*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* DHCP process states */
#define DHCP_START                 (unsigned char) 1
#define DHCP_WAIT_ADDRESS          (unsigned char) 2
#define DHCP_ADDRESS_ASSIGNED      (unsigned char) 3
#define DHCP_TIMEOUT               (unsigned char) 4
#define DHCP_LINK_DOWN             (unsigned char) 5

typedef struct lan_s
{
	void (*init)(unsigned char instance, unsigned long static_ip);
	unsigned char (*idle)(unsigned char instance);
}lan_t;

/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "adc_def.c"
#endif
/*#####################################################*/
#endif /* ADC_DEF_H_ */
/*#####################################################*/
