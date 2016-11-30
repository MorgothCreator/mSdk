/*
 * usb_mouse_interface.c
 *
 *  Created on: Nov 13, 2014
 *      Author: Morgoth
 */


#include "usb_host_mouse_interface.h"
#include "driver/USBH/Class/HID/Inc/usbh_hid.h"
#include "driver/USBH/Class/HID/Inc/usbh_hid_keybd.h"
#include "driver/USBH/Class/HID/Inc/usbh_hid_mouse.h"

USBH_HandleTypeDef usb_mouse_host_param;
unsigned char MouseClkDrLastState;
unsigned int mouse_old_position_x = -1;
unsigned int mouse_old_position_y = -1;

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_DISCONNECT,
  APPLICATION_START,
  APPLICATION_READY,
  APPLICATION_RUNNING,
}HID_ApplicationTypeDef;

HID_ApplicationTypeDef usb_host_mouse_state = APPLICATION_IDLE;
/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
	  usb_host_mouse_state = APPLICATION_DISCONNECT;
    break;

  //case HOST_USER_CLASS_ACTIVE:
	//  usb_host_mouse_state = APPLICATION_READY;
   // break;

  case HOST_USER_CLASS_SELECTED:
	  usb_host_mouse_state = APPLICATION_READY;
	  //USBH_HID_InterfaceInit(phost);
	  USBH_HID_KeybdInit(phost);
	  USBH_HID_MouseInit(phost);
	  USBH_ReEnumerate(phost);
     break;


  case HOST_USER_CONNECTION:
	  usb_host_mouse_state = APPLICATION_START;
    break;

  default:
    break;
  }
}


void _usb_mouse_host_init(unsigned int instance)
{
	/* Init Host Library */
	USBH_Init(&usb_mouse_host_param, USBH_UserProcess, 0);

	/* Add Supported Class */
	USBH_RegisterClass(&usb_mouse_host_param, USBH_HID_CLASS);

	/* Start Host Process */
	USBH_Start(&usb_mouse_host_param);

	//USBH_HID_MouseInit(&usb_mouse_host_param);
}

void _usb_mouse_host_idle(unsigned int instance, tControlCommandData *control_comand)
{
	//if(!usb_mouse_host_param.pActiveClass)
		//return;
    USBH_Process(&usb_mouse_host_param);
	if(usb_host_mouse_state != APPLICATION_READY)
		return;

    HID_MOUSE_Info_TypeDef *mouse_info= USBH_HID_GetMouseInfo(&usb_mouse_host_param);
    if(mouse_info)
    {
    	control_comand->X = mouse_info->x;
    	control_comand->Y = mouse_info->y;
    	if(MouseClkDrLastState == Cursor_NoAction && (mouse_info->buttons[0]))
    	{
    		MouseClkDrLastState = Cursor_Down;
    		control_comand->Cursor = Cursor_Down;
    	}
    	else if((MouseClkDrLastState == Cursor_Down || MouseClkDrLastState == Cursor_Move) && (mouse_info->buttons[0]))
    	{
    		MouseClkDrLastState = Cursor_Move;
    		control_comand->Cursor = Cursor_Move;
    	}
    	else if((MouseClkDrLastState == Cursor_Down || MouseClkDrLastState == Cursor_Move) && (~mouse_info->buttons[0]))
    	{
    		MouseClkDrLastState = Cursor_Up;
    		control_comand->Cursor = Cursor_Up;
    	}
    	else if(MouseClkDrLastState == Cursor_Up && (~mouse_info->buttons[0]))
    	{
    		MouseClkDrLastState = Cursor_NoAction;
    		control_comand->Cursor = Cursor_NoAction;
    	}
    	if(mouse_old_position_x != mouse_info->x || mouse_old_position_y != mouse_info->y)
    	{
    		mouse_old_position_x = mouse_info->x;
    		mouse_old_position_y = mouse_info->y;
    		control_comand->WindowRefresh |= true;
    	}
    }
}

