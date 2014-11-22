/*
 * usb_mouse_interface.c
 *
 *  Created on: Nov 13, 2014
 *      Author: Morgoth
 */


#include "usb_host_mouse_interface.h"
#include "api/gpio_api.h"

#include "lib/fs/fat.h"

#include "sys/sysdelay.h"

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


void _usb_mouse_host_init(unsigned int instance)
{
	MouseXPosition = 0;
	MouseYPosition = 0;
}

void _usb_mouse_host_idle(unsigned int instance, tControlCommandData *control_comand)
{
	MouseState = 0;
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

