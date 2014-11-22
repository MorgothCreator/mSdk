/*
 * usb_interface.c
 *
 *  Created on: Dec 25, 2012
 *      Author: XxXx
 */
#include "usb_host_msc_interface.h"


#include "api/gpio_api.h"

#include "lib/fs/fat.h"

#include "sys/sysdelay.h"

#include "board_init.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "lib/fat_fs/inc/ff.h"

extern new_uart* DebugCom;
new_gpio *LedStatusUsb0 = NULL;
new_gpio *LedStatusUsb1 = NULL;

void USBHCDEvents(void *pvData);

//*****************************************************************************
//
// This buffer holds the full path to the current working directory.  Initially
// it is root ("/").
//
//*****************************************************************************
#define PATH_BUF_SIZE   4
//static char g_cCwdBuf1[PATH_BUF_SIZE] = "3:/";
//static char g_cCwdBuf2[PATH_BUF_SIZE] = "4:/";

FATFS g_sFatFs1;
FATFS g_sFatFs2;
DIR g_sDirObject;



unsigned int USBMSCReadBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	//unsigned int status = CPUIntStatus();
	//CPUirqe();
/*	if(LedStatusUsb0) gpio_out(LedStatusUsb0, 1);
	if(USBHMSCBlockRead((unsigned int)_ctrl, block, ptr, nblks) == 0)
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 1;
	}
	else
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 0;
	}*/
	return 0;
}

unsigned int USBMSCWriteBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	//unsigned int status = CPUIntStatus();
	//CPUirqe();
	/*if(LedStatusUsb0) gpio_out(LedStatusUsb0, 1);
	if(USBHMSCBlockWrite((unsigned int)_ctrl, block, ptr, nblks) == 0)
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 1;
	}
	else
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 0;
	}*/
	return 0;
}

void _usb_msc_host_ioctl(void *ctrl, unsigned int  command,  unsigned int *buffer)
{
	/*tUSBHMSCInstance * _ctrl = (tUSBHMSCInstance *)ctrl;
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           *buffer = _ctrl->ulNumBlocks;
            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = _ctrl->ulBlockSize;

            break;
        }
        default:
        {
            *buffer = 0;
            break;
        }

    }*/
}



void _usb_msc_host_init(unsigned int instance, new_gpio* StatusLed)
{

}

extern FileInfo_t *FILE1;

void _usb_msc_host_idle(unsigned int instance)
{

}
