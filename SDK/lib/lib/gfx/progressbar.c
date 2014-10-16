/*
 *  lib/gfx/progressbar.c
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
#include <stdlib.h>
#include "progressbar.h"
#include "window_def.h"
#include "controls_definition.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "graphic_string.h"
#include "lib/generic.h"
#include "board_properties.h"
//#######################################################################################
static void paint_progressbar(tProgressBar* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand)
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
	color = controls_color.Control_Color_Enabled_Buton_Pull;
	if(!settings->Enabled || !ParentWindow->Internals.OldStateEnabled) color = settings->Color.Disabled.Buton;
	put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
	signed int Position = to_percentage(settings->MinimValue, settings->MaximValue, settings->Size.X - 8, settings->Value);
	put_rectangle(pDisplay, x_start + 4, y_start + 4, Position, y_len - 8, true, controlls_change_color(color, -2));
	if(settings->Internals.Caption.Text)
	{
		pDisplay->sClipRegion.sXMin = x_start + 4;
		pDisplay->sClipRegion.sYMin = y_start + 4;
		pDisplay->sClipRegion.sXMax = ((x_start + x_len) - 4);
		pDisplay->sClipRegion.sYMax = ((y_start + y_len) - 4);
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		signed int x_str_location;
		signed int y_str_location;
		if(settings->Internals.Caption.WordWrap)
		{
			StringProperties_t str_properties = string_properties_get(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, settings->Internals.Caption.WordWrap, -1);
			x_str_location = x_start + ((x_len>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			y_str_location = y_start + ((y_len>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}else
		{
			x_str_location = x_start + 4;			
			unsigned char CharHeight = char_height_get(settings->Internals.Caption.Font);
			y_str_location = y_start + ((settings->Internals.Size.Y>>1)-(CharHeight>>1));
		}
		if(settings->Enabled == true) put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Enabled.Ink.Pull, settings->Color.Enabled.Buton.Pull, false, true, settings->Internals.Caption.WordWrap, x_str_location, y_str_location, 0, 0);
		else  put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Disabled.Ink, settings->Color.Disabled.Buton, false, true, settings->Internals.Caption.WordWrap, x_str_location, y_str_location, 0, 0);
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
void progressbar(tProgressBar *settings, tControlCommandData* control_comand)
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
	/* Verify if position on size has been modified */
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
		settings->Internals.Caption.Font = settings->Caption.Font;
		settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;
		settings->Internals.OldMinimValue = settings->MinimValue;
		settings->Internals.OldMaximValue = settings->MaximValue;
		settings->Internals.OldValue = settings->Value;
	}

	//signed int Position = to_percentage(settings->MinimValue, settings->MaximValue, settings->Size.X - 4, settings->Value);
	//signed int OldPosition = to_percentage(settings->MinimValue, settings->MaximValue, settings->Size.X - 4, settings->Internals.OldValue);
	//if(Position != OldPosition)
	if(settings->Value != settings->Internals.OldValue)
	{
		settings->Internals.OldValue = settings->Value;
		settings->Internals.NeedEntireRepaint = true;
		if((control_comand->Cursor == Cursor_Move && settings->StateChangedOn == Cursor_Move) ||
			(control_comand->Cursor == Cursor_Up && settings->StateChangedOn == Cursor_Up) ||
				(control_comand->Cursor == Cursor_Down && settings->StateChangedOn == Cursor_Down))
		{
			settings->Events.ValueChanged = true;
			if(settings->Events.OnValueChange.CallBack && settings->Enabled && settings->Visible)settings->Events.OnValueChange.CallbackReturnData = settings->Events.OnValueChange.CallBack(settings->Events.OnValueChange.CallbackData);
		}
		//control_comand->CursorCoordonateUsed = true;
	}

	if(ParentWindow)
	{
		if((settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X) != settings->Internals.Position.X ||
				(settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y) != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.Caption.Font != settings->Caption.Font ||
								settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
									settings->Internals.Caption.Text != settings->Caption.Text ||
										settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
											settings->Internals.OldMinimValue != settings->MinimValue ||
												settings->Internals.OldMaximValue != settings->MaximValue ||
													settings->Internals.OldValue != settings->Value ||
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
							settings->Internals.Caption.Font != settings->Caption.Font ||
								settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
									settings->Internals.Caption.Text != settings->Caption.Text ||
										settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
											settings->Internals.OldMinimValue != settings->MinimValue ||
												settings->Internals.OldMaximValue != settings->MaximValue ||
													settings->Internals.OldValue != settings->Value ||
														settings->Internals.OldStateEnabled != settings->Enabled)
															settings->Internals.NeedEntireRefresh = true;
	}

	int X_StartBox = settings->Internals.Position.X;
	int Y_StartBox = settings->Internals.Position.Y;
	int X_LenBox = settings->Internals.Size.X;
	int Y_LenBox = settings->Internals.Size.Y;
	tDisplay *pDisplay = settings->Internals.pDisplay;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
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
		settings->Internals.Caption.Font = settings->Caption.Font;
		//settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;
		settings->Internals.OldMinimValue = settings->MinimValue;
		settings->Internals.OldMaximValue = settings->MaximValue;
		settings->Internals.OldValue = settings->Value;
		if(settings->Size.X == 0 || settings->Size.Y == 0) return;

		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		settings->Caption.Text = settings->Internals.Caption.Text;

		tRectangle back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = X_StartBox;
		pDisplay->sClipRegion.sYMin = Y_StartBox;
		pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
		pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		CursorState Cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		paint_progressbar(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		control_comand->Cursor = Cursor;
		pDisplay->sClipRegion = back_up_clip;
		control_comand->WindowRefresh = true;

		settings->Internals.ParentWindowStateEnabled = ParentWindow->Internals.OldStateEnabled;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.OldStateEnabled = settings->Enabled;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		control_comand->WindowRefresh |= true;
		return;
	}
	/* Check if inside window */
	bool inside_window = check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;

	if((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Move || control_comand->Cursor == Cursor_Down) &&
			/*settings->Internals.OldStateCursor != control_comand->Cursor &&*/
				(inside_window == true || settings->Internals.CursorDownInsideBox == true) &&
					settings->Enabled == true &&
						settings->Visible == true &&
							control_comand->CursorCoordonateUsed == false)
	{
		if(control_comand->Cursor == Cursor_Down && inside_window == true) settings->Internals.CursorDownInsideBox = true;
		//settings->Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window) settings->Events.CursorDown = true;
		if(control_comand->Cursor == Cursor_Up && inside_window == true)settings->Events.CursorUp = true;
		if(control_comand->Cursor == Cursor_Move && inside_window == true) settings->Events.CursorMove = true;
		if(settings->Events.OnDown.CallBack && control_comand->Cursor == Cursor_Down)
		{
			settings->Events.OnDown.CallbackReturnData = settings->Events.OnDown.CallBack(settings->Events.OnDown.CallbackData);
		}
		if(settings->Events.OnUp.CallBack && control_comand->Cursor == Cursor_Up && inside_window == true)
		{
			settings->Events.OnUp.CallbackReturnData = settings->Events.OnUp.CallBack(settings->Events.OnUp.CallbackData);
		}
		if(settings->Events.OnMove.CallBack && control_comand->Cursor == Cursor_Move && inside_window == true && settings->Visible && settings->Enabled == true)
		{
			settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings->Events.OnMove.CallbackData);
		}
		if((control_comand->Cursor == Cursor_Move/* && settings->StateChangedOn == Cursor_Move) ||
				(control_comand->Cursor == Cursor_Up && settings->StateChangedOn == Cursor_Up) ||
					(control_comand->Cursor == Cursor_Down && settings->StateChangedOn == Cursor_Down*/))
		{
			if(settings->Internals.CursorDownInsideBox)
			{
				settings->Value = percentage_to(settings->MinimValue, settings->MaximValue, settings->Size.X - 8, control_comand->X - (X_StartBox + 4));
				if(settings->Value > settings->MaximValue) settings->Value = settings->MaximValue;
				if(settings->Value < settings->MinimValue) settings->Value = settings->MinimValue;
			}
		}
		tRectangle back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = X_StartBox;
		pDisplay->sClipRegion.sYMin = Y_StartBox;
		pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
		pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		paint_progressbar(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		pDisplay->sClipRegion = back_up_clip;
	}
	if(control_comand->Cursor && settings->Internals.CursorDownInsideBox) control_comand->CursorCoordonateUsed |= true;
	if(settings->Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction)) settings->Internals.CursorDownInsideBox = false;
	//control_comand->CursorCoordonateUsed |= settings->Internals.CursorDownInsideBox;
	//control_comand->WindowRefresh |= true;
	return;
}
//#######################################################################################
tProgressBar *new_progressbar(void *ParentWindow)
{
	tProgressBar *settings = (tProgressBar*)calloc(1, sizeof(tProgressBar));

	if(!settings || !ParentWindow) return NULL;
	settings->Internals.ParentWindow = ParentWindow;

	tWindow *_ParentWindow = (tWindow *)ParentWindow;
	settings->Internals.pDisplay = _ParentWindow->Internals.pDisplay;

	settings->Caption.TextAlign = Align_Left;
	settings->Caption.WordWrap = true;
	settings->Caption.Font = controls_color.DefaultFont;
	settings->Caption.Text = "Progressbar";
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
	settings->StateChangedOn = Cursor_Move;
	settings->Visible = true;
	//settings->Value = 0;
	//settings->MinimValue = 0;
	settings->MaximValue = 100;
	return settings;
}
//#######################################################################################
bool free_progressbar(tProgressBar* settings)
{
	if(!settings) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	progressbar(settings, &comand);
	if(settings->Internals.Caption.Text) free(settings->Internals.Caption.Text);
	if(settings->Caption.Text) free(settings->Caption.Text);
	if(settings) free(settings);
	return true;
}
//#######################################################################################




