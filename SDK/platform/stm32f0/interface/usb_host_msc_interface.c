/*
 * usb_interface.c
 *
 * Created: 3/7/2013 6:44:53 PM
 *  Author: XxXx
 */ 

#include "main.h"
#include "board_init.h"
#include "usb_host_msc_interface.h"

unsigned int USBMSCReadBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	return 1;
}

unsigned int USBMSCWriteBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	return 1;
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
