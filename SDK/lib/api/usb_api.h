/*
 *  lib/api/usb_msc_host_api.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef USB_MSC_HOST_API_H_
#define USB_MSC_HOST_API_H_

#include "gpio_api.h"
#include "interface/usb_dev_msc_interface.h"
#include "lib/gfx/controls_definition.h"

extern USBD_DRV_RW_FUNC drv_rw_func[];

/*-----------------------------------------------------------*/
void usb_msc_host_init(unsigned int instance, new_gpio* StatusLed);
void usb_msc_host_idle(unsigned int instance);
unsigned int usb_msc_host_read(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks);
unsigned int usb_msc_host_write(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks);
void usb_msc_host_ioctl(void *_ctrl, unsigned int  command,  unsigned int *buffer);
/*-----------------------------------------------------------*/
void usb_mouse_host_init(unsigned int instance);
void usb_mouse_host_idle(unsigned int instance, tControlCommandData *control_comand);
/*-----------------------------------------------------------*/
void usb_msc_dev_init(unsigned int instance, void *slave_controls);
void usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present);
/*-----------------------------------------------------------*/
void usb_com_dev_init(unsigned int instance);
unsigned int usb_com_dev_receive(unsigned char* buff);
unsigned int usb_com_dev_send(unsigned char* buff, unsigned int nbytes);
/*-----------------------------------------------------------*/

#ifdef HEADER_INCLUDE_C_FILES
#include "usb_msc_host_api.c"
#endif

#endif /* USB_MSC_HOST_API_H_ */
