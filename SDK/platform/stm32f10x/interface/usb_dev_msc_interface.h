#ifndef __usb_dev_msc_interface__
#define __usb_dev_msc_interface__

#include "stdbool.h"

#define GET_SECTOR_COUNT 1
#define GET_SECTOR_SIZE 2

typedef struct {
	void*				DriveStruct;
	unsigned int		(*drv_r_func)(void*, void*, unsigned long, unsigned int);
	unsigned int		(*drv_w_func)(void*, void*, unsigned long, unsigned int);
	void				(*drv_ioctl_func)(void *_ctrl, unsigned int, unsigned int*);
}USBD_DRV_RW_FUNC;

extern USBD_DRV_RW_FUNC drv_rw_func;

void _usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present);
void _usb_msc_dev_init(unsigned int instance);

#endif
