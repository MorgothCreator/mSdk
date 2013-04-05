/*
 *  lib/gfx/listbox.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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
#include "checkbox.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "graphic_string.h"
#include "controls_definition.h"
#include "board_properties.h"
//#######################################################################################
static void paint_checkbox(tCheckBox* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, CursorState cursor)
{
	unsigned int color = 0;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + y_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
	//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
	//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
	//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	if(cursor == Cursor_Down) color = controls_color.Control_Color_Enabled_Border_Push;
	else if(cursor == Cursor_Move) color = controls_color.Control_Color_Enabled_Border_Push;
	else if(cursor == Cursor_Up) color = controls_color.Control_Color_Enabled_Border_Pull;
	else color = controls_color.Control_Color_Enabled_Border_Pull;
	put_rectangle(pDisplay, x_start, y_start, y_len, y_len, false, controlls_change_color(color, -3));
	put_rectangle(pDisplay, x_start + 1, y_start + 1, y_len - 2, y_len - 2, false, controlls_change_color(color, -2));
	if(cursor == Cursor_Down) color = controls_color.Control_Color_Enabled_Buton_Push;
	else if(cursor == Cursor_Move) color = controls_color.Control_Color_Enabled_Buton_Push;
	else if(cursor == Cursor_Up) color = controls_color.Control_Color_Enabled_Buton_Pull;
	else color = controls_color.Control_Color_Enabled_Buton_Pull;
	put_rectangle(pDisplay, x_start + 2, y_start + 2, y_len - 4, y_len - 4, true, color);
	if(settings->Cheched == true) put_rectangle(pDisplay, x_start + 4, y_start + 4, y_len - 8, y_len - 8, true, controlls_change_color(color, -3));
	if(settings->Internals.Caption.Text)
	{
		pDisplay->sClipRegion.sXMin = x_start + y_len + 2;
		pDisplay->sClipRegion.sYMin = y_start + 4;
		pDisplay->sClipRegion.sXMax = ((x_start + x_len) - 4);
		pDisplay->sClipRegion.sYMax = ((y_start + y_len) - 4);
		//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
		//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
		//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
		//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		signed int x_str_location;
		signed int y_str_location;
		if(settings->Internals.Caption.WordWrap)
		{
			StringProperties_t str_properties = string_properties_get(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, settings->Internals.Caption.WordWrap, -1);
			x_str_location = x_start + (((settings->Internals.Size.X - y_len)>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			y_str_location = y_start + ((settings->Internals.Size.Y>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}else
		{
			x_str_location = x_start + y_len + 2;
			unsigned char CharHeight = char_height_get(settings->Internals.Caption.Font);
			y_str_location = y_start + ((settings->Internals.Size.Y>>1)-(CharHeight>>1));
		}
		if(cursor == Cursor_Down) put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Enabled.Ink.Push, settings->Color.Enabled.Buton.Push, false, true, settings->Internals.Caption.WordWrap, x_str_location, y_str_location, 0, 0);
		else if(cursor == Cursor_Move) put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Enabled.Ink.Push, settings->Color.Enabled.Buton.Push, false, true, settings->Internals.Caption.WordWrap, x_str_location, y_str_location, 0, 0);
		else if(cursor == Cursor_Up) put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Enabled.Ink.Pull, settings->Color.Enabled.Buton.Pull, false, true, settings->Internals.Caption.WordWrap, x_str_location, y_str_location, 0, 0);
		else put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Enabled.Ink.Pull, settings->Color.Enabled.Buton.Pull, false, true, settings->Internals.Caption.WordWrap, x_str_location, y_str_location, 0, 0);
	}
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + x_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
	//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
	//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
	//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	box_cache_clean(pDisplay, x_start, y_start, x_len, y_len);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void checkbox(tCheckBox *settings, tControlCommandData* control_comand)
{
	if((control_comand->CursorCoordonateUsed == true && settings->Internals.NeedEntireRefresh == false) || settings == NULL) return;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse commands */
#ifdef NO_ENUM_ON_SWITCH
		switch((unsigned char)control_comand->Comand)
