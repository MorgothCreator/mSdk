/*
 * usb_mouse_interface.c
 *
 *  Created on: Nov 13, 2014
 *      Author: Morgoth
 */


#include "usb_host_mouse_interface.h"

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
#include "interface/lcd_interface.h"

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


#include "api/gpio_api.h"

#include "lib/fs/fat.h"

#include "sys/sysdelay.h"
#include "aintc/aintc_usb.h"
#include "clk/clk_usb.h"

#include "board_init.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/touchscreen_def.h"
#include "lib/fat_fs/inc/ff.h"
#include "lib/gfx/controls_definition.h"

new_screen* ScreenRander;

unsigned int g_ulButtons;
int MouseXPosition;
int MouseYPosition;
int MouseWheel;
unsigned char MouseClkDrLastState;
volatile bool MousePositionChange = false;
int MouseState;

//*****************************************************************************
//
// The global value used to store the mouse instance value.
//
//*****************************************************************************
static unsigned int g_ulMouseInstance;
//*****************************************************************************
//
// The size of the mouse device interface's memory pool in bytes.
//
//*****************************************************************************
#define MOUSE_MEMORY_SIZE       128

//*****************************************************************************
//
// The memory pool to provide to the mouse device.
//
//*****************************************************************************
unsigned char g_pucMouseBuffer[MOUSE_MEMORY_SIZE];
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
// Declare the USB Events driver interface.
//
//*****************************************************************************
DECLARE_EVENT_DRIVER(g_sUSBMouseEventDriver, 0, 0, USBHCDEvents);

//*****************************************************************************
//
// The global that holds all of the host drivers in use in the application.
// In this case, only the Mouse class is loaded.
//
//*****************************************************************************
static tUSBHostClassDriver const * const g_ppHostClassDrivers[] =
{
    &g_USBHIDClassDriver
    ,&g_sUSBMouseEventDriver
};
//*****************************************************************************
//
// This global holds the number of class drivers in the g_ppHostClassDrivers
// list.
//
//*****************************************************************************
static const unsigned int g_ulNumHostClassDrivers =
    sizeof(g_ppHostClassDrivers) / sizeof(tUSBHostClassDriver *);
//*****************************************************************************
//
// This enumerated type is used to hold the states of the mouse.
//
//*****************************************************************************
typedef enum
{
    //
    // No device is present.
    //
    STATE_MOUSE_NO_DEVICE,

    //
    // Mouse has been detected and needs to be initialized in the main
    // loop.
    //
    STATE_MOUSE_INIT,

    //
    // Mouse is connected and waiting for events.
    //
    STATE_MOUSE_CONNECTED,

    //
    // An unsupported device has been attached.
    //
    STATE_MOUSE_UNKNOWN_DEVICE,

    //
    // A power fault has occured.
    //
    STATE_MOUSE_POWER_FAULT,
}
eUSBMouseState;
volatile eUSBMouseState eMouseState;
//*****************************************************************************
//
// This function updates the cursor position based on deltas received from
// the mouse device.
//
// \param iXDelta is the signed movement in the X direction.
// \param iYDelta is the signed movement in the Y direction.
//
// This function is called by the mouse handler code when it detects a change
// in the position of the mouse.  It will take the inputs and force them
// to be constrained to the display area of the screen.  If the left mouse
// button is pressed then the mouse will draw on the screen and if it is not
// it will move around normally.  A side effect of not being able to read the
// current state of the screen is that the cursor will erase anything it moves
// over while the left mouse button is not pressed.
//
// \return None.
//
//*****************************************************************************
void
UpdateCursor(int iXDelta, int iYDelta, int iWheel)
{
	MouseXPosition += iXDelta;
	if(MouseXPosition < 0) MouseXPosition = 0;
	if(MouseXPosition >= ScreenRander->raster_timings->X) MouseXPosition = ScreenRander->raster_timings->X - 1;
	MouseYPosition += iYDelta;
	if(MouseYPosition < 0) MouseYPosition = 0;
	if(MouseYPosition >= ScreenRander->raster_timings->Y) MouseYPosition = ScreenRander->raster_timings->Y - 1;
	MouseWheel += iWheel;
}
//*****************************************************************************
//
// This is the callback from the USB HID mouse handler.
//
// \param pvCBData is ignored by this function.
// \param ulEvent is one of the valid events for a mouse device.
// \param ulMsgParam is defined by the event that occurs.
// \param pvMsgData is a pointer to data that is defined by the event that
// occurs.
//
// This function will be called to inform the application when a mouse has
// been plugged in or removed and anytime mouse movement or button pressed
// is detected.
//
// \return This function will return 0.
//
//*****************************************************************************
unsigned int
MouseCallback(void *pvCBData, unsigned int ulEvent, unsigned int ulMsgParam,
              void *pvMsgData)
{
    switch(ulEvent)
    {
        //
        // New mouse detected.
        //
        case USB_EVENT_CONNECTED:
        {

            //
            // Proceed to the STATE_MOUSE_INIT state so that the main loop can
            // finish initialized the mouse since USBHMouseInit() cannot be
            // called from within a callback.
            //
        	eMouseState = STATE_MOUSE_INIT;

            break;
        }

        //
        // Mouse has been unplugged.
        //
        case USB_EVENT_DISCONNECTED:
        {
			//
            // Change the state so that the main loop knows that the mouse is
            // no longer present.
            //
        	eMouseState = STATE_MOUSE_NO_DEVICE;

            //
            // Reset the button state.
            //
            g_ulButtons = 0;

            break;
        }

        //
        // Mouse button press detected.
        //
        case USBH_EVENT_HID_MS_PRESS:
        {
            //
            // Save the new button that was pressed.
            //
            g_ulButtons |= ulMsgParam;

            break;
        }

        //
        // Mouse button release detected.
        //
        case USBH_EVENT_HID_MS_REL:
        {
            //
            // Remove the button from the pressed state.
            //
            g_ulButtons &= ~ulMsgParam;

            break;
        }

        //
        // Mouse X movement detected.
        //
        case USBH_EVENT_HID_MS_X:
        {
            //
            // Update the cursor on the screen.
            //
           	UpdateCursor((signed char)ulMsgParam, 0, 0);
           	MousePositionChange = true;

            break;
        }

        //
        // Mouse Y movement detected.
        //
        case USBH_EVENT_HID_MS_Y:
        {
            //
            // Update the cursor on the screen.
            //
           	UpdateCursor(0, (signed char)ulMsgParam, 0);

           	MousePositionChange = true;
            break;
        }
    }
    //
    // Update the status area of the screen.
    //
   	//UpdateStatus();

    return(0);
}


