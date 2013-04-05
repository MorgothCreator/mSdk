/*
 * clk_usb.h
 *
 *  Created on: Dec 25, 2012
 *      Author: XxXx
 */

#ifndef CLK_USB_H_
#define CLK_USB_H_

void _USB0ModuleClkConfig(void);
void USBModuleClkEnable(unsigned int ulIndex, unsigned int ulBase);
void USBModuleClkDisable(unsigned int ulIndex, unsigned int ulBase);

#ifdef HEADER_INCLUDE_C_FILES
#include "clk_usb.c"
#endif

#endif /* CLK_USB_H_ */
