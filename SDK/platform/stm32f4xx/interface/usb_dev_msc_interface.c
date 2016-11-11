
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
#include "../driver/USBD/Class/MSC/usbd_storage.h"
#include "../driver/USBD/Class/MSC/usbd_desc.h"
#include "lib/fat_fs/inc/diskio.h"

DRV_RW_FUNC usbd_drv_func[2];
USBD_HandleTypeDef USBD_Device[2];

bool _usb_msc_dev_media_connected(unsigned int instance)
{
	//if(USBD_Device[instance].dev_state == USBD_STATE_DEFAULT)
		return true;
	//else
		//return false;
}

void _usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present)
{
	if(media_is_present)
		USBD_Start(&USBD_Device[instance]);
	else
		USBD_Stop(&USBD_Device[instance]);
}
void _usb_msc_dev_init(unsigned int instance, void *slave_controls)
{
	//memcpy(&usbd_drv_func[instance], slave_controls, sizeof(DRV_RW_FUNC));
	//usbd_drv_func[instance] = (USBD_DRV_RW_FUNC *)slave_controls;
	  /* Init MSC Application */
	  USBD_Init(&USBD_Device[instance], &MSC_Desc, 0);

	  /* Add Supported Class */
	  USBD_RegisterClass(&USBD_Device[instance], USBD_MSC_CLASS);

	  /* Add Storage callbacks for MSC Class */
	  USBD_MSC_RegisterStorage(&USBD_Device[instance], &USBD_DISK_fops);

	  /* Start Device Process */
	  //USBD_Start(&USBD_Device[instance]);

}


