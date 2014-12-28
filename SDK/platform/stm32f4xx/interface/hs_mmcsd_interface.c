/*
 * hs_mmcsd_interface.c
 *
 * Created: 2/11/2013 3:10:04 PM
 *  Author: XxXx
 */
//#######################################################################################
#include <stdbool.h>
#include "board_properties.h"
#include "hs_mmcsd_interface.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "lib/fs/fat.h"
//#######################################################################################
unsigned int MMCSDWriteCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return false;
}

unsigned int MMCSDReadCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return false;
}

void _mmcsd_ioctl(void *_ctrl, unsigned int  command,  unsigned int *buffer)
{

}

bool _mmcsd_init(void *SdStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed)
{
	return false;
}
void _mmcsd_idle(void *SdStruct)
{

}
//#######################################################################################
