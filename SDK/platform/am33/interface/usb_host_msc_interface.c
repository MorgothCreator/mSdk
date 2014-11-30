/*
 * usb_interface.c
 *
 *  Created on: Dec 25, 2012
 *      Author: XxXx
 */
#include "usb_host_msc_interface.h"

#include "include/cppi41dma.h"

#include "interface/usblib/include/usblib.h"
#include "interface/usblib/include/usbhhid.h"
#include "interface/usblib/include/usbhost.h"
#include "interface/usblib/include/usbhid.h"
#include "interface/usblib/include/usbhhid.h"
#include "interface/usblib/include/usbhhidmouse.h"
#include "interface/usblib/include/usbhmsc.h"
#include "interface/usblib/include/usbmsc.h"
#include "interface/usblib/include/usblibpriv.h"
#include "interface/usblib/include/usbdevice.h"

#ifdef HEADER_INCLUDE_C_FILES
#include "interface/usblib/usbbuffer.c"
#include "interface/usblib/usbdata.c"
#include "interface/usblib/usbdesc.c"
#include "interface/usblib/usbkeyboardmap.c"
#include "interface/usblib/usbringbuf.c"
#include "interface/usblib/usbtick.c"
#include "interface/usblib/host/usbhhid.c"
#include "interface/usblib/host/usbhhidkeyboard.c"
#include "interface/usblib/host/usbhhidmouse.c"
#include "interface/usblib/host/usbhmsc.c"
#include "interface/usblib/host/usbhostenum.c"
#include "interface/usblib/host/usbhscsi.c"
#endif

#include "driver/cpu.h"

#include "api/gpio_api.h"

#include "lib/fs/fat.h"

#include "sys/sysdelay.h"
#include "aintc/aintc_usb.h"
#include "clk/clk_usb.h"

#include "board_init.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "lib/fat_fs/inc/ff.h"

extern new_uart* DebugCom;
new_gpio *LedStatusUsb0 = NULL;
new_gpio *LedStatusUsb1 = NULL;

void USBHCDEvents(void *pvData);

//*****************************************************************************
//
// This buffer holds the full path to the current working directory.  Initially
// it is root ("/").
//
//*****************************************************************************
#define PATH_BUF_SIZE   4
static char g_cCwdBuf1[PATH_BUF_SIZE] = "3:/";
static char g_cCwdBuf2[PATH_BUF_SIZE] = "4:/";

FATFS g_sFatFs1;
FATFS g_sFatFs2;
DIR g_sDirObject;

//*****************************************************************************
//
// A macro that holds the number of result codes.
//
//*****************************************************************************
//#define NUM_FRESULT_CODES (sizeof(g_sFresultStrings) / sizeof(tFresultString))

//*****************************************************************************
//
// The size of the host controller's memory pool in bytes.
//
//*****************************************************************************
#define HCD_MEMORY_SIZE         128

//*****************************************************************************
//
// The memory pool to provide to the Host controller driver.
//
//*****************************************************************************
unsigned char g_pHCDPool[HCD_MEMORY_SIZE];

//*****************************************************************************
//
// The instance data for the MSC driver.
//
//*****************************************************************************
unsigned int g_ulMSCInstance0Usb0 = 0;//UsbMsc driver
unsigned int g_ulMSCInstance0Usb1 = 0;//UsbMsc driver

unsigned int g_ulMSCInstance1Usb0 = 0;//UsbMouse driver
unsigned int g_ulMSCInstance1Usb1 = 0;//UsbMouse driver



/*tUSBHTimeOut *USBHTimeOut0 = NULL;
tUSBHTimeOut *USBHTimeOut1 = NULL;
unsigned int deviceRetryOnTimeOut0 = USBMSC_DRIVE_RETRY;
unsigned int deviceRetryOnTimeOut1 = USBMSC_DRIVE_RETRY;*/
//*****************************************************************************
//
// Declare the USB Events driver interface.
//
//*****************************************************************************
DECLARE_EVENT_DRIVER(g_sUSBEventDriver, 0, 0, USBHCDEvents);

//*****************************************************************************
//
// The global that holds all of the host drivers in use in the application.
// In this case, only the MSC class is loaded.
//
//*****************************************************************************
static tUSBHostClassDriver const * const g_ppHostClassDriversMsc[] =
{
    &g_USBHostMSCClassDriver,
	//&g_USBHIDClassDriver,
    &g_sUSBEventDriver
};
//*****************************************************************************
//
// This global holds the number of class drivers in the g_ppHostClassDrivers
// list.
//
//*****************************************************************************
#define NUM_CLASS_DRIVERS       (sizeof(g_ppHostClassDriversMsc) /               \
                                 sizeof(g_ppHostClassDriversMsc[0]))


