/*
 * usb_interface.h
 *
 * Created: 3/7/2013 6:44:43 PM
 *  Author: XxXx
 */ 


#ifndef USB_INTERFACE_H_
#define USB_INTERFACE_H_

#include <stdbool.h>
#include "api/gpio_def.h"

unsigned int USBMSCReadBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks);
unsigned int USBMSCWriteBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks);
void _usb_msc_host_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
void _usb_msc_host_init(unsigned int instance, new_gpio* StatusLed);
void _usb_msc_host_idle(unsigned int instance);


#ifdef HEADER_INCLUDE_C_FILES
#include "usb_interface.c"
#endif

#endif /* USB_INTERFACE_H_ */