#else
		switch(control_comand->Comand)
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
	/* Verify if position on size has been modified */
	if(settings->Internals.Control.Initiated == false)
	{
		settings->Internals.Position.X = settings->Position.X;
		settings->Internals.Position.Y = settings->Position.Y;
		settings->Internals.Size.X = settings->Size.X;
		settings->Internals.Size.Y = settings->Size.Y;
	}
	if(settings->Position.X != settings->Internals.Position.X ||
			settings->Position.Y != settings->Internals.Position.Y ||
				settings->Size.X != settings->Internals.Size.X ||
					settings->Size.Y != settings->Internals.Size.Y ||
						settings->Internals.Caption.Font != settings->Caption.Font ||
							settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
								settings->Internals.Caption.Text != settings->Caption.Text ||
									settings->Internals.Caption.WordWrap != settings->Caption.WordWrap)
										settings->Internals.NeedEntireRefresh = true;

	int X_StartBox = settings->Internals.Position.X;
	int Y_StartBox = settings->Internals.Position.Y;
	int X_LenBox = settings->Internals.Size.X;
	int Y_LenBox = settings->Internals.Size.Y;
	tDisplay *pDisplay = settings->Internals.pDisplay;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
	if(settings->Internals.NeedEntireRefresh == true || (settings->Internals.OldStateVisible != settings->Visible && settings->Visible == false))
	{
		if(!settings->Internals.NoPaintBackGround)
		{
			settings->Internals.OldStateVisible = settings->Visible;
			tRectangle back_up_clip = pDisplay->sClipRegion;
			pDisplay->sClipRegion.sXMin = X_StartBox;
			pDisplay->sClipRegion.sYMin = Y_StartBox;
			pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
			pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
			//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
			//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
			//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip);
			put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, settings->Color.Scren);
			box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			pDisplay->sClipRegion = back_up_clip;
		}
	}

	/* Verify if is Entire refresh, entire repaint, or state changed */
	if((settings->Internals.NeedEntireRefresh == true ||
			settings->Internals.NeedEntireRepaint == true ||
				settings->Internals.Control.Initiated == false ||
					//settings->Enabled == true ||
						settings->Internals.OldStateVisible != settings->Visible) &&
							settings->Visible == true)
	{
		/* Copy new locations and dimensions to actual locations and dimensions */
		settings->Internals.Position.X = settings->Position.X;
		settings->Internals.Position.Y = settings->Position.Y;
		settings->Internals.Size.X = settings->Size.X;
		settings->Internals.Size.Y = settings->Size.Y;
		settings->Internals.Caption.Font = settings->Caption.Font;
		//settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		settings->Caption.Text = settings->Internals.Caption.Text;
		if(settings->Enabled == true)
		{
			paint_checkbox(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, Cursor_Up);
		}
		else
		{
			tRectangle back_up_clip = pDisplay->sClipRegion;
			pDisplay->sClipRegion.sXMin = X_StartBox;
			pDisplay->sClipRegion.sYMin = Y_StartBox;
			pDisplay->sClipRegion.sXMax = X_StartBox + Y_LenBox;
			pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
			//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
			//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
			//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip);
			unsigned int color = controls_color.Control_Color_Disabled_Border_Pull;
			put_rectangle(pDisplay, X_StartBox, Y_StartBox, Y_LenBox, Y_LenBox, false, controlls_change_color(color, -3));
			put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + 1, Y_LenBox - 2, Y_LenBox - 2, false, controlls_change_color(color, -2));
			color = controls_color.Control_Color_Disabled_Buton_Pull;
			put_rectangle(pDisplay, X_StartBox + 2, Y_StartBox + 2, Y_LenBox - 4, Y_LenBox - 4, true, color);
			if(settings->Cheched == true) put_rectangle(pDisplay, X_StartBox + 4, Y_StartBox + 4, Y_LenBox - 8, Y_LenBox - 8, true, controlls_change_color(color, -3));
			if(settings->Internals.Caption.Text)
			{
				pDisplay->sClipRegion.sXMin = X_StartBox + Y_LenBox + 2;
				pDisplay->sClipRegion.sYMin = Y_StartBox + 4;
				pDisplay->sClipRegion.sXMax = ((X_StartBox + X_LenBox) - 4);
				pDisplay->sClipRegion.sYMax = ((Y_StartBox + Y_LenBox) - 4);
				//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
				//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
				//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
				//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
				clip_limit(&pDisplay->sClipRegion, &back_up_clip);
				signed int x_str_location;
				signed int y_str_location;
				if(settings->Internals.Caption.WordWrap)
				{
					StringProperties_t str_properties = string_properties_get(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, settings->Internals.Caption.WordWrap, -1);
					x_str_location = pDisplay->sClipRegion.sXMin + (((settings->Internals.Size.X - Y_LenBox)>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
					y_str_location = Y_StartBox + ((settings->Internals.Size.Y>>1)-(str_properties.StringColsHeight_Pixels>>1));
				}else
				{
					x_str_location = pDisplay->sClipRegion.sXMin;
					y_str_location = Y_StartBox + ((settings->Internals.Size.Y>>1)-(settings->Internals.Caption.Font->ucHeight>>1));
				}
				put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Disabled.Ink, settings->Color.Disabled.Buton, false, true, settings->Internals.Caption.WordWrap, x_str_location, y_str_location, 0, 0);
			}
			pDisplay->sClipRegion.sXMin = X_StartBox;
			pDisplay->sClipRegion.sYMin = Y_StartBox;
			pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
			pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
			//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
			//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
			//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip);
			box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			pDisplay->sClipRegion = back_up_clip;
		}
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		return;
	}
	/* Check if inside window */
	bool inside_window = check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;

	if(((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			settings->Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || settings->Internals.CursorDownInsideBox == true) &&
					settings->Visible == true) ||
						(settings->Internals.OldStateCheched != settings->Cheched && settings->Visible == true && settings->Enabled == true))
	{
		if(control_comand->Cursor == Cursor_Down && inside_window == true) settings->Internals.CursorDownInsideBox = true;
		settings->Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window == true && settings->Internals.CursorDownInsideBox == true)settings->Events.CursorDown = true;
		if(control_comand->Cursor == Cursor_Up && inside_window == true && settings->Internals.CursorDownInsideBox == true)settings->Events.CursorUp = true;
		if(control_comand->Cursor == Cursor_Move && inside_window == true && settings->Internals.CursorDownInsideBox == true)settings->Events.CursorMove = true;
		if(settings->Events.OnDown.CallBack && control_comand->Cursor == Cursor_Down && inside_window == true && settings->Internals.CursorDownInsideBox == true)
		{
			settings->Events.CursorDown = true;
			settings->Events.OnDown.CallbackReturnData = settings->Events.OnDown.CallBack(settings->Events.OnDown.CallbackData);
		}
		if(settings->Events.OnUp.CallBack && control_comand->Cursor == Cursor_Up && inside_window == true && settings->Internals.CursorDownInsideBox == true)
		{
			settings->Events.CursorUp = true;
			settings->Events.OnUp.CallbackReturnData = settings->Events.OnUp.CallBack(settings->Events.OnUp.CallbackData);
		}

		if(control_comand->Cursor == Cursor_Down && settings->StateChangedOn == Cursor_Down && inside_window == true && settings->Internals.CursorDownInsideBox == true)
		{
			if(settings->Cheched == true)
			{
				settings->Cheched = false;
				if(settings->Events.OnUnCheched.CallBack) settings->Events.OnUnCheched.CallbackReturnData = settings->Events.OnUnCheched.CallBack(settings->Events.OnUnCheched.CallbackData);
				settings->Events.UnChecked = true;
			}
			else
			{
				settings->Cheched = true;
				if(settings->Events.OnCheched.CallBack) settings->Events.OnCheched.CallbackReturnData = settings->Events.OnCheched.CallBack(settings->Events.OnCheched.CallbackData);
				settings->Events.Checked = true;
			}
		}
		if(control_comand->Cursor == Cursor_Up && settings->StateChangedOn == Cursor_Up && inside_window == true && settings->Internals.CursorDownInsideBox == true)
		{
			if(settings->Cheched == true)
			{
				settings->Cheched = false;
				if(settings->Events.OnUnCheched.CallBack) settings->Events.OnUnCheched.CallbackReturnData = settings->Events.OnUnCheched.CallBack(settings->Events.OnUnCheched.CallbackData);
				settings->Events.UnChecked = true;
			}
			else
			{
				settings->Cheched = true;
				if(settings->Events.OnCheched.CallBack) settings->Events.OnCheched.CallbackReturnData = settings->Events.OnCheched.CallBack(settings->Events.OnCheched.CallbackData);
				settings->Events.Checked = true;
			}
		}
		settings->Internals.OldStateCheched = settings->Cheched;
		tRectangle back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = X_StartBox;
		pDisplay->sClipRegion.sYMin = Y_StartBox;
		pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
		pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
		//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
		//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
		//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		paint_checkbox(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->Cursor);
		pDisplay->sClipRegion = back_up_clip;
	}
	if(settings->Events.OnMove.CallBack && control_comand->Cursor == Cursor_Move && inside_window == true && settings->Internals.CursorDownInsideBox == true && settings->Visible && settings->Enabled == true)
	{
		settings->Events.CursorMove = true;
		settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings->Events.OnMove.CallbackData);
	}
	if(settings->Internals.CursorDownInsideBox == true && control_comand->Cursor == Cursor_Up) settings->Internals.CursorDownInsideBox = false;
	control_comand->CursorCoordonateUsed = settings->Internals.CursorDownInsideBox;
	return;
}
//#######################################################################################
tCheckBox *new_checkbox(tDisplay *ScreenDisplay)
{
	tCheckBox* settings = (tCheckBox*)calloc(1, sizeof(tCheckBox));

	if(!settings || !ScreenDisplay) return NULL;

	settings->Internals.pDisplay = ScreenDisplay;
	settings->Caption.TextAlign = Align_Left;
	settings->Caption.WordWrap = true;
	settings->Caption.Font = controls_color.DefaultFont;
	settings->Caption.Text = "CheckBox";
	//settings->Caption.Text = malloc(sizeof("Textbox") + 1);
	//strcpy(settings->Caption.Text, "Textbox");

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
	settings->Size.X = 100;
	settings->Size.Y = 40;
	settings->StateChangedOn = Cursor_Up;
	settings->Visible = true;
	return settings;
}
//#######################################################################################
bool free_checkbox(tCheckBox* settings)
{
	if(!settings) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	checkbox(settings, &comand);
	if(settings->Internals.Caption.Text) free(settings->Internals.Caption.Text);
	if(settings->Caption.Text) free(settings->Caption.Text);
	if(settings) free(settings);
	return true;
}
//#######################################################################################