//*****************************************************************************
//
// Hold the current state for the application.
//
//*****************************************************************************
typedef enum
{
    //
    // No device is present.
    //
    STATE_NO_DEVICE,

    //
    // Mass storage device is being enumerated.
    //
    STATE_DEVICE_ENUM,

    //
    // Mass storage device is ready.
    //
    STATE_DEVICE_READY,

    //
    // An unsupported device has been attached.
    //
    STATE_UNKNOWN_DEVICE,

    //
    // A power fault has occurred.
    //
    STATE_POWER_FAULT,


}
tState;
volatile tState g_eState0;
volatile tState g_eUIState0;
volatile tState g_eState1;
volatile tState g_eUIState1;


//*****************************************************************************
//
// The current USB operating mode - Host, Device or unknown.
//
//*****************************************************************************
tUSBMode g_eCurrentUSBMode;

#ifdef DMA_MODE

endpointInfo epInfo[]=
{
	{
		USB_EP_TO_INDEX(USB_EP_1),
		CPDMA_DIR_RX,
		CPDMA_MODE_SET_TRANSPARENT,
	},

	{
		USB_EP_TO_INDEX(USB_EP_1),
		CPDMA_DIR_TX,
		CPDMA_MODE_SET_GRNDIS,
	},

	{
		USB_EP_TO_INDEX(USB_EP_2),
		CPDMA_DIR_RX,
		CPDMA_MODE_SET_TRANSPARENT,
	},

	{
		USB_EP_TO_INDEX(USB_EP_2),
		CPDMA_DIR_TX,
		CPDMA_MODE_SET_GRNDIS,
	}

};

#define NUMBER_OF_ENDPOINTS 4

#endif

//*****************************************************************************
//
// This is the generic callback from host stack.
//
// \param pvData is actually a pointer to a tEventInfo structure.
//
// This function will be called to inform the application when a USB event has
// occurred that is outside those related to the mass storage device.  At this
// point this is used to detect unsupported devices being inserted and removed.
// It is also used to inform the application when a power fault has occurred.
// This function is required when the g_USBGenericEventDriver is included in
// the host controller driver array that is passed in to the
// USBHCDRegisterDrivers() function.
//
// \return None.
//
//*****************************************************************************
void USBHCDEvents(void *pvData)
{
    tEventInfo *pEventInfo;

    //
    // Cast this pointer to its actual type.
    //
    pEventInfo = (tEventInfo *)pvData;

    if(pEventInfo->ulInstance == g_ulMSCInstance0Usb1 || pEventInfo->ulInstance == g_ulMSCInstance1Usb1)
    {
		switch(pEventInfo->ulEvent)
		{
			//
			// New keyboard detected.
			//
			case USB_EVENT_CONNECTED:
			{
				//
				// An unknown device was detected.
				//
				g_eState1 = STATE_UNKNOWN_DEVICE;

				break;
			}

			//
			// Keyboard has been unplugged.
			//
			case USB_EVENT_DISCONNECTED:
			{
				//
				// Unknown device has been removed.
				//
				g_eState1 = STATE_NO_DEVICE;

				break;
			}

			case USB_EVENT_POWER_FAULT:
			{
				//
				// No power means no device is present.
				//
				g_eState1 = STATE_POWER_FAULT;

				break;
			}

			default:
			{
				break;
			}
		}
    }
    else if(pEventInfo->ulInstance == g_ulMSCInstance0Usb0 || pEventInfo->ulInstance == g_ulMSCInstance1Usb0)
    {
		switch(pEventInfo->ulEvent)
		{
			//
			// New keyboard detected.
			//
			case USB_EVENT_CONNECTED:
			{
				//
				// An unknown device was detected.
				//
				g_eState0 = STATE_UNKNOWN_DEVICE;

				break;
			}

			//
			// Keyboard has been unplugged.
			//
			case USB_EVENT_DISCONNECTED:
			{
				//
				// Unknown device has been removed.
				//
				g_eState0 = STATE_NO_DEVICE;

				break;
			}

			case USB_EVENT_POWER_FAULT:
			{
				//
				// No power means no device is present.
				//
				g_eState0 = STATE_POWER_FAULT;

				break;
			}

			default:
			{
				break;
			}
		}
    }
}

