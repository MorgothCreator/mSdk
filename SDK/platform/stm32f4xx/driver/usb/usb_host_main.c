#include <stdio.h>
#include "usb_core.h"
#include "usbh_core.h"
#include "class/MSC/inc/usbh_msc_core.h"
#include "class/MSC/inc/usbh_msc_scsi.h"
#include "api/uart_api.h"
#include "lib/fat_fs/inc/ff.h"

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

#define SMALL_FONT_LINE_WIDTH	10
#define SMALL_FONT_COLUMN_WIDTH	10


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;

/* State Machine for the USBH_USR_ApplicationState */
#define USH_USR_FS_INIT       0
#define USH_USR_FS_READLIST   1
#define USH_USR_FS_WRITEFILE  2
#define USH_USR_FS_DRAW       3

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages ---------------*/
const char MSG_HOST_INIT[]          = "> Host Library Initialized\n\r";
const char MSG_DEV_ATTACHED[]       = "> Device Attached\n\r";
const char MSG_DEV_DISCONNECTED[]   = "> Device Disconnected\n\r";
const char MSG_DEV_ENUMERATED[]     = "> Enumeration completed\n\r";
const char MSG_DEV_HIGHSPEED[]      = "> High speed device detected\n\r";
const char MSG_DEV_FULLSPEED[]      = "> Full speed device detected\n\r";
const char MSG_DEV_LOWSPEED[]       = "> Low speed device detected\n\r";
const char MSG_DEV_ERROR[]          = "> Device fault \n\r";

const char MSG_MSC_CLASS[]          = "> Mass storage device connected\n\r";
const char MSG_HID_CLASS[]          = "> HID device connected\n\r";

const char USB_HID_MouseStatus[]    = "> HID Demo Device : Mouse\n\r";
const char USB_HID_KeybrdStatus[]   = "> HID Demo Device : Keyboard\n\r";
const char MSG_UNREC_ERROR[]        = "> UNRECOVERED ERROR STATE\n\r";

const char MSG_DISK_SIZE[]        = "> Size of the disk in MBytes: \n\r";
const char MSG_LUN[]              = "> LUN Available in the device: \n\r";
const char MSG_ROOT_CONT[]        = "> Exploring disk flash ... \n\r";
const char MSG_WR_PROTECT[]       = "> The disk is write protected \n\r";


/**
* @}
*/

uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;

/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/
#define KYBRD_FIRST_COLUMN               (uint16_t)319
#define KYBRD_LAST_COLUMN                (uint16_t)7
#define KYBRD_FIRST_LINE                 (uint8_t)120
#define KYBRD_LAST_LINE                  (uint8_t)200


/**
* @}
*/
uint8_t  KeybrdCharXpos           = 0;
uint16_t KeybrdCharYpos           = 0;
extern  int16_t  x_loc, y_loc;


/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Functions
* @{
*/





/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
  static uint8_t startup = 0;

  if(startup == 0 )
  {
    startup = 1;
    /* Configure the LEDs */
    //STM_EVAL_LEDInit(LED4);

		/* init the Debug COM */
    //STM32f4_Discovery_Debug_Init();

#ifdef USE_USB_OTG_HS
    printf(" USB OTG HS HID Host\n\r");
#else
    printf(" USB OTG FS HID Host\n\r");
#endif
    printf("> USB Host library started.\n\r");
    printf ("USB Host Library v2.1.0\n\r" );
  }
}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
  printf (MSG_DEV_ATTACHED);
}

/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
  printf(MSG_UNREC_ERROR);
}

/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected (void)
{
  printf(MSG_DEV_DISCONNECTED);
}

