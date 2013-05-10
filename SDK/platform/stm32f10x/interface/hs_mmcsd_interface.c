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
bool _mmcsd_init(void *SdStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed)
{
	return false;
}
void _mmcsd_idle(void *SdStruct)
{

}
//#######################################################################################