//*****************************************************************************
//
// This is the callback from the MSC driver.
//
// \param ulInstance is the driver instance which is needed when communicating
// with the driver.
// \param ulEvent is one of the events defined by the driver.
// \param pvData is a pointer to data passed into the initial call to register
// the callback.
//
// This function handles callback events from the MSC driver.  The only events
// currently handled are the MSC_EVENT_OPEN and MSC_EVENT_CLOSE.  This allows
// the main routine to know when an MSC device has been detected and
// enumerated and when an MSC device has been removed from the system.
//
// \return Returns \e true on success or \e false on failure.
//
//*****************************************************************************
void
MSCCallback(unsigned int ulInstance, unsigned int ulEvent, void *pvData)
{
    if(ulInstance == g_ulMSCInstance0Usb1)
    {
		//
		// Determine the event.
		//
		switch(ulEvent)
		{
			//
			// Called when the device driver has successfully enumerated an MSC
			// device.
			//
			case MSC_EVENT_OPEN:
			{
				//
				// Proceed to the enumeration state.
				//
				g_eState1 = STATE_DEVICE_ENUM;
				break;
			}

			//
			// Called when the device driver has been unloaded due to error or
			// the device is no longer present.
			//
			case MSC_EVENT_CLOSE:
			{
				//
				// Go back to the "no device" state and wait for a new connection.
				//
				g_eState1 = STATE_NO_DEVICE;

				break;
			}

			default:
			{
				break;
			}
		}
    }
    else if(ulInstance == g_ulMSCInstance0Usb0)
    {
		//
		// Determine the event.
		//
		switch(ulEvent)
		{
			//
			// Called when the device driver has successfully enumerated an MSC
			// device.
			//
			case MSC_EVENT_OPEN:
			{
				//
				// Proceed to the enumeration state.
				//
				g_eState0 = STATE_DEVICE_ENUM;
				break;
			}

			//
			// Called when the device driver has been unloaded due to error or
			// the device is no longer present.
			//
			case MSC_EVENT_CLOSE:
			{
				//
				// Go back to the "no device" state and wait for a new connection.
				//
				g_eState0 = STATE_NO_DEVICE;

				break;
			}

			default:
			{
				break;
			}
		}
    }
}


unsigned int USBMSCReadBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	unsigned int status = CPUIntStatus();
	CPUirqe();
	if(LedStatusUsb0) gpio_out(LedStatusUsb0, 1);
	if(USBHMSCBlockRead((unsigned int)_ctrl, block, ptr, nblks) == 0)
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		if(status & 0x80) CPUirqd();
		return 1;
	}
	else
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		if(status & 0x80) CPUirqd();
		return 0;
	}
}

unsigned int USBMSCWriteBlock(void *_ctrl, void *ptr, unsigned long block,
                              unsigned int nblks)
{
	unsigned int status = CPUIntStatus();
	CPUirqe();
	if(LedStatusUsb0) gpio_out(LedStatusUsb0, 1);
	if(USBHMSCBlockWrite((unsigned int)_ctrl, block, ptr, nblks) == 0)
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		if(status & 0x80) CPUirqd();
		return 1;
	}
	else
	{
		if(LedStatusUsb0) gpio_out(LedStatusUsb0, 0);
		if(status & 0x80) CPUirqd();
		return 0;
	}

}

void _usb_msc_host_ioctl(void *ctrl, unsigned int  command,  unsigned int *buffer)
{
	tUSBHMSCInstance * _ctrl = (tUSBHMSCInstance *)ctrl;
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           *buffer = _ctrl->ulNumBlocks;
            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = _ctrl->ulBlockSize;

            break;
        }
        default:
        {
            *buffer = 0;
            break;
        }

    }
}



