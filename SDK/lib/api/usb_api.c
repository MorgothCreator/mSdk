/*
 *  lib/api/usb_msc_host_api.c
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

#include "usb_api.h"
#include "interface/usb_host_msc_interface.h"
#include "interface/usb_host_mouse_interface.h"
#include "interface/usb_dev_msc_interface.h"
#include "interface/usb_dev_com_interface.h"
#include "api/gpio_api.h"
#include "lib/gfx/controls_definition.h"
/*-----------------------------------------------------------*/
void usb_msc_host_init(unsigned int instance, new_gpio* StatusLed)
{
	_usb_msc_host_init(instance, StatusLed);
}

void usb_msc_host_idle(unsigned int instance)
{
	_usb_msc_host_idle(instance);
}

unsigned int usb_msc_host_read(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return USBMSCReadBlock(_ctrl, ptr, block, nblks);
}

unsigned int usb_msc_host_write(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return USBMSCWriteBlock(_ctrl, ptr, block, nblks);
}

void usb_msc_host_ioctl(void *_ctrl, unsigned int  command,  unsigned int *buffer)
{
	_usb_msc_host_ioctl(_ctrl, command, buffer);
}
/*-----------------------------------------------------------*/
void usb_mouse_host_init(unsigned int instance)
{
	_usb_mouse_host_init(instance);
}

void usb_mouse_host_idle(unsigned int instance, tControlCommandData *control_comand)
{
	_usb_mouse_host_idle(instance, control_comand);
}
/*-----------------------------------------------------------*/
void usb_msc_dev_init(unsigned int instance)
{
	_usb_msc_dev_init(instance);
}

void usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present)
{
	_usb_msc_dev_media_change_state(instance, media_is_present);
}
/*-----------------------------------------------------------*/
void usb_com_dev_init(unsigned int instance)
{
	_usb_com_dev_init(instance);
}

unsigned int usb_com_dev_receive(unsigned char* buff)
{
	return _usb_com_dev_receive(buff);
}

unsigned int usb_com_dev_send(unsigned char* buff, unsigned int nbytes)
{
	return _usb_com_dev_send(buff, nbytes);
}
/*-----------------------------------------------------------*/


