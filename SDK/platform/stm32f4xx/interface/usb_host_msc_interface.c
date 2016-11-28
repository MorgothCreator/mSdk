/*
 * usb_interface.c
 *
 * Created: 3/7/2013 6:44:53 PM
 *  Author: XxXx
 */ 

#include "main.h"
#include "board_init.h"
#include "usb_host_msc_interface.h"
#include "driver/stm32f4xx_hal_hcd.h"
#include "driver/USBH/Core/Inc/usbh_core.h"
#include "driver/USBH/Class/MSC/inc/usbh_msc.h"
#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "lib/fat_fs/inc/diskio.h"
#include "lib/fat_fs/inc/ff.h"
#include "ffconf.h"

HCD_HandleTypeDef  hhcd;
USBH_HandleTypeDef usb_msc_host_param[2]; /* USB Host handle */

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_START,
  APPLICATION_RUNNING,
}MSC_ApplicationTypeDef;

MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;

#if _USE_BUFF_WO_ALIGNMENT == 0
/* Local buffer use to handle buffer not aligned 32bits*/
static DWORD scratch[_MAX_SS / 4];
#endif

//char USBDISKPath[4];          /* USB Host logical drive path */

//*****************************************************************************
//
// This buffer holds the full path to the current working directory.  Initially
// it is root ("/").
//
//*****************************************************************************
#define PATH_BUF_SIZE   7
char g_cCwdBuf1[PATH_BUF_SIZE] = "USB1:/";
//char g_cCwdBuf2[PATH_BUF_SIZE] = "4:/";

FATFS g_sFatFs1;
//FATFS g_sFatFs2;
DIR g_sDirObject;

Gpio_t *LedStatusUsb0 = NULL;
Gpio_t *LedStatusUsb1 = NULL;


/**
  * @brief  Reads Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef  status = USBH_OK;

  if ((DWORD)buff & 3) /* DMA Alignment issue, do single up to aligned buffer */
  {
#if _USE_BUFF_WO_ALIGNMENT == 0
    while ((count--)&&(status == USBH_OK))
    {
      status = USBH_MSC_Read(&usb_msc_host_param[lun], lun, sector + count, (unsigned char *)scratch, 1);
      if(status == USBH_OK)
      {
        memcpy (&buff[count * _MAX_SS] ,scratch, _MAX_SS);
      }
      else
      {
        break;
      }
    }
#else
    return res;
#endif
  }
  else
  {
    status = USBH_MSC_Read(&usb_msc_host_param[lun], lun, sector, buff, count);
  }

  if(status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&usb_msc_host_param[lun], lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog ("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USBH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef  status = USBH_OK;

  if ((DWORD)buff & 3) /* DMA Alignment issue, do single up to aligned buffer */
  {
#if _USE_BUFF_WO_ALIGNMENT == 0
    while (count--)
    {
      memcpy (scratch, &buff[count * _MAX_SS], _MAX_SS);

      status = USBH_MSC_Write(&usb_msc_host_param[lun], lun, sector + count, (BYTE *)scratch, 1) ;
      if(status == USBH_FAIL)
      {
        break;
      }
    }
#else
    return res;
#endif
  }
  else
  {
    status = USBH_MSC_Write(&usb_msc_host_param[lun], lun, sector, (BYTE *)buff, count);
  }

  if(status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&usb_msc_host_param[lun], lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = RES_WRPRT;
      break;

    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}


unsigned int USBMSCReadBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	//unsigned int status = CPUIntStatus();
	//CPUirqe();
	if(LedStatusUsb0) gpio.out(LedStatusUsb0, 1);
	if(USBH_read(0, ptr, block, nblks) == 0)
	{
		if(LedStatusUsb0) gpio.out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 1;
	}
	else
	{
		if(LedStatusUsb0) gpio.out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 0;
	}
}

unsigned int USBMSCWriteBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	//unsigned int status = CPUIntStatus();
	//CPUirqe();
	if(LedStatusUsb0) gpio.out(LedStatusUsb0, 1);
	if(USBH_write(0, ptr, block, nblks) == 0)
	{
		if(LedStatusUsb0) gpio.out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 1;
	}
	else
	{
		if(LedStatusUsb0) gpio.out(LedStatusUsb0, 0);
		//if(status & 0x80) CPUirqd();
		return 0;
	}

}

void _usb_msc_host_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{
	//HCD_HandleTypeDef * _ctrl = (HCD_HandleTypeDef *)&usb_msc_host_param[unit_nr];//(HCD_HandleTypeDef *)ctrl;
	  //DRESULT res = RES_ERROR;
	  MSC_LUNTypeDef info;
	  unsigned char lun = unit_nr;
	  switch (command)
	  {
	  /* Make sure that no pending write process */
	  case CTRL_SYNC:
	    //res = RES_OK;
	    break;

	  /* Get number of sectors on the disk (DWORD) */
	  case GET_SECTOR_COUNT :
	    if(USBH_MSC_GetLUNInfo(&usb_msc_host_param[unit_nr], lun, &info) == USBH_OK)
	    {
	      *(DWORD*)buffer = info.capacity.block_nbr;
	      //res = RES_OK;
	    }
	    else
	    {
	      //res = RES_ERROR;
	    }
	    break;

	  /* Get R/W sector size (WORD) */
	  case GET_SECTOR_SIZE :
	    if(USBH_MSC_GetLUNInfo(&usb_msc_host_param[unit_nr], lun, &info) == USBH_OK)
	    {
	      *(DWORD*)buffer = info.capacity.block_size;
	      //res = RES_OK;
	    }
	    else
	    {
	      //res = RES_ERROR;
	    }
	    break;

	    /* Get erase block size in unit of sector (DWORD) */
	  case GET_BLOCK_SIZE :

	    if(USBH_MSC_GetLUNInfo(&usb_msc_host_param[unit_nr], lun, &info) == USBH_OK)
	    {
	      *(DWORD*)buffer = info.capacity.block_size;
	      //res = RES_OK;
	    }
	    else
	    {
	      //res = RES_ERROR;
	    }
	    break;

	  //default:
	    //res = RES_PARERR;
	  }

	  return;// res;
}