void _usb_msc_host_init(unsigned int instance, new_gpio* StatusLed)
{
    if(instance == 0)
    {
		g_eState0 = STATE_NO_DEVICE;
		g_eUIState0 = STATE_NO_DEVICE;
	    //
	    // Enable Clocking to the USB controller.
	    //
	    USB0ModuleClkConfig();
    }
    else
    {
		g_eState1 = STATE_NO_DEVICE;
		g_eUIState1 = STATE_NO_DEVICE;
    }

    if(instance) LedStatusUsb1 = StatusLed;
    else LedStatusUsb0 = StatusLed;
    //
    //Delay timer setup
    //
    //DelayTimerSetup();


    //
    //Setup the AINT Controller
    //
    USBInterruptEnable(instance);
    //
    // Register the host class drivers.
    //
    USBHCDRegisterDrivers(instance, g_ppHostClassDriversMsc, NUM_CLASS_DRIVERS);
    //
    // Register the host class drivers.
    //
    //USBHCDRegisterDrivers(instance, g_ppHostClassDriversMouse, g_ulNumHostClassDrivers);
    //
    // Open an instance of the mass storage class driver.
    //
    if(instance) g_ulMSCInstance0Usb1 = USBHMSCDriveOpen(instance, 1, MSCCallback);
    else g_ulMSCInstance0Usb0 = USBHMSCDriveOpen(instance, 0, MSCCallback);

    //if(instance) g_ulMSCInstance1Usb1 = USBHMouseOpen(instance, MouseCallbackUsb1, g_pucMouseBuffer, MOUSE_MEMORY_SIZE);
    //else g_ulMSCInstance1Usb0 = USBHMouseOpen(instance, MouseCallbackUsb0, g_pucMouseBuffer, MOUSE_MEMORY_SIZE);


    //
    // Initialize the power configuration.  This sets the power enable signal
    // to be active high and does not enable the power fault.
    //
    USBHCDPowerConfigInit(instance, USBHCD_VBUS_AUTO_HIGH);

#ifdef DMA_MODE
    Cppi41DmaInit(instance, epInfo, NUMBER_OF_ENDPOINTS);
#endif

	/*SET_CONNECT_RETRY(instance, 4);

    if(instance == 0) USBHCDTimeOutHook(instance, &USBHTimeOut0);
    else USBHCDTimeOutHook(instance, &USBHTimeOut1);*/
    //
    // Initialize the host controller.
    //
    USBHCDInit(instance, g_pHCDPool, HCD_MEMORY_SIZE);
}

tState eStateCopy0;
tState eStateCopy1;
extern FileInfo_t *FILE1;

