/*
 * hs_mmcsd_interface.h
 *
 * Created: 2/11/2013 3:10:12 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef HS_MMCSD_INTERFACE_H_
#define HS_MMCSD_INTERFACE_H_
#include <stdbool.h>
#include "api/gpio_def.h"
/*#####################################################*/
unsigned int MMCSDWriteCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks);
unsigned int MMCSDReadCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks);
void _mmcsd_init(unsigned int unit_nr, new_gpio* Cs, new_gpio* StatusLed);
void _mmcsd_idle(unsigned int unit_nr);
void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
/*#####################################################*/
#endif /* HS_MMCSD_INTERFACE_H_ */
/*#####################################################*/