void _usb_mouse_host_init(unsigned int instance)
{
	MouseXPosition = 0;
	MouseYPosition = 0;
    if(instance == 0)
    {
		//g_eMouseState0 = STATE_MOUSE_NO_DEVICE;
		//g_eMouseUIState0 = STATE_MOUSE_NO_DEVICE;
	    //
	    // Enable Clocking to the USB controller.
	    //
	    USB0ModuleClkConfig();
    }
    else
    {
		//g_eMouseState1 = STATE_MOUSE_NO_DEVICE;
		//g_eMouseUIState1 = STATE_MOUSE_NO_DEVICE;
    }
    //
    //Setup the AINT Controller
    //
    USBInterruptEnable(instance);
	//
    // Register the host class drivers.
    //
    USBHCDRegisterDrivers(USB_INSTANCE_Host_Mouse, g_ppHostClassDrivers, g_ulNumHostClassDrivers);
    //
    // Open an instance of the mouse driver.  The mouse does not need
    // to be present at this time, this just saves a place for it and allows
    // the applications to be notified when a mouse is present.
    //
    g_ulMouseInstance = USBHMouseOpen(USB_INSTANCE_Host_Mouse, MouseCallback, g_pucMouseBuffer, MOUSE_MEMORY_SIZE);

    //
    // Initialize the power configuration. This sets the power enable signal
    // to be active high and does not enable the power fault.
    //
    USBHCDPowerConfigInit(USB_INSTANCE_Host_Mouse, USBHCD_VBUS_AUTO_HIGH);

    //
    // Initialize the host controller stack.
    //
    USBHCDInit(USB_INSTANCE_Host_Mouse, g_pHCDPool, MOUSE_MEMORY_SIZE);
    //
    // Call the main loop for the Host controller driver.
    //
    USBHCDMain(USB_INSTANCE_Host_Mouse, g_ulMouseInstance);
}

void _usb_mouse_host_idle(unsigned int instance, tControlCommandData *control_comand)
{
	MouseState = 0;
	switch(eMouseState)
    {
        //
        // This state is entered when the mouse is first detected.
        //
        case STATE_MOUSE_INIT:
        {
            //
            // Initialize the newly connected mouse.
            //
            USBHMouseInit(g_ulMouseInstance);

            //
            // Proceed to the mouse connected state.
            //
            eMouseState = STATE_MOUSE_CONNECTED;
            MouseState = 1;

            //
            // Update the status on the screen.
            //
            //UpdateStatus();

            break;
        }
        case STATE_MOUSE_CONNECTED:
        {
            //
            // Nothing is currently done in the main loop when the mouse
            // is connected.
            //
        	MouseState = 2;
            break;
        }
        case STATE_MOUSE_NO_DEVICE:
        {
            //
            // The mouse is not connected so nothing needs to be done here.
            //
        	MouseState = 3;
            break;
        }
        default:
        {
            break;
        }
    }
    USBHCDMain(USB_INSTANCE_Host_Mouse, g_ulMouseInstance);
	control_comand->X = MouseXPosition;
	control_comand->Y = MouseYPosition;
	if(MouseClkDrLastState == Cursor_NoAction && (g_ulButtons & 0x1))
	{
		MouseClkDrLastState = Cursor_Down;
		control_comand->Cursor = Cursor_Down;
	}
	else if((MouseClkDrLastState == Cursor_Down || MouseClkDrLastState == Cursor_Move) && (g_ulButtons & 0x1))
	{
		MouseClkDrLastState = Cursor_Move;
		control_comand->Cursor = Cursor_Move;
	}
	else if((MouseClkDrLastState == Cursor_Down || MouseClkDrLastState == Cursor_Move) && (~g_ulButtons & 0x1))
	{
		MouseClkDrLastState = Cursor_Up;
		control_comand->Cursor = Cursor_Up;
	}
	else if(MouseClkDrLastState == Cursor_Up && (~g_ulButtons & 0x1))
	{
		MouseClkDrLastState = Cursor_NoAction;
		control_comand->Cursor = Cursor_NoAction;
	}
	if(MousePositionChange)
	{
		control_comand->WindowRefresh |= true;
		MousePositionChange = false;
	}
}