void _usb_msc_host_idle(unsigned int instance)
{
    if(instance == 1 && (g_ulMSCInstance0Usb1 != 0 || g_ulMSCInstance1Usb1 != 0))
    {
    	//unsigned int ulPrompt;
    	//
    	// See if a mass storage device has been enumerated.
    	//
    	if(g_eState1 == STATE_DEVICE_ENUM)
    	{
    		//UARTprintf(DebugCom, "\nDevice enumerate.\n");
    	    //
    	    // Take it easy on the Mass storage device if it is slow to
    	    // start up after connecting.
    	    //
    	    if(USBHMSCDriveReady(g_ulMSCInstance0Usb1) != 0)
    	    {
    	    	//
    	    	// Wait about 100ms before attempting to check if the
    	    	// device is ready again.
    	    	//
    	    	Sysdelay(100);

    	        return;
    	    }

            //deviceRetryOnTimeOut1 = USBMSC_DRIVE_RETRY;
#ifndef thirdpartyfatfs
    		Drives_Table[8] = new_(new_fat_disk);
    		Drives_Table[8]->DiskInfo_SdDriverStructAddr = (void*)g_ulMSCInstance0Usb1;
    		//Drives_Table[4]->drive_init = MMCSD_CardInit;
    		Drives_Table[8]->drive_read_page = USBMSCReadBlock;
    		Drives_Table[8]->drive_write_page = USBMSCWriteBlock;
    		if(_Fat_Mount(8))
    		{
																	UARTPuts(DebugCom,   "USB1 drive 8 mounted\n\r" , -1);
																	UARTPuts(DebugCom,   "USB1 Fat fs detected\n\r" , -1);
    			if(Drives_Table[8]->DiskInfo_FatType == NoFs ){ 	UARTprintf(DebugCom, "USB1 Fs type:                 None\n\r");}
    			if(Drives_Table[8]->DiskInfo_FatType == Fat12){ 	UARTprintf(DebugCom, "USB1 Fs type:                 Fat12\n\r");}
    			if(Drives_Table[8]->DiskInfo_FatType == Fat16){ 	UARTprintf(DebugCom, "USB1 Fs type:                 Fat16\n\r");}
    			if(Drives_Table[8]->DiskInfo_FatType == Fat32){ 	UARTprintf(DebugCom, "USB1 Fs type:                 Fat32\n\r");}
    																UARTprintf(DebugCom, "USB1 BootSectorAddress:       %u \n\r",Drives_Table[8]->DiskInfo_BootSectorAddress);
    																UARTprintf(DebugCom, "USB1 BytesPerSector:          %d \n\r",Drives_Table[8]->DiskInfo_BytesPerSector);
    																UARTprintf(DebugCom, "USB1 SectorsPerCluster:       %d \n\r",Drives_Table[8]->DiskInfo_SectorsPerCluster);
    																UARTprintf(DebugCom, "USB1 AllocTable1Begin:        %u \n\r",Drives_Table[8]->DiskInfo_AllocTable1Begin);
    																UARTprintf(DebugCom, "USB1 NumberOfFats:            %d \n\r",Drives_Table[8]->DiskInfo_NumberOfFats);
    																UARTprintf(DebugCom, "USB1 MediaType:               %d \n\r",Drives_Table[8]->DiskInfo_MediaType);
    																UARTprintf(DebugCom, "USB1 AllocTableSize:          %u \n\r",Drives_Table[8]->DiskInfo_AllocTableSize);
    																UARTprintf(DebugCom, "USB1 DataSectionBegin:        %d \n\r",Drives_Table[8]->DiskInfo_DataSectionBegin);
    			if(Drives_Table[8]->DiskInfo_FatType == Fat32)		UARTprintf(DebugCom, "USB1 DiskCapacity:            %uMB\n\r",(unsigned long long)((unsigned long long)Drives_Table[8]->DiskInfo_AllocTableSize * 128 * (unsigned long long)Drives_Table[8]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[8]->DiskInfo_BytesPerSector) / 1000000);
    			else if(Drives_Table[8]->DiskInfo_FatType == Fat16)	UARTprintf(DebugCom, "USB1 DiskCapacity:            %uMB\n\r",(unsigned long long)((unsigned long long)Drives_Table[8]->DiskInfo_AllocTableSize * 256 * (unsigned long long)Drives_Table[8]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[8]->DiskInfo_BytesPerSector) / 1000000);
    			FILE1 = _FatData_OpenSesion(8);
    			if(FILE1) UARTPuts(DebugCom, "Fat File Sesion Init OK\n\r", -1);
    			else UARTPuts(DebugCom, "Fat File Sesion Init ERROR\n\r", -1);
    		}
    		else 													UARTPuts(DebugCom,   "USB1 Fat not detected\n\r" , -1);
#else
            g_sFatFs2.drv_rw_func.DriveStruct = (void*)g_ulMSCInstance0Usb1;
            g_sFatFs2.drv_rw_func.drv_r_func = USBMSCReadBlock;
            g_sFatFs2.drv_rw_func.drv_w_func = USBMSCWriteBlock;
            if(!f_mount(4, &g_sFatFs2))
            {
#ifdef usb_host_msc_debug
            	if(f_opendir(&g_sDirObject, g_cCwdBuf2) == FR_OK)
                {
					if(DebugCom)
					{
																			UARTPuts(DebugCom,   "USB1 drive 4 mounted\n\r" , -1);
																			UARTPuts(DebugCom,   "USB1 Fat fs detected\n\r" , -1);
																			UARTprintf(DebugCom, "USB1 Fs type:                 ");
						if(g_sFatFs2.fs_type == FS_FAT12)	{ 				UARTprintf(DebugCom, "Fat12");}
						else if(g_sFatFs2.fs_type == FS_FAT16){ 				UARTprintf(DebugCom, "Fat16");}
						else if(g_sFatFs2.fs_type == FS_FAT32){ 				UARTprintf(DebugCom, "Fat32");}
						else								{ 				UARTprintf(DebugCom, "None");}
																			UARTprintf(DebugCom, "\n\r");
																			//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
																			UARTprintf(DebugCom, "USB1 BytesPerSector:          %d \n\r",/*(int)g_sFatFs.s_size*/512);
																			UARTprintf(DebugCom, "USB1 SectorsPerCluster:       %d \n\r",(int)g_sFatFs2.csize);
																			//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																			UARTprintf(DebugCom, "USB1 NumberOfFats:            %d \n\r",(int)g_sFatFs2.n_fats);
																			//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																			//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																			UARTprintf(DebugCom, "USB1 DataSectionBegin:        %d \n\r",(int)g_sFatFs2.fatbase);
																			UARTprintf(DebugCom, "USB1 uSD DiskCapacity:        %uMB\n\r",(unsigned long)((unsigned long long)((unsigned long long)g_sFatFs2.max_clust * (unsigned long long)g_sFatFs2.csize * (unsigned long long)/*g_sFatFs.s_size*/512) / 1000000));
					}
                } else  if(DebugCom)										UARTPuts(DebugCom,   "USB1 ERROR oppening path\n\r" , -1);
#endif
            }else if(DebugCom)												UARTPuts(DebugCom,   "USB1 ERROR mounting disk\n\r" , -1);
#endif
    	    g_eState1 = STATE_DEVICE_READY;
    	    //}
    	}
    	//
    	// See if the state has changed.  We make a copy of g_eUIState0 to
    	// prevent a compiler warning about undefined order of volatile
    	// accesses.
    	//
        /*if((USBHTimeOut1->Status.slEP0)||
            (USBHTimeOut1->Status.slNonEP0))
        {
            deviceRetryOnTimeOut1--;
        }

        if(!deviceRetryOnTimeOut1)
        {
            g_eState1 = STATE_TIMEDOUT;
        	deviceRetryOnTimeOut1 = USBMSC_DRIVE_RETRY;
            USBHTimeOut1->Value.slEP0 = USB_EP0_TIMEOUT_MILLISECS;
        	USBHTimeOut1->Value.slNonEP0= USB_NONEP0_TIMEOUT_MILLISECS;
        	USBHTimeOut1->Status.slEP0 = 0;
            USBHTimeOut1->Status.slNonEP0= 0;
        }*/

        eStateCopy1 = g_eUIState1;
    	if(g_eState1 != eStateCopy1)
    	{
    	    //
    	    // Determine the new state.
    	    //
    	    switch((int)g_eState1)
    	    {
    	        //
    	        // A previously connected device has been disconnected.
    	        //
    	        case STATE_NO_DEVICE:
    	        {
    	            if(g_eUIState1 == STATE_UNKNOWN_DEVICE)
    	            {
    	                UARTprintf(DebugCom, "\nUnknown device disconnected.\n");
    	            }
    	            else
    	            {
#ifdef usb_host_msc_debug
#ifndef thirdpartyfatfs
    					UARTPuts(DebugCom, "USB1 Fat dismount\n\r" , -1);
    					_Fat_Unmount(8);
    					UARTPuts(DebugCom, "USB1 Fat dismount OK\n\r" , -1);
#endif
    	            	UARTprintf(DebugCom, "\nMass storage device disconnected.\n");
#endif
    	            }
    	         //ulPrompt = 1;
    	         break;
    	         }

    	         //
    	         // A mass storage device is being enumerated.
    	         //
    	         case STATE_DEVICE_ENUM:
    	         {
    	        	 UARTprintf(DebugCom, "\nDevice enumerate.\n");
    	        	 break;
    	         }

    	         //
    	         // A mass storage device has been enumerated and initialized.
    	         //
    	         case STATE_DEVICE_READY:
    	         {
    	             UARTprintf(DebugCom, "\nMass storage device connected.\n");
    	             //ulPrompt = 1;
    	             break;
    	         }

    	         //
    	         // An unknown device has been connected.
    	         //
    	         case STATE_UNKNOWN_DEVICE:
    	         {
    	             UARTprintf(DebugCom, "\nUnknown device connected.\n");
    	             //ulPrompt = 1;
    	             break;
    	         }

    	         //
    	         // A power fault has occurred.
    	         //
    	         case STATE_POWER_FAULT:
    	         {
    	             UARTprintf(DebugCom, "\nPower fault.\n");
    	             //ulPrompt = 1;
    	             break;
    	          }
    	      }

    	      //
    	      // Save the current state.
    	      //
    	      g_eUIState1 = g_eState1;
    	 }
        USBHCDMain(instance, g_ulMSCInstance0Usb1);
    }
    else if (instance == 0 && g_ulMSCInstance0Usb0 != 0)
    {
		//unsigned int ulPrompt;
		//
		// See if a mass storage device has been enumerated.
		//
		if(g_eState0 == STATE_DEVICE_ENUM)
		{
			//
			// Take it easy on the Mass storage device if it is slow to
			// start up after connecting.
			//
			if(USBHMSCDriveReady(g_ulMSCInstance0Usb0) != 0)
			{
				//
				// Wait about 100ms before attempting to check if the
				// device is ready again.
				//
				Sysdelay(100);

				return;
			}

#ifndef thirdpartyfatfs
            //deviceRetryOnTimeOut0 = USBMSC_DRIVE_RETRY;
			Drives_Table[4] = new_(new_fat_disk);
			Drives_Table[4]->DiskInfo_SdDriverStructAddr = (void*)g_ulMSCInstance0Usb0;
			//Drives_Table[4]->drive_init = MMCSD_CardInit;
			Drives_Table[4]->drive_read_page = USBMSCReadBlock0;
			Drives_Table[4]->drive_write_page = USBMSCWriteBlock0;
			if(_Fat_Mount(4))
			{
																	UARTPuts(DebugCom,   "USB0 drive 4 mounted\n\r" , -1);
																	UARTPuts(DebugCom,   "USB0 Fat fs detected\n\r" , -1);
				if(Drives_Table[4]->DiskInfo_FatType == NoFs ){ 	UARTprintf(DebugCom, "USB0 Fs type:                 None\n\r");}
				if(Drives_Table[4]->DiskInfo_FatType == Fat12){ 	UARTprintf(DebugCom, "USB0 Fs type:                 Fat12\n\r");}
				if(Drives_Table[4]->DiskInfo_FatType == Fat16){ 	UARTprintf(DebugCom, "USB0 Fs type:                 Fat16\n\r");}
				if(Drives_Table[4]->DiskInfo_FatType == Fat32){ 	UARTprintf(DebugCom, "USB0 Fs type:                 Fat32\n\r");}
																	UARTprintf(DebugCom, "USB0 BootSectorAddress:       %u \n\r",Drives_Table[4]->DiskInfo_BootSectorAddress);
																	UARTprintf(DebugCom, "USB0 BytesPerSector:          %d \n\r",Drives_Table[4]->DiskInfo_BytesPerSector);
																	UARTprintf(DebugCom, "USB0 SectorsPerCluster:       %d \n\r",Drives_Table[4]->DiskInfo_SectorsPerCluster);
																	UARTprintf(DebugCom, "USB0 AllocTable1Begin:        %u \n\r",Drives_Table[4]->DiskInfo_AllocTable1Begin);
																	UARTprintf(DebugCom, "USB0 NumberOfFats:            %d \n\r",Drives_Table[4]->DiskInfo_NumberOfFats);
																	UARTprintf(DebugCom, "USB0 MediaType:               %d \n\r",Drives_Table[4]->DiskInfo_MediaType);
																	UARTprintf(DebugCom, "USB0 AllocTableSize:          %u \n\r",Drives_Table[4]->DiskInfo_AllocTableSize);
																	UARTprintf(DebugCom, "USB0 DataSectionBegin:        %d \n\r",Drives_Table[4]->DiskInfo_DataSectionBegin);
				if(Drives_Table[4]->DiskInfo_FatType == Fat32)		UARTprintf(DebugCom, "USB0 DiskCapacity:            %uMB\n\r",(unsigned long long)((unsigned long long)Drives_Table[4]->DiskInfo_AllocTableSize * 128 * (unsigned long long)Drives_Table[4]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[4]->DiskInfo_BytesPerSector) / 1000000);
				else if(Drives_Table[4]->DiskInfo_FatType == Fat16)	UARTprintf(DebugCom, "USB0 DiskCapacity:            %uMB\n\r",(unsigned long long)((unsigned long long)Drives_Table[4]->DiskInfo_AllocTableSize * 256 * (unsigned long long)Drives_Table[4]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[4]->DiskInfo_BytesPerSector) / 1000000);
				FILE1 = _FatData_OpenSesion(4);
				if(FILE1) UARTPuts(DebugCom, "Fat File Sesion Init OK\n\r", -1);
				else UARTPuts(DebugCom, "Fat File Sesion Init ERROR\n\r", -1);
			}
			else 													UARTPuts(DebugCom,   "USB0 Fat not detected\n\r" , -1);
#else
            g_sFatFs1.drv_rw_func.DriveStruct = (void*)g_ulMSCInstance0Usb0;
            g_sFatFs1.drv_rw_func.drv_r_func = USBMSCReadBlock;
            g_sFatFs1.drv_rw_func.drv_w_func = USBMSCWriteBlock;
            if(!f_mount(3, &g_sFatFs1))
            {
#ifdef usb_host_msc_debug
                if(f_opendir(&g_sDirObject, g_cCwdBuf1) == FR_OK)
                {
					if(DebugCom)
					{
																			UARTPuts(DebugCom,   "USB0 drive 3 mounted\n\r" , -1);
																			UARTPuts(DebugCom,   "USB0 Fat fs detected\n\r" , -1);
																			UARTprintf(DebugCom, "USB0 Fs type:                 ");
						if(g_sFatFs1.fs_type == FS_FAT12)	{ 				UARTprintf(DebugCom, "Fat12");}
						else if(g_sFatFs1.fs_type == FS_FAT16){ 				UARTprintf(DebugCom, "Fat16");}
						else if(g_sFatFs1.fs_type == FS_FAT32){ 				UARTprintf(DebugCom, "Fat32");}
						else								{ 				UARTprintf(DebugCom, "None");}
																			UARTprintf(DebugCom, "\n\r");
																			//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
																			UARTprintf(DebugCom, "USB0 BytesPerSector:          %d \n\r",/*(int)g_sFatFs.s_size*/512);
																			UARTprintf(DebugCom, "USB0 SectorsPerCluster:       %d \n\r",(int)g_sFatFs1.csize);
																			//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																			UARTprintf(DebugCom, "USB0 NumberOfFats:            %d \n\r",(int)g_sFatFs1.n_fats);
																			//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																			//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																			UARTprintf(DebugCom, "USB0 DataSectionBegin:        %d \n\r",(int)g_sFatFs1.fatbase);
																			UARTprintf(DebugCom, "USB0 uSD DiskCapacity:        %uMB\n\r",(unsigned long)((unsigned long long)((unsigned long long)g_sFatFs1.max_clust * (unsigned long long)g_sFatFs1.csize * (unsigned long long)/*g_sFatFs.s_size*/512) / 1000000));
					}
                } else  if(DebugCom)										UARTPuts(DebugCom,   "USB0 ERROR oppening path\n\r" , -1);
#endif
            }else if(DebugCom)												UARTPuts(DebugCom,   "USB0 ERROR mounting disk\n\r" , -1);
#endif
			g_eState0 = STATE_DEVICE_READY;
			//}
		}
        /*if((USBHTimeOut0->Status.slEP0)||
            (USBHTimeOut0->Status.slNonEP0))
        {
            deviceRetryOnTimeOut0--;
        }

        if(!deviceRetryOnTimeOut0)
        {
            g_eState0 = STATE_TIMEDOUT;
        	deviceRetryOnTimeOut0 = USBMSC_DRIVE_RETRY;
            USBHTimeOut0->Value.slEP0 = USB_EP0_TIMEOUT_MILLISECS;
        	USBHTimeOut0->Value.slNonEP0= USB_NONEP0_TIMEOUT_MILLISECS;
        	USBHTimeOut0->Status.slEP0 = 0;
            USBHTimeOut0->Status.slNonEP0= 0;
        }*/

		//
		// See if the state has changed.  We make a copy of g_eUIState0 to
		// prevent a compiler warning about undefined order of volatile
		// accesses.
		//
		eStateCopy0 = g_eUIState0;
		if(g_eState0 != eStateCopy0)
		{
		    //
		    // Determine the new state.
		    //
		    switch((int)g_eState0)
		    {
		        //
		        // A previously connected device has been disconnected.
		        //
		        case STATE_NO_DEVICE:
		        {
		            if(g_eUIState0 == STATE_UNKNOWN_DEVICE)
		            {
		                UARTprintf(DebugCom, "\nUnknown device disconnected.\n");
		            }
		            else
		            {
#ifdef usb_host_msc_debug
#ifndef thirdpartyfatfs
						UARTPuts(DebugCom, "USB0 Fat dismount\n\r" , -1);
						_Fat_Unmount(4);
						UARTPuts(DebugCom, "USB0 Fat dismount OK\n\r" , -1);
#endif
		            	UARTprintf(DebugCom, "\nMass storage device disconnected.\n");
#endif
		            }
		         //ulPrompt = 1;
		         break;
		         }

		         //
		         // A mass storage device is being enumerated.
		         //
		         case STATE_DEVICE_ENUM:
		         {
		        	 UARTprintf(DebugCom, "\nDevice enumerate.\n");
		        	 break;
		         }

		         //
		         // A mass storage device has been enumerated and initialized.
		         //
		         case STATE_DEVICE_READY:
		         {
		             UARTprintf(DebugCom, "\nMass storage device connected.\n");
		             //ulPrompt = 1;
		             break;
		         }

		         //
		         // An unknown device has been connected.
		         //
		         case STATE_UNKNOWN_DEVICE:
		         {
		             UARTprintf(DebugCom, "\nUnknown device connected.\n");
		             //ulPrompt = 1;
		             break;
		         }

		         //
		         // A power fault has occurred.
		         //
		         case STATE_POWER_FAULT:
		         {
		             UARTprintf(DebugCom, "\nPower fault.\n");
		             //ulPrompt = 1;
		             break;
		          }
		      }

		      //
		      // Save the current state.
		      //
		      g_eUIState0 = g_eState0;
		 }
	    USBHCDMain(instance, g_ulMSCInstance0Usb0);
    }
}
