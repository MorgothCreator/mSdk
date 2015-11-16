/*
 * usb_dev_msc_interface.c
 *
 *  Created on: Nov 16, 2014
 *      Author: Morgoth
 */

#include "usb_dev_msc_interface.h"


#include "include/cppi41dma.h"
#include "include/hw/hw_usb.h"
#include "include/interrupt.h"
#include "include/usb.h"
#include "interface/usblib/include/usb-ids.h"
#include "interface/usblib/include/usbdevice.h"
#include "interface/usblib/include/usbdmsc.h"

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

#include "sys/sysdelay.h"
#include "aintc/aintc_usb.h"
#include "clk/clk_usb.h"

#include "board_init.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "usb_dev_msc/usb_msc_structs.h"

USBD_DRV_RW_FUNC drv_rw_func[2];
unsigned char *dataBuffer;

//*****************************************************************************
//
// The USB controller instance
//
//*****************************************************************************
#define USB_INSTANCE 0

//*****************************************************************************
//
// The number of ticks to wait before falling back to the idle state.  Since
// the tick rate is 100Hz this is approximately 3 seconds.
//
//*****************************************************************************
#define USBMSC_ACTIVITY_TIMEOUT 30

//*****************************************************************************
//
// DMA Configuration.
//
//*****************************************************************************
#define NUMBER_OF_ENDPOINTS     4 //Total number of send points(RX +TX) used in this USB configuration
#define USB_MSC_BUFER_SIZE      65536

//*****************************************************************************
//
// This enumeration holds the various states that the device can be in during
// normal operation.
//
//*****************************************************************************
volatile enum
{
    //
    // Unconfigured.
    //
    MSC_DEV_DISCONNECTED,

    //
    // Connected but not yet fully enumerated.
    //
    MSC_DEV_CONNECTED,

    //
    // Connected and fully enumerated but not currently handling a command.
    //
    MSC_DEV_IDLE,

    //
    // Currently reading the SD card.
    //
    MSC_DEV_READ,

    //
    // Currently writing the SD card.
    //
    MSC_DEV_WRITE,
}
g_eMSCState;

//*****************************************************************************
//
// The Flags that handle updates to the status area to avoid drawing when no
// updates are required..
//
//*****************************************************************************
#define FLAG_UPDATE_STATUS      1
static unsigned int g_ulFlags;
//static unsigned int g_ulIdleTimeout;
unsigned int g_bufferIndex = 0;
//*****************************************************************************
//
// DMA Configuration.
//
//*****************************************************************************

#ifdef DMA_MODE
/*    endpointInfo epInfo[]=
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
        }

    };*/

#endif
//*****************************************************************************
//
// Handles bulk driver notifications related to the receive channel (data from
// the USB host).
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ulEvent identifies the event we are being notified about.
// \param ulMsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the bulk driver to notify us of any events
// related to operation of the receive data channel (the OUT channel carrying
// data from the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned int
RxHandler(void *pvCBData, unsigned int ulEvent,
               unsigned int ulMsgValue, void *pvMsgData)
{
    return(0);
}

//*****************************************************************************
//
// Handles bulk driver notifications related to the transmit channel (data to
// the USB host).
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ulEvent identifies the event we are being notified about.
// \param ulMsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the bulk driver to notify us of any events
// related to operation of the transmit data channel (the IN channel carrying
// data to the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned int
TxHandler(void *pvCBData, unsigned int ulEvent, unsigned int ulMsgValue,
          void *pvMsgData)
{
    return(0);
}

