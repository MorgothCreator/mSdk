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
void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{

}

unsigned int MMCSDReadCmdSend(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks)
{
	return 0;
}

unsigned int MMCSDWriteCmdSend(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks)
{
	return 0;
}

void _mmcsd_init(unsigned int unit_nr, new_gpio* CardDetect, new_gpio* StatusLed)
{

}
void _mmcsd_idle(unsigned int unit_nr)
{

}
//#######################################################################################
