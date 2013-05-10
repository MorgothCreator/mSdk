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

void _usb_host_init(unsigned int instance, new_gpio* StatusLed);
void _usb_host_idle(unsigned int instance);

#ifdef HEADER_INCLUDE_C_FILES
#include "usb_interface.c"
#endif

#endif /* USB_INTERFACE_H_ */