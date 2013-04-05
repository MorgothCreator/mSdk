/*
 * hs_mmcsd_interface.h
 *
 *  Created on: Dec 13, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef HS_MMCSD_INTERFACE_H_
#define HS_MMCSD_INTERFACE_H_
/*#####################################################*/
#include "api/gpio_def.h"
/*#####################################################*/
void _mmcsd_init(void *SdStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed);
void mmcsd_idle(void *SdStruct);
/*#####################################################*/
#include "hs_mmcsd_interface.c"
/*#####################################################*/
#endif /* HS_MMCSD_INTERFACE_H_ */
/*#####################################################*/
