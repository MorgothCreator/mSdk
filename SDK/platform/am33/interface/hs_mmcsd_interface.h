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
//extern mmcsdCtrlInfo  ctrlInfo;
/*#####################################################*/
void _mmcsd_init(void *SdCtrlStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed);
void _mmcsd_idle(void *SdCtrlStruct);
void _mmcsd_ioctl(void *SdCtrlStruct, unsigned int  command,  unsigned int *buffer);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsd_interface.c"
#endif
/*#####################################################*/
#endif /* HS_MMCSD_INTERFACE_H_ */
/*#####################################################*/
