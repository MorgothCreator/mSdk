/*
 * core_init.c
 *
 * Created: 2/8/2013 12:30:34 AM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "core_init.h"
//#include "board_properties.h"
#include "api/gpio_api.h"
#include "driver/sys_ctrl.h"

unsigned long CoreFreq = 48000000;


void _core_init(void)
{
	SysCtrlPowerEverything();
	CoreFreq = 48000000;
}
