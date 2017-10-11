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
#include "mmcsd_interface.h"

#include "api/edma_api.h"
#include "mmcsdlib/mmcsd_proto.h"
#include "api/gpio_def.h"
/*#####################################################*/
extern mmcsdCtrlInfo  ctrlInfo[];
/*#####################################################*/
unsigned int _mmcsd_read(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks);
unsigned int _mmcsd_write(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks);
void _mmcsd_init(unsigned int unit_nr, new_gpio* CardDetect, new_gpio* StatusLed);
bool _mmcsd_idle(unsigned int unit_nr);
void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsd_interface.c"
#endif
/*#####################################################*/
#endif /* HS_MMCSD_INTERFACE_H_ */
/*#####################################################*/