/**
* @brief  USBH_USR_ResetUSBDevice
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    printf(MSG_DEV_HIGHSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    printf(MSG_DEV_FULLSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    printf(MSG_DEV_LOWSPEED);
  }
  else
  {
    printf(MSG_DEV_ERROR);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  uint8_t temp[50];
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;


  sprintf((char *)temp , "VID : %04Xh\n\r" , (uint32_t)(*hs).idVendor);
  printf(temp);


  sprintf((char *)temp , "PID : %04Xh\n\r" , (uint32_t)(*hs).idProduct);
  printf(temp);
}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{

}


/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;

  id = itfDesc;

  if((*id).bInterfaceClass  == 0x08)
  {
    printf(MSG_MSC_CLASS);
  }
  else if((*id).bInterfaceClass  == 0x03)
  {
    printf(MSG_HID_CLASS);
  }
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  char temp[100];
  sprintf(temp, "Manufacturer : %s\n\r", (char *)ManufacturerString);
  printf(temp);

}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  char temp[100];
  sprintf((char *)temp, "Product : %s\n\r", (char *)ProductString);
  printf(temp);

}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  uint8_t temp[100];
  sprintf((char *)temp, "Serial Number : %s\n\r", (char *)SerialNumString);
  printf(temp);
}

/**
* @brief  EnumerationDone
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
  /* Enumeration complete */
  printf((void *)MSG_DEV_ENUMERATED);

  printf("To start the HID class operations: \n\r");
  printf("Press Key...                       \n\r");
}

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
  printf ("> Device not supported.\n\r");

}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{

  USBH_USR_Status usbh_usr_status;

  usbh_usr_status = USBH_USR_NO_RESP;

  /*Key B3 is in polling mode to detect user action */
  /*USER1 B3 is in polling mode to detect user action */
  /*if(Get_Peek_Key() != EOF)
  {
    usbh_usr_status = USBH_USR_RESP_OK;

  }*/

  return usbh_usr_status;

}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected (void)
{
   printf ("Overcurrent detected.\n\r");

}

/**
* @brief  USR_MOUSE_Init
*         Init Mouse window
* @param  None
* @retval None
*/
/*void USR_MOUSE_Init	(void)
{

  printf((void*)USB_HID_MouseStatus);

  HID_MOUSE_ButtonReleased(0);
  HID_MOUSE_ButtonReleased(1);
  HID_MOUSE_ButtonReleased(2);

  x_loc  = 0;
  y_loc  = 0;

}*/

/**
* @brief  USR_MOUSE_ProcessData
*         Process Mouse data
* @param  data : Mouse data to be displayed
* @retval None
*/
/*void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{

  uint8_t idx = 1;
  static uint8_t b_state[3] = { 0, 0 , 0};

  if ((data->x != 0) && (data->y != 0))
  {
    HID_MOUSE_UpdatePosition(data->x , data->y);
  }

  for ( idx = 0 ; idx < 3 ; idx ++)
  {

    if(data->button & 1 << idx)
    {
      if(b_state[idx] == 0)
      {
        HID_MOUSE_ButtonPressed (idx);
        b_state[idx] = 1;
      }
    }
    else
    {
      if(b_state[idx] == 1)
      {
        HID_MOUSE_ButtonReleased (idx);
        b_state[idx] = 0;
      }
    }
  }


}
*/
/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
/*void  USR_KEYBRD_Init (void)
{

  printf((void*)USB_HID_KeybrdStatus);
  printf("> Use Keyboard to tape characters: \n\r");

  KeybrdCharXpos = KYBRD_FIRST_LINE;
  KeybrdCharYpos = KYBRD_FIRST_COLUMN;
}*/


/**
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be displayed
* @retval None
*/
#if 0
void  USR_KEYBRD_ProcessData (uint8_t data)
{


  if(data == '\n')
  {
    KeybrdCharYpos = KYBRD_FIRST_COLUMN;

    /*Increment char X position*/
    KeybrdCharXpos+=SMALL_FONT_LINE_WIDTH;

  }
  else if (data == '\r')
  {
    /* Manage deletion of charactter and upadte cursor location*/
    if( KeybrdCharYpos == KYBRD_FIRST_COLUMN)
    {
      /*First character of first line to be deleted*/
      if(KeybrdCharXpos == KYBRD_FIRST_LINE)
      {
        KeybrdCharYpos =KYBRD_FIRST_COLUMN;
      }
      else
      {
        KeybrdCharXpos-=SMALL_FONT_LINE_WIDTH;
        KeybrdCharYpos =(KYBRD_LAST_COLUMN+SMALL_FONT_COLUMN_WIDTH);
      }
    }
    else
    {
      KeybrdCharYpos +=SMALL_FONT_COLUMN_WIDTH;

    }
		printf("  ");
  }
  else
  {
    printf("%c",data);
    /* Update the cursor position on LCD */

    /*Increment char Y position*/
    KeybrdCharYpos -=SMALL_FONT_COLUMN_WIDTH;

    /*Check if the Y position has reached the last column*/
    if(KeybrdCharYpos == KYBRD_LAST_COLUMN)
    {
      KeybrdCharYpos = KYBRD_FIRST_COLUMN;

      /*Increment char X position*/
      KeybrdCharXpos+=SMALL_FONT_LINE_WIDTH;

    }
  }
}
#endif
/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
}

