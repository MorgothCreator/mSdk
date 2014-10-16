/*
 *  lib/gfx/picturebox.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdbool.h>
#include "picturebox.h"
#include "window_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/timer_api.h"
#include "3d.h"
#include "controls_definition.h"
//#######################################################################################
static void paint_picturebox(tPictureBox* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand)
{
	unsigned int color = 0;
	tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + x_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	color = controls_color.Control_Color_Enabled_Border_Pull;
	if(!settings->Enabled || !ParentWindow->Internals.OldStateEnabled) color = settings->Color.Disabled.Border;
	put_rectangle(pDisplay, x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
	put_rectangle(pDisplay, x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
	color = settings->BackgroundColor;
	if(!settings->Enabled || !ParentWindow->Internals.OldStateEnabled) color = settings->Color.Disabled.Buton;
	//color = controls_color.Control_Color_Enabled_Buton_Pull;
	if(settings->PaintBackground) put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
	if(settings->Events.Refresh.CallBack)
	{
		tRectangle clip;
		clip.sXMin = x_start + 2;
		clip.sXMax = x_start + x_len - 2;
		clip.sYMin = y_start + 2;
		clip.sYMax = y_start + y_len - 2;
		clip_limit(&pDisplay->sClipRegion, &clip);
		settings->Internals.PictureWindowLimits = pDisplay->sClipRegion;
		signed int X = control_comand->X;
		signed int Y = control_comand->Y;
		control_comand->X -= settings->Internals.Position.X + 2;
		control_comand->Y -= settings->Internals.Position.Y + 2;
		settings->Events.Refresh.CallbackReturnData = settings->Events.Refresh.CallBack(settings, control_comand);
		control_comand->X = X;
		control_comand->Y = Y;
	}
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + x_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	box_cache_clean(pDisplay, x_start, y_start, x_len, y_len);
	pDisplay->sClipRegion = back_up_clip;
	control_comand->WindowRefresh |= true;
}
//#######################################################################################
void picturebox(tPictureBox *settings, tControlCommandData* control_comand)
{
	if(settings == NULL) return;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse commands */
#ifdef NO_ENUM_ON_SWITCH
		switch((unsigned char)control_comand->Comand)
#else
		switch((int)control_comand->Comand)
