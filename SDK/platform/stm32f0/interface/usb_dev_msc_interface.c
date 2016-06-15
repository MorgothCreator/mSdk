
/*
 * usb_dev_msc_interface.c
 *
 *  Created on: Oct 18, 2015
 *      Author: Iulian Gheorghiu
 */

#include "usb_dev_msc_interface.h"
#include "sys/sysdelay.h"
//#include "aintc/aintc_usb.h"
//#include "clk/clk_usb.h"

#include "board_init.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
//#include "usb_dev_msc/usb_msc_structs.h"

USBD_DRV_RW_FUNC drv_rw_func[2];
unsigned char *dataBuffer;

void _usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present)
{

}
void _usb_msc_dev_init(unsigned int instance, void *slave_controls)
{

}


