/*
 * usb_dev_msc_interface.h
 *
 *  Created on: Nov 16, 2014
 *      Author: Morgoth
 */

#ifndef USB_DEV_MSC_INTERFACE_H_
#define USB_DEV_MSC_INTERFACE_H_

#include <stdbool.h>

#define GET_SECTOR_COUNT 1
#define GET_SECTOR_SIZE 2

typedef struct {
	unsigned char		controlled_unit_nr;
	void*				DriveStruct;
	unsigned int		(*drv_r_func)(void*, void*, unsigned long, unsigned int);
	unsigned int		(*drv_w_func)(void*, void*, unsigned long, unsigned int);
	void				(*drv_ioctl_func)(unsigned int unit_nr, unsigned int, unsigned int*);
}USBD_DRV_RW_FUNC;

extern USBD_DRV_RW_FUNC drv_rw_func[];

void _usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present);
void _usb_msc_dev_init(unsigned int instance, void *slave_controls);
bool _usb_msc_dev_media_connected(unsigned int instance);


#endif /* USB_DEV_MSC_INTERFACE_H_ */