#endif
		{
		case Control_Entire_Repaint:
			settings->Internals.NeedEntireRepaint = true;
			return;
		case Control_Entire_Refresh:
			settings->Internals.NeedEntireRefresh = true;
			return;
		case Control_Refresh:
			settings->Internals.NeedEntireRefresh = true;
			break;
		case Control_Set_Position_X:
			settings->Position.X = (signed int)control_comand->Data;
			return;
		case Control_Set_Position_Y:
			settings->Position.Y = (signed int)control_comand->Data;
			return;
		case Control_Set_Size_X:
			settings->Size.X = (signed int)control_comand->Data;
			return;
		case Control_Set_Size_Y:
			settings->Size.X = (signed int)control_comand->Data;
			return;
		case Control_Set_Enabled:
			settings->Enabled = (bool)control_comand->Data;
			return;
		case Control_Set_Visible:
			settings->Visible = (bool)control_comand->Data;
			return;
		}
	}
	tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	if(settings->Internals.Control.Initiated == false)
	{
		if(ParentWindow)
		{
			settings->Internals.Position.X = settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X;
			settings->Internals.Position.Y = settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y;
		}
		else
		{
			settings->Internals.Position.X = settings->Position.X;
			settings->Internals.Position.Y = settings->Position.Y;
		}
		settings->Internals.Size.X = settings->Size.X;
		settings->Internals.Size.Y = settings->Size.Y;
	}
	/* Verify if position on size has been modified */
	if(ParentWindow)
	{
		if((settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X) != settings->Internals.Position.X ||
				(settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y) != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.OldStateEnabled != settings->Enabled ||
								ParentWindow->Internals.OldStateEnabled != settings->Internals.ParentWindowStateEnabled)
											settings->Internals.NeedEntireRefresh = true;
	}
	else
	{
		if(settings->Position.X != settings->Internals.Position.X ||
				settings->Position.Y != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.OldStateEnabled != settings->Enabled)
											settings->Internals.NeedEntireRefresh = true;
	}

	signed int X_StartBox = settings->Internals.Position.X;
	signed int Y_StartBox = settings->Internals.Position.Y;
	signed int X_LenBox = settings->Internals.Size.X;
	signed int Y_LenBox = settings->Internals.Size.Y;
	tDisplay *pDisplay = settings->Internals.pDisplay;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
	/*if(settings->Internals.NeedEntireRefresh == true || (settings->Internals.OldStateVisible != settings->Visible && settings->Visible == false))
	{
		settings->Internals.OldStateVisible = settings->Visible;
		if(settings->Events.Refresh.CallBack)
		{
			tRectangle back_up_clip = pDisplay->sClipRegion;
			tRectangle clip;
			clip.sXMin = X_StartBox + 2;
			clip.sXMax = (X_StartBox + X_LenBox) - 2;
			clip.sYMin = Y_StartBox + 2;
			clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
			clip_limit(&pDisplay->sClipRegion, &clip);
			settings->Internals.PictureWindowLimits = pDisplay->sClipRegion;
			signed int X = control_comand->X;
			signed int Y = control_comand->Y;
			control_comand->X -= settings->Position.X + 2;
			control_comand->Y -= settings->Position.Y + 2;
			settings->Events.Refresh.CallbackReturnData = settings->Events.Refresh.CallBack(settings, control_comand);
			control_comand->X = X;
			control_comand->Y = Y;
			box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			pDisplay->sClipRegion = back_up_clip;
		}
	}*/
	if(settings->Internals.NeedEntireRefresh == true || settings->Internals.OldStateVisible != settings->Visible)
	{
		if(!settings->Internals.NoPaintBackGround || !settings->Visible)
		{
			settings->Internals.OldStateVisible = settings->Visible;
			tRectangle back_up_clip = pDisplay->sClipRegion;
			pDisplay->sClipRegion.sXMin = X_StartBox;
			pDisplay->sClipRegion.sYMin = Y_StartBox;
			pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
			pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip);
			put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, settings->Color.Scren);
			box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			pDisplay->sClipRegion = back_up_clip;
			control_comand->WindowRefresh |= true;
			if(!settings->Visible) return;
		}
		settings->Internals.NeedEntireRefresh = true;
	}


	/* Verify if is Entire refresh, entire repaint, or state changed */
	if((settings->Internals.NeedEntireRefresh == true ||
			settings->Internals.NeedEntireRepaint == true ||
				settings->Internals.Control.Initiated == false) &&
							settings->Visible == true)
	{
		/* Copy new locations and dimensions to actual locations and dimensions */
		if(ParentWindow)
		{
			settings->Internals.Position.X = settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X;
			settings->Internals.Position.Y = settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y;
		}
		else
		{
			settings->Internals.Position.X = settings->Position.X;
			settings->Internals.Position.Y = settings->Position.Y;
		}
		settings->Internals.Size.X = settings->Size.X;
		settings->Internals.Size.Y = settings->Size.Y;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		paint_picturebox(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		settings->Internals.ParentWindowStateEnabled = ParentWindow->Internals.OldStateEnabled;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.OldStateEnabled = settings->Enabled;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		//control_comand->Cursor = cursor;
		//control_comand->WindowRefresh |= true;
		return;
	}
	/* Check if inside window */
	bool inside_window = check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;
	if((control_comand->Cursor) &&
			//settings->Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || settings->Internals.CursorDownInsideBox == true) &&
					settings->Enabled == true &&
						settings->Visible == true &&
							control_comand->CursorCoordonateUsed == false)
	{
		//settings->Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window == true)
		{
			settings->Events.CursorDown = true;
			settings->Internals.CursorDownInsideBox = true;
			if(settings->Events.OnDown.CallBack)
			{
				tRectangle back_up_clip = pDisplay->sClipRegion;
				tRectangle clip;
				clip.sXMin = X_StartBox + 2;
				clip.sXMax = (X_StartBox + X_LenBox) - 2;
				clip.sYMin = Y_StartBox + 2;
				clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
				clip_limit(&pDisplay->sClipRegion, &clip);
				settings->Internals.PictureWindowLimits = pDisplay->sClipRegion;
				signed int X = control_comand->X;
				signed int Y = control_comand->Y;
				control_comand->X -= settings->Internals.Position.X + 2;
				control_comand->Y -= settings->Internals.Position.Y + 2;
				settings->Events.OnDown.CallbackReturnData = settings->Events.OnDown.CallBack(settings, control_comand);
				control_comand->X = X;
				control_comand->Y = Y;
				box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
				pDisplay->sClipRegion = back_up_clip;
				control_comand->WindowRefresh |= true;
			}
		}
		if(control_comand->Cursor == Cursor_Up && settings->Internals.CursorDownInsideBox == true && inside_window == true)
		{
			settings->Events.CursorUp = true;
			if(settings->Events.OnUp.CallBack)
			{
				tRectangle back_up_clip = pDisplay->sClipRegion;
				tRectangle clip;
				clip.sXMin = X_StartBox + 2;
				clip.sXMax = (X_StartBox + X_LenBox) - 2;
				clip.sYMin = Y_StartBox + 2;
				clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
				clip_limit(&pDisplay->sClipRegion, &clip);
				settings->Internals.PictureWindowLimits = pDisplay->sClipRegion;
				signed int X = control_comand->X;
				signed int Y = control_comand->Y;
				control_comand->X -= settings->Internals.Position.X + 2;
				control_comand->Y -= settings->Internals.Position.Y + 2;
				settings->Events.OnUp.CallbackReturnData = settings->Events.OnUp.CallBack(settings, control_comand);
				control_comand->X = X;
				control_comand->Y = Y;
				box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
				pDisplay->sClipRegion = back_up_clip;
				control_comand->WindowRefresh |= true;
			}
		}
		//paint_picturebox(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		if(control_comand->Cursor == Cursor_Move && settings->Internals.CursorDownInsideBox == true && inside_window == true)
		{
			settings->Events.CursorMove = true;
			if(settings->Events.OnMove.CallBack)
			{
				tRectangle back_up_clip = pDisplay->sClipRegion;
				tRectangle clip;
				clip.sXMin = X_StartBox + 2;
				clip.sXMax = (X_StartBox + X_LenBox) - 2;
				clip.sYMin = Y_StartBox + 2;
				clip.sYMax = (Y_StartBox + Y_LenBox) - 2;
				clip_limit(&pDisplay->sClipRegion, &clip);
				settings->Internals.PictureWindowLimits = pDisplay->sClipRegion;
				signed int X = control_comand->X;
				signed int Y = control_comand->Y;
				control_comand->X -= settings->Internals.Position.X + 2;
				control_comand->Y -= settings->Internals.Position.Y + 2;
				settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings, control_comand);
				control_comand->X = X;
				control_comand->Y = Y;
				box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
				pDisplay->sClipRegion = back_up_clip;
				control_comand->WindowRefresh |= true;
			}
		}
	}
	if(control_comand->Cursor && settings->Internals.CursorDownInsideBox) control_comand->CursorCoordonateUsed |= true;
	if(settings->Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction)) settings->Internals.CursorDownInsideBox = false;
	//control_comand->CursorCoordonateUsed |= settings->Internals.CursorDownInsideBox;
	//control_comand->WindowRefresh |= true;
	return;
}
//#######################################################################################
tPictureBox *new_picturebox(void *ParentWindow)
{
	tPictureBox* settings = (tPictureBox*)calloc(1, sizeof(tPictureBox));

	if(!settings || !ParentWindow) return NULL;
	settings->Internals.ParentWindow = ParentWindow;

	tWindow *_ParentWindow = (tWindow *)ParentWindow;
	settings->Internals.pDisplay = _ParentWindow->Internals.pDisplay;

	settings->Color.Scren = controls_color.Scren;
	settings->Color.Enabled.BackGround = controls_color.Control_Color_Enabled_BackGround;
	settings->Color.Enabled.Border.Pull = controls_color.Control_Color_Enabled_Border_Pull;
	settings->Color.Enabled.Border.Push = controls_color.Control_Color_Enabled_Border_Push;
	settings->Color.Enabled.Buton.Pull = controls_color.Control_Color_Enabled_Buton_Pull;
	settings->Color.Enabled.Buton.Push = controls_color.Control_Color_Enabled_Buton_Push;
	settings->Color.Enabled.Ink.Pull = controls_color.Control_Color_Enabled_Ink_Pull;
	settings->Color.Enabled.Ink.Push = controls_color.Control_Color_Enabled_Ink_Push;

	settings->Color.Disabled.BackGround = controls_color.Control_Color_Disabled_BackGround;
	settings->Color.Disabled.Border = controls_color.Control_Color_Disabled_Border_Pull;
	settings->Color.Disabled.Buton = controls_color.Control_Color_Disabled_Buton_Pull;
	settings->Color.Disabled.Ink = controls_color.Control_Color_Disabled_Ink_Pull;

	settings->Enabled = true;
	settings->Position.X = 10;
	settings->Position.Y = 10;
	settings->Size.X = 60;
	settings->Size.Y = 20;
	settings->Visible = true;
	settings->BackgroundColor = ClrWhite;
	return settings;
}
//#######################################################################################
bool free_picturebox(tPictureBox* settings)
{
	if(!settings) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	picturebox(settings, &comand);
	if(settings) free(settings);
	return true;
}
//#######################################################################################
void picturebox_clear(tPictureBox* settings)
{
	tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	signed int X_StartBox = settings->Internals.Position.X;
	signed int Y_StartBox = settings->Internals.Position.Y;
	signed int X_LenBox = settings->Internals.Size.X;
	signed int Y_LenBox = settings->Internals.Size.Y;
	pDisplay->sClipRegion.sXMin = X_StartBox;
	pDisplay->sClipRegion.sYMin = Y_StartBox;
	pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
	pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	unsigned int color = settings->BackgroundColor;
	if(!settings->Enabled || !ParentWindow->Internals.OldStateEnabled) color = settings->Color.Disabled.Buton;
	put_rectangle(pDisplay, X_StartBox + 2, Y_StartBox + 2, X_LenBox - 4, Y_LenBox - 4, true, color);
	box_cache_clean(pDisplay, X_StartBox + 2, Y_StartBox + 2, X_LenBox - 4, Y_LenBox - 4);
	pDisplay->sClipRegion = back_up_clip;
}
/*//#######################################################################################
void picturebox_copy_rectangle(tPictureBox* settings, unsigned int *src_buff, signed int src_x_buff_size, signed int src_y_buff_size, signed int src_x_offset, signed int src_y_offset)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	//put_pixel(pDisplay, X + settings->Position.X + 2, Y + settings->Position.Y + 2, color);

	signed int Y_cnt = 0;
	signed int X_len = (pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin);
	if(X_len <= 0) return;
	for(; Y_cnt < src_y_offset + (pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin); Y_cnt++)
	{
		if(Y_cnt + pDisplay->sClipRegion.sYMin >= pDisplay->sClipRegion.sYMax) break;
		memcpy((void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), (void *)((char *)(src_buff + ((Y_cnt + src_y_offset) * src_x_buff_size)) - 1), X_len * sizeof(pDisplay->DisplayData[0]));
		CacheDataCleanInvalidateBuff((unsigned int)(void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), X_len * sizeof(pDisplay->DisplayData[0]) + 64);
	}
	pDisplay->sClipRegion = back_up_clip;
}*/
//#######################################################################################
void picturebox_copy_rectangle(tPictureBox* settings, unsigned int *src_buff, unsigned int dest_buff_data_offset, unsigned int src_buff_data_offset, tRectangle *_dest_rectangle, tRectangle *_src_rectangle, signed int src_width, signed int src_height)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	//tRectangle back_up_clip = pDisplay->sClipRegion;
	//pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	////put_pixel(pDisplay, X + settings->Position.X + 2, Y + settings->Position.Y + 2, color);

	tRectangle *dest_rectangle = _dest_rectangle;
	tRectangle *src_rectangle = _src_rectangle;

	signed int dest_X_StartBox = settings->Internals.Position.X;
	signed int dest_Y_StartBox = settings->Internals.Position.Y;
	//signed int dest_X_EndBox = settings->Internals.Position.X + settings->Internals.Size.X;
	//signed int dest_Y_EndBox = settings->Internals.Position.Y + settings->Internals.Size.Y;

	//Calculate the destination locations on the screen.
	dest_rectangle->sXMin += dest_X_StartBox;
	dest_rectangle->sXMax += dest_X_StartBox;
	dest_rectangle->sYMin += dest_Y_StartBox;
	dest_rectangle->sYMax += dest_Y_StartBox;
	if(dest_rectangle->sXMax < settings->Internals.Position.X + 2 ||
			dest_rectangle->sYMax < settings->Internals.Position.Y + 2 ||
				dest_rectangle->sXMin >= settings->Internals.Position.X + (settings->Internals.Size.X - 4) ||
					dest_rectangle->sYMin >= settings->Internals.Position.Y + (settings->Internals.Size.Y - 4)) return;

	//Limit the destination area to the picture box size.
	/*if(dest_rectangle->sXMin < dest_X_StartBox) dest_rectangle->sXMin = dest_X_StartBox;
	if(dest_rectangle->sYMin < dest_Y_StartBox) dest_rectangle->sYMin = dest_Y_StartBox;
	if(dest_rectangle->sXMax >= dest_X_EndBox) dest_rectangle->sXMax = dest_X_EndBox;
	if(dest_rectangle->sYMax >= dest_Y_EndBox) dest_rectangle->sYMax = dest_Y_EndBox;*/

	clip_limit(dest_rectangle, &settings->Internals.PictureWindowLimits);

	signed int x_line_len = dest_rectangle->sXMax - dest_rectangle->sXMin;

	if(x_line_len <= 0) return;
	if(dest_X_StartBox <= dest_rectangle->sXMin) dest_X_StartBox = dest_rectangle->sXMin;

	signed int Y_cnt = dest_Y_StartBox;
	if(Y_cnt <= dest_rectangle->sYMin) Y_cnt = dest_rectangle->sYMin;

	signed int X_Start_Src_Buff = (settings->Internals.Position.X + 2) - settings->Internals.PictureWindowLimits.sXMin;
	if(X_Start_Src_Buff > 0) X_Start_Src_Buff = 0;
	else X_Start_Src_Buff = (~X_Start_Src_Buff) + 1;
	signed int Y_Start_Src_Buff = (settings->Internals.Position.Y + 2) - settings->Internals.PictureWindowLimits.sYMin;
	if(Y_Start_Src_Buff > 0) Y_Start_Src_Buff = 0;
	else Y_Start_Src_Buff = (~Y_Start_Src_Buff) + 1;

	for(; Y_cnt < dest_rectangle->sYMax; Y_cnt++)
	{
		//if(Y_cnt >= pDisplay->sClipRegion.sYMax) break;
		memcpy((void *)(pDisplay->DisplayData + dest_buff_data_offset + (Y_cnt * pDisplay->Width) + dest_X_StartBox), (void *)((char *)(src_buff + src_buff_data_offset + (((Y_cnt - settings->Internals.PictureWindowLimits.sYMin + Y_Start_Src_Buff) + src_rectangle->sYMin) * src_width) + src_rectangle->sXMin + X_Start_Src_Buff) - 1), x_line_len * sizeof(pDisplay->DisplayData[0]));
		//CacheDataCleanInvalidateBuff((unsigned int)(void *)(pDisplay->DisplayData + 8 + ((Y_cnt + settings->Position.Y + 2) * pDisplay->Width) + settings->Position.X + 2), X_len * sizeof(pDisplay->DisplayData[0]) + 64);
	}
	//pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_pixel(tPictureBox* settings, signed int X, signed int Y, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_pixel(pDisplay, X + settings->Internals.Position.X + 2, Y + settings->Internals.Position.Y + 2, color);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_horizontal_line(tPictureBox* settings, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_horizontal_line(pDisplay, X1 + settings->Internals.Position.X + 2, X2 + settings->Internals.Position.X + 2, Y + settings->Internals.Position.Y + 2, width, color);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_vertical_line(tPictureBox* settings, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_vertical_line(pDisplay, Y1 + settings->Internals.Position.Y + 2, Y2 + settings->Internals.Position.Y + 2, X + settings->Internals.Position.X + 2, width, color);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_circle(tPictureBox* settings, signed int x, signed int y, signed int radius, unsigned char fill, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_circle(pDisplay, x + settings->Internals.Position.X + 2, y + settings->Internals.Position.Y + 2, radius, fill, color);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_line(tPictureBox* settings, signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_line(pDisplay, X1 + settings->Internals.Position.X + 2, Y1 + settings->Internals.Position.Y + 2, X2 + settings->Internals.Position.X + 2, Y2 + settings->Internals.Position.Y + 2, width, color);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_elipse(tPictureBox* settings, signed int xc,signed int yc,signed int rx,signed int ry, unsigned char fill, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_elipse(pDisplay, xc + settings->Internals.Position.X + 2, yc + settings->Internals.Position.Y + 2, rx + settings->Internals.Position.X + 2, ry + settings->Internals.Position.Y + 2, fill, color);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_triangle(tPictureBox* settings, signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char fill, unsigned int color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_triangle(pDisplay, Ax + settings->Internals.Position.X + 2, Ay + settings->Internals.Position.Y + 2, Bx + settings->Internals.Position.X + 2, By + settings->Internals.Position.Y + 2, Cx + settings->Internals.Position.X + 2, Cy + settings->Internals.Position.Y + 2, fill, color);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_string(tPictureBox* settings, tFont *pFont, char *pcString, signed int lLength, unsigned int foreground_color, unsigned int background_color, bool ulOpaque, bool ulVisible, bool WordWrap, signed int lX, signed int lY, signed int _SelStart, signed int _SelLen)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tDisplay *pDisplay = settings->Internals.pDisplay;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	put_string(pDisplay, pFont, pcString, lLength, foreground_color, background_color, ulOpaque, ulVisible, WordWrap, lX + settings->Internals.Position.X + 2, lY + settings->Internals.Position.Y + 2, _SelStart, _SelLen);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void picturebox_put_3d_triangle(tPictureBox* settings, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 4;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);

	unsigned int color = Color;//(controlls_change_color(Color, (-triangle_median))<<8) & 0xFFFFFF00;

	signed int X_start = (signed int)screenPoints.x[1];
	signed int Y_start = (signed int)screenPoints.y[1];
	signed int X_end = (signed int)screenPoints.x[2];
	signed int Y_end = (signed int)screenPoints.y[2];
	picturebox_put_line(settings, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[2];
	Y_start = (signed int)screenPoints.y[2];
	X_end = (signed int)screenPoints.x[3];
	Y_end = (signed int)screenPoints.y[3];
	picturebox_put_line(settings, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[3];
	Y_start = (signed int)screenPoints.y[3];
	X_end = (signed int)screenPoints.x[1];
	Y_end = (signed int)screenPoints.y[1];
	picturebox_put_line(settings, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);
}
//#######################################################################################
void picturebox_put_3d_rectangle(tPictureBox* settings, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color)
{
	//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 8;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);
	picturebox_put_line(settings, (signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);

	picturebox_put_line(settings, (signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);

	picturebox_put_line(settings, (signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);

	picturebox_put_line(settings, (signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);
	picturebox_put_line(settings, (signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);
}
//#######################################################################################
