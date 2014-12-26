/*
 *  lib/gfx/buton.c
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
#include "button.h"
#include "window_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/timer_api.h"
#include "graphic_string.h"
#include "controls_definition.h"
//#######################################################################################
static void paint_button(tButton* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand)
{
	unsigned int color = 0;
	tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + x_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	CursorState cursor = control_comand->Cursor;
	if(cursor == Cursor_Down || cursor == Cursor_Move)
		color = controls_color.Control_Color_Enabled_Border_Push;
	else if(cursor == Cursor_Up)
		color = controls_color.Control_Color_Enabled_Border_Pull;
	else
		color = controls_color.Control_Color_Enabled_Border_Pull;
	if(!settings->Enabled || !ParentWindow->Internals.OldStateEnabled)
		color = settings->Color.Disabled.Border;
	put_rectangle(pDisplay, x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
	put_rectangle(pDisplay, x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
	if(cursor == Cursor_Down || cursor == Cursor_Move)
		color = controls_color.Control_Color_Enabled_Buton_Push;
	/*else if(cursor == Cursor_Move) color = controls_color.Control_Color_Enabled_Buton_Push;*/
	else if(cursor == Cursor_Up)
		color = controls_color.Control_Color_Enabled_Buton_Pull;
	else
		color = controls_color.Control_Color_Enabled_Buton_Pull;
	if(!settings->Enabled || !ParentWindow->Internals.OldStateEnabled)
		color = settings->Color.Disabled.Buton;
	put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
	if(settings->Internals.Caption.Text)
	{
		pDisplay->sClipRegion.sXMin = x_start + 2;
		pDisplay->sClipRegion.sYMin = y_start + 2;
		pDisplay->sClipRegion.sXMax = ((x_start + x_len) - 2);
		pDisplay->sClipRegion.sYMax = ((y_start + y_len) - 2);
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);

		signed int x_str_location = pDisplay->sClipRegion.sXMin;
		signed int y_str_location = pDisplay->sClipRegion.sYMin;
		if(settings->Caption.TextAlign == Align_Center){
			StringProperties_t str_properties = string_properties_get(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, settings->Internals.Caption.WordWrap, -1);
			x_str_location = x_start + ((x_len>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			y_str_location = y_start + ((y_len>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}
		print_string_properties properties;
		properties.pDisplay = pDisplay;
		properties.pFont = settings->Internals.Caption.Font;
		properties.pcString = settings->Internals.Caption.Text;
		properties.lLength = -1;
		//properties.foreground_color = settings->Color.Enabled.Ink.Push;
		//properties.background_color = settings->Color.Enabled.Buton.Push;
		properties.ulOpaque = false;
		properties.ulVisible = true;
		properties.WordWrap = settings->Internals.Caption.WordWrap;
		properties.lX = x_str_location;
		properties.lY = y_str_location;
		properties._SelStart = 0;
		properties._SelLen = 0;
		if(settings->Enabled == true) {
			if(cursor == Cursor_Down || cursor == Cursor_Move) {
				properties.foreground_color = settings->Color.Enabled.Ink.Push;
				properties.background_color = settings->Color.Enabled.Buton.Push;
			}
			else if(cursor == Cursor_Up) {
				properties.foreground_color = settings->Color.Enabled.Ink.Pull;
				properties.background_color = settings->Color.Enabled.Buton.Pull;
			}
			else {
				properties.foreground_color = settings->Color.Enabled.Ink.Pull;
				properties.background_color = settings->Color.Enabled.Buton.Pull;
			}
		} else {
				properties.foreground_color = settings->Color.Disabled.Ink;
				properties.background_color = settings->Color.Disabled.Buton;
		}
		put_string(&properties);
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
void button(tButton *settings, tControlCommandData* control_comand)
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
	tWindow *ParentWindow = (tWindow *)settings->Internals.ParentWindow;
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
							settings->Internals.Caption.Font != settings->Caption.Font ||
								settings->Internals.Caption.Text != settings->Caption.Text ||
									settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
										settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
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
								settings->Internals.Caption.Text != settings->Caption.Text ||
									settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
										settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
											settings->Internals.OldStateEnabled != settings->Enabled)
												settings->Internals.NeedEntireRefresh = true;
	}

	//CursorState cursor = control_comand->Cursor;
	//if(settings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
	//if(settings->Internals.Caption.Text != NULL && settings->Caption.Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Caption.Text) == NULL)
		//settings->Internals.NeedEntireRefresh = true;

	signed int X_StartBox = settings->Internals.Position.X;
	signed int Y_StartBox = settings->Internals.Position.Y;
	signed int X_LenBox = settings->Internals.Size.X;
	signed int Y_LenBox = settings->Internals.Size.Y;
	tDisplay *pDisplay = ParentWindow->Internals.pDisplay;

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
		settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;
		if(settings->Size.X == 0 || settings->Size.Y == 0) return;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		settings->Caption.Text = settings->Internals.Caption.Text;
		paint_button(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
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
	if((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			settings->Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || settings->Internals.CursorDownInsideBox == true) &&
					settings->Enabled == true &&
						settings->Visible == true &&
							control_comand->CursorCoordonateUsed == false)
	{
		settings->Internals.OldStateCursor = control_comand->Cursor;
		if(control_comand->Cursor == Cursor_Down && inside_window == true)
		{
			timer_interval(&settings->Internals.ContinuouslyPushTimer, 500);
			timer_enable(&settings->Internals.ContinuouslyPushTimer);
			settings->Events.CursorDown = true;
			settings->Internals.CursorDownInsideBox = true;
			if(settings->Events.OnDown.CallBack)
			{
				settings->Events.OnDown.CallbackReturnData = settings->Events.OnDown.CallBack(settings->Events.OnDown.CallbackData);
			}
		}
		if(control_comand->Cursor == Cursor_Up && settings->Internals.CursorDownInsideBox == true)
		{
			timer_disable(&settings->Internals.ContinuouslyPushTimer);
			settings->Events.CursorUp = true;
			if(settings->Events.OnUp.CallBack)
			{
				settings->Events.OnUp.CallbackReturnData = settings->Events.OnUp.CallBack(settings->Events.OnUp.CallbackData);
			}
		}
		paint_button(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
	}
	if(control_comand->Cursor == Cursor_Move && inside_window == true && settings->Internals.CursorDownInsideBox == true && (timer_tick(&settings->Internals.ContinuouslyPushTimer) || settings->Internals.ContinuouslyPushTimerDisabled))
	{
		timer_interval(&settings->Internals.ContinuouslyPushTimer, 50);
		timer_enable(&settings->Internals.ContinuouslyPushTimer);
		settings->Events.CursorMove = true;
		if(settings->Events.OnMove.CallBack)
		{
			settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings->Events.OnMove.CallbackData);
		}
	}
	if(control_comand->Cursor && settings->Internals.CursorDownInsideBox) control_comand->CursorCoordonateUsed |= true;
	if(settings->Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction)) settings->Internals.CursorDownInsideBox = false;
	//control_comand->CursorCoordonateUsed |= settings->Internals.CursorDownInsideBox;
	//control_comand->WindowRefresh |= true;
	return;
}
//#######################################################################################
tButton *new_button(void *ParentWindow)
{
	tButton* settings = (tButton*)calloc(1, sizeof(tButton));

	if(!settings || !ParentWindow) return NULL;
	settings->Internals.ParentWindow = ParentWindow;
	//settings->Internals.pDisplay = ScreenDisplay;
	settings->Caption.TextAlign = Align_Center;
	settings->Caption.WordWrap = true;
	settings->Caption.Font = controls_color.DefaultFont;
	//settings->Caption.Text = "Button";
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
	settings->Size.X = 60;
	settings->Size.Y = 20;
	settings->StateChangedOn = Cursor_Up;
	settings->Visible = true;
	return settings;
}
//#######################################################################################
bool free_button(tButton* settings)
{
	if(!settings) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	button(settings, &comand);
	if(settings->Internals.Caption.Text) free(settings->Internals.Caption.Text);
	if(settings->Caption.Text) free(settings->Caption.Text);
	if(settings) free(settings);
	return true;
}
//#######################################################################################