FATFS fatfs;
FIL file;
/**
* @brief  USBH_USR_MSC_Application
*         Demo application for mass storage
* @param  None
* @retval Staus
*/
int USBH_USR_MSC_Application(void)
{
  FRESULT res;
  uint8_t writeTextBuff[] = "STM32 Connectivity line Host Demo application using FAT_FS   ";
  uint16_t bytesWritten, bytesToWrite;

  switch(USBH_USR_ApplicationState)
  {
  case USH_USR_FS_INIT:

    /* Initialises the File System*/
    if ( f_mount( 0, &fatfs ) != FR_OK ) {
      /* efs initialisation fails*/
    	printf("> Cannot initialize File System.\n");
      return(-1);
    }
    printf("> File System initialized.\n");
    printf("> Disk capacity : %d Bytes\n", USBH_MSC_Param.MSCapacity * \
      USBH_MSC_Param.MSPageLength);

    if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED) {
    	printf(MSG_WR_PROTECT);
    }

    USBH_USR_ApplicationState = USH_USR_FS_READLIST;
    break;

  case USH_USR_FS_READLIST:

	  printf(MSG_ROOT_CONT);
    //Explore_Disk("0:/", 1);
    //line_idx = 0;
    USBH_USR_ApplicationState = USH_USR_FS_WRITEFILE;

    break;

  case USH_USR_FS_WRITEFILE:

    //LCD_SetTextColor(Green);
    //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
    //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key to write file");
    //LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);
    USB_OTG_BSP_mDelay(100);

    /*Key B3 in polling*/
    //while((HCD_IsDeviceConnected(&USB_OTG_Core)))/* && \
      (STM_EVAL_PBGetState (BUTTON_USER) == RESET))
    //{
    //  Toggle_Leds();
    //}*/
    /* Writes a text file, STM32.TXT in the disk*/
    printf("> Writing File to disk flash ...\n");
    if (USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED) {

    	printf ( "> Disk flash is write protected \n");
      USBH_USR_ApplicationState = USH_USR_FS_DRAW;
      break;
    }

    /* Register work area for logical drives */
    f_mount(0, &fatfs);

    if (f_open(&file, "0:STM32.TXT",FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
      /* Write buffer to file */
      bytesToWrite = sizeof(writeTextBuff);
      res= f_write (&file, writeTextBuff, bytesToWrite, (void *)&bytesWritten);

      if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/ {
    	  printf("> STM32.TXT CANNOT be writen.\n");
      } else {
    	  printf("> 'STM32.TXT' file created\n");
      }

      /*close file and filesystem*/
      f_close(&file);
      f_mount(0, NULL);
    } else {
    	printf ("> STM32.TXT created in the disk\n");
    }

    USBH_USR_ApplicationState = USH_USR_FS_DRAW;

    //LCD_SetTextColor(Green);
   // LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
    //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "To start Image slide show Press Key.");
    //LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);

    break;

  case USH_USR_FS_DRAW:

    /*Key B3 in polling*/
    /*while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
      (STM_EVAL_PBGetState (BUTTON_USER) == RESET))
    {
      Toggle_Leds();
    }*/

    while(HCD_IsDeviceConnected(&USB_OTG_Core))
    {
      if ( f_mount( 0, &fatfs ) != FR_OK ) {
        /* fat_fs initialisation fails*/
        return(-1);
      }
      return 0;//Image_Browser("0:/");
    }
    break;
  default: break;
  }
  return(0);
}



USBH_Usr_cb_TypeDef USR_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError

};


void usb_msc_host_init()
{
	  /* Init Host Library */
	  USBH_Init(&USB_OTG_Core,
	#ifdef USE_USB_OTG_FS
	            USB_OTG_FS_CORE_ID,
	#else
	            USB_OTG_HS_CORE_ID,
	#endif
	            &USB_Host,
	            &USBH_MSC_cb,
	            &USR_cb);


}