/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, unsigned char id)
{
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_IDLE;
    //BSP_LED_Off(LED4);
    //BSP_LED_Off(LED5);
    //f_mount(1, (const unsigned char*)"");
    break;

  case HOST_USER_CLASS_ACTIVE://
    Appli_state = APPLICATION_START;
    break;

  default:
    break;
  }
}


void _usb_msc_host_init(unsigned int instance, new_gpio* StatusLed)
{
	//if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
	  //{    /*##-2- Init Host Library ################################################*/
		USBH_Init(&usb_msc_host_param[instance], USBH_UserProcess, 0);

		/*##-3- Add Supported Class ##############################################*/
		USBH_RegisterClass(&usb_msc_host_param[instance], USBH_MSC_CLASS);

		/*##-4- Start Host Process ###############################################*/
		USBH_Start(&usb_msc_host_param[instance]);
	  //}

}

void _usb_msc_host_idle(unsigned int instance)
{
    /* USB Host Background task */
    USBH_Process(&usb_msc_host_param[instance]);
    /* Mass Storage Application State Machine */
    switch(Appli_state)
    {
    case APPLICATION_START:
        g_sFatFs1.drv_rw_func.DriveStruct = (void*)&usb_msc_host_param[instance];
        g_sFatFs1.drv_rw_func.drv_r_func = USBMSCReadBlock;
        g_sFatFs1.drv_rw_func.drv_w_func = USBMSCWriteBlock;
#if (_FFCONF == 82786)
        if(!f_mount(instance, &g_sFatFs1))
#else
        char drv_name_buff[6];
        drv_name_buff[1] = 'U';
        drv_name_buff[1] = 'S';
        drv_name_buff[1] = 'B';
        drv_name_buff[0] = '1' + instance;
        drv_name_buff[1] = ':';
        drv_name_buff[2] = '\0';
        if(!f_mount(&g_sFatFs1, drv_name_buff, 1))
#endif
        {
        	if(f_opendir(&g_sDirObject, g_cCwdBuf1) == FR_OK)
            {
#ifdef USBH_MSC_DEBUG_EN
						if(DebugCom)
						{
							uart.printf(DebugCom,   "USBH MSC%d drive %d mounted\n\r" , 0 , 1);
							uart.printf(DebugCom,   "USBH MSC%d Fat fs detected\n\r" , 0);
							uart.printf(DebugCom, "USBH MSC%d Fs type:                 " , 0);
							if(g_sFatFs1.fs_type == FS_FAT12)	{
								uart.printf(DebugCom, "Fat12");}
							else if(g_sFatFs1.fs_type == FS_FAT16){
								uart.printf(DebugCom, "Fat16");}
							else if(g_sFatFs1.fs_type == FS_FAT32){
								uart.printf(DebugCom, "Fat32");}
							else if(g_sFatFs1.fs_type == FS_EXFAT){
								uart.printf(DebugCom, "eXFAT");}
							else								{ 				uart.printf(DebugCom, "None");}
							uart.printf(DebugCom, "\n\r");
																				//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
							uart.printf(DebugCom, "USBH MSC%d BytesPerSector:          %d \n\r",0, /*(int)g_sFatFs.s_size*/512);
							uart.printf(DebugCom, "USBH MSC%d SectorsPerCluster:       %d \n\r",0, (int)g_sFatFs1.csize);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																				//UARTprintf(DebugCom, "USBH MSC%d NumberOfFats:            %d \n\r",0, (int)g_sFatFs1.n_fats);
																				//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																				//UARTprintf(DebugCom, "USBH MSC%d DataSectionBegin:        %d \n\r",0, (int)g_sFatFs1.fatbase);
							uart.printf(DebugCom, "USBH MSC%d uSD DiskCapacity:        %uMB\n\r",0, (unsigned long)((unsigned long long)((unsigned long long)g_sFatFs1.n_fatent * (unsigned long long)/*g_sFatFs.s_size*/512 * (unsigned long long)g_sFatFs1.csize) / 1000000));
						}
#endif
            }
        	else  if(DebugCom)										uart.printf(DebugCom,   "USBH %d ERROR oppening path\n\r" , 0);
        }
        else  if(DebugCom)												uart.printf(DebugCom,   "USBH %d ERROR mounting disk\n\r" , 0);
        Appli_state = APPLICATION_IDLE;
        break;

    case APPLICATION_IDLE:
    	default:
    		break;
    }

}
