/*
 * hs_mmcsd_interface.h
 *
 *  Created on: Dec 13, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef _HS_MMCSD_INTERFACE_H_
#define _HS_MMCSD_INTERFACE_H_
/*#####################################################*/
#include "interface/hs_mmcsd_interface.h"
#include "api/edma_api.h"
#include "mmcsdlib/mmcsd_proto.h"
#include "api/gpio_def.h"
/*#####################################################*/
extern mmcsdCtrlInfo  ctrlInfo[];
/*#####################################################*/
void _mmcsd_init(unsigned int unit_nr, new_gpio* Cs, new_gpio* StatusLed);
void _mmcsd_idle(unsigned int unit_nr);
void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsd_interface.c"
#endif
/*#####################################################*/
#endif /* HS_MMCSD_INTERFACE_H_ */
/*#####################################################*/
