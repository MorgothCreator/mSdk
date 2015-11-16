/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "../inc/diskio.h"
//#include "interface/mmcsdlib/mmcsd_proto.h"
//#include "interface/mmcsdlib/hs_mmcsdlib.h"
#include "../inc/ff.h"			/* FatFs configurations and declarations */

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
//extern mmcsdCtrlInfo  ctrlInfo;
extern FATFS *FatFs[];
/*-----------------------------------------------------------------------*/
/* Inicializes a Drive                                                    */

DSTATUS disk_initialize (BYTE drv)    /* Physical drive nmuber (0..) */
{
  DSTATUS stat = STA_NOINIT;
  
  //if(HCD_IsDeviceConnected(&USB_OTG_Core_dev))
  //{
    stat &= ~STA_NOINIT;
  //}
  
  return stat;
  
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	if(!FatFs[drv]->drv_rw_func.DriveStruct) return RES_PARERR;
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
		BYTE drv,		/* Physical drive nmuber (0..) */
		BYTE *buff,		/* Data buffer to store read data */
		DWORD sector,	/* Sector address (LBA) */
		BYTE count		/* Number of sectors to read (1..255) */
)
{
	if(!FatFs[drv]->drv_rw_func.DriveStruct || !FatFs[drv]->drv_rw_func.drv_r_func) return RES_PARERR;
	if(FatFs[drv]->drv_rw_func.drv_r_func((void*)FatFs[drv]->drv_rw_func.DriveStruct, (void*)buff, sector, count)) return RES_OK;
	else return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
		BYTE drv,			/* Physical drive nmuber (0..) */
		const BYTE *buff,	/* Data to be written */
		DWORD sector,		/* Sector address (LBA) */
		BYTE count			/* Number of sectors to write (1..255) */
)
{
	if(!FatFs[drv]->drv_rw_func.DriveStruct || !FatFs[drv]->drv_rw_func.drv_w_func) return RES_PARERR;
	if(FatFs[drv]->drv_rw_func.drv_w_func((void*)FatFs[drv]->drv_rw_func.DriveStruct, (void*)buff, sector, count)) return RES_OK;
	else return RES_ERROR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(!FatFs[drv]->drv_rw_func.DriveStruct) return RES_PARERR;
	return RES_OK;
}

