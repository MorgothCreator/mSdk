/*
 * aintc_usb.h
 *
 *  Created on: Dec 25, 2012
 *      Author: XxXx
 */

#ifndef AINTC_USB_H_
#define AINTC_USB_H_

void USBInterruptEnable(int usbInstance);
void USBEnableInt(unsigned int ulBase);
void USBClearInt(unsigned int ulBase);

#ifdef HEADER_INCLUDE_C_FILES
#include "aintc_usb.c"
#endif

#endif /* AINTC_USB_H_ */