//*****************************************************************************
//
// Sets up the AINTC Interrupt
//
//*****************************************************************************
static void _USB0AINTCConfigure_(void)
{
    /* Registering the Interrupt Service Routine(ISR). */
    IntRegister(SYS_INT_USB0, USB0DeviceIntHandler);

    /* Setting the priority for the system interrupt in AINTC. */
    IntPrioritySet(SYS_INT_USB0, 1, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the system interrupt in AINTC. */
    IntSystemEnable(SYS_INT_USB0);
}

static void _CPDMAAINTCConfigure_(void)
{

    /* Registering the Interrupt Service Routine(ISR). */
    IntRegister(SYS_INT_USBSSINT, USB0DeviceIntHandler);

    /* Setting the priority for the system interrupt in AINTC. */
    IntPrioritySet(SYS_INT_USBSSINT, 1, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the system interrupt in AINTC. */
    IntSystemEnable(SYS_INT_USBSSINT);
}
static void _USBInterruptEnable_(void)
{
    /* Configuring AINTC to receive USB interrupts. */
    _USB0AINTCConfigure_();

    _CPDMAAINTCConfigure_();
}

//*****************************************************************************
//
// This function is the call back notification function provided to the USB
// library's mass storage class.
//
//*****************************************************************************
unsigned int
USBDMSCEventCallback(void *pvCBData, unsigned int ulEvent,
                     unsigned int ulMsgParam, void *pvMsgData)
{
    //
    // Reset the time out every time an event occurs.
    //
   // g_ulIdleTimeout = USBMSC_ACTIVITY_TIMEOUT;

    switch(ulEvent)
    {
        //
        // Writing to the device.
        //
        case USBD_MSC_EVENT_WRITING:
        {
            //
            // Only update if this is a change.
            //
            if(g_eMSCState != MSC_DEV_WRITE)
            {
                //
                // Go to the write state.
                //
                g_eMSCState = MSC_DEV_WRITE;

                //
                // Cause the main loop to update the screen.
                //
                g_ulFlags |= FLAG_UPDATE_STATUS;
            }

            break;
        }

        //
        // Reading from the device.
        //
        case USBD_MSC_EVENT_READING:
        {
            //
            // Only update if this is a change.
            //
            if(g_eMSCState != MSC_DEV_READ)
            {
                //
                // Go to the read state.
                //
                g_eMSCState = MSC_DEV_READ;

                //
                // Cause the main loop to update the screen.
                //
                g_ulFlags |= FLAG_UPDATE_STATUS;
            }

            break;
        }
        case USBD_MSC_EVENT_IDLE:
        default:
        {
            break;
        }
    }

    return(0);
}

void _usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present)
{
	//tUSBDMSCMediaStatus eMediaStatus = USBDMSC_MEDIA_UNKNOWN;
	if(media_is_present == true)  USBDevConnect(g_USBInstance[0].uiBaseAddr);//eMediaStatus = USBDMSC_MEDIA_PRESENT;
	else if(media_is_present == false) USBDevDisconnect(g_USBInstance[0].uiBaseAddr);//eMediaStatus = USBDMSC_MEDIA_NOTPRESENT;
	//USBDMSCMediaChange((tUSBDMSCDevice *)&g_sMSCDevice, eMediaStatus);
}

void _usb_msc_dev_init(unsigned int instance, void *slave_controls)
{
    memcpy(&drv_rw_func[instance], slave_controls, sizeof(USBD_DRV_RW_FUNC));
	//
    //USB module clock enable
    //
    if(instance == 0)
    {
    	USB0ModuleClkConfig();
    //
    //USB Interrupt enable
    //
    	_USBInterruptEnable_();
    }

    //
    // Initialize the idle timeout and reset all flags.
    //
    //g_ulIdleTimeout = 0;
    g_ulFlags = 0;

    //
    // Initialize the state to idle.
    //
    g_eMSCState = MSC_DEV_IDLE;

    USBDMSCInit(instance, (tUSBDMSCDevice *)&g_sMSCDevice);

#ifdef DMA_MODE
    Cppi41DmaInit(instance, epInfo, NUMBER_OF_ENDPOINTS);

    for(;g_bufferIndex < NUM_OF_RX_BDs; g_bufferIndex++)
    {
        dataBuffer = (unsigned char *)cppiDmaAllocBuffer();
        doDmaRxTransfer(instance, USB_MSC_BUFER_SIZE, dataBuffer,
                            g_sMSCDevice.psPrivateData->ucOUTEndpoint);
    }
#endif

}
