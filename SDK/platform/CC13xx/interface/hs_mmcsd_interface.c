/*
 * hs_mmcsd_interface.c
 *
 * Created: 2/11/2013 3:10:04 PM
 *  Author: XxXx
 */ 
//#######################################################################################
#include <stdbool.h>
#include "api/gpio_def.h"

/*#####################################################*/
/*#####################################################*/
unsigned int MMCSDWriteCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return 0;
}
/*#####################################################*/
unsigned int MMCSDReadCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return 0;
}
/*#####################################################*/
void _mmcsd_init(unsigned int unit_nr, new_gpio* Cs, new_gpio* StatusLed)
{

}
void _mmcsd_idle(unsigned int unit_nr)
{

}
void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{

}
/*#####################################################*/
