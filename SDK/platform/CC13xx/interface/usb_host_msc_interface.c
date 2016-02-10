/*
 * usb_interface.c
 *
 *  Created on: Dec 25, 2012
 *      Author: XxXx
 */
#include "main.h"
#include "usb_host_msc_interface.h"

unsigned int USBMSCReadBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	return 0;
}

unsigned int USBMSCWriteBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	return 0;
}


void _usb_msc_host_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{

}



void _usb_msc_host_init(unsigned int instance, new_gpio* StatusLed)
{

}


void _usb_msc_host_idle(unsigned int instance)
{

}
