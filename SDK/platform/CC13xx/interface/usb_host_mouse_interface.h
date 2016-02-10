/*
 * usb_mouse_interface.h
 *
 *  Created on: Nov 13, 2014
 *      Author: Morgoth
 */

#ifndef USB_MOUSE_INTERFACE_H_
#define USB_MOUSE_INTERFACE_H_
#include "lib/gfx/controls_definition.h"

#define USB_INSTANCE_Host_Mouse 1

extern unsigned int g_ulButtons;
extern int MouseXPosition;
extern int MouseYPosition;
extern int MouseState;
extern int MscState;
extern int MouseWheel;


void _usb_mouse_host_init(unsigned int instance);
void _usb_mouse_host_idle(unsigned int instance, tControlCommandData *control_comand);


#endif /* USB_MOUSE_INTERFACE_H_ */
