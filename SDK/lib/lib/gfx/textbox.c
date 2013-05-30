/*
 *  lib/gfx/textbox.c
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
#include "textbox.h"
#include "controls_definition.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "scrollbar.h"
#include "graphic_string.h"
#include "board_properties.h"
//#######################################################################################
static void paint_textbox(tTextBox* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand)
{
	//if(control_comand->CursorCoordonateUsed && !settings->Internals.NeedEntireRefresh && !settings->Internals.NeedEntireRepaint) return;

	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + x_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
	//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
	//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
	//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	unsigned int color = settings->Color.Enabled.Buton.Pull;
	//color = controls_color.Control_Color_Enabled_Border_Pull;
	if(settings->Internals.NeedEntireRefresh /*control_comand->Comand == Control_Entire_Refresh*/)
	{
		put_rectangle(pDisplay, x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
		put_rectangle(pDisplay, x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
		put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4 - settings->Internals.Size.ScrollSize, y_len - 4 - settings->Internals.Size.ScrollSize, true, color);
	}
	if(settings->Internals.Caption.Text)
	{
		pDisplay->sClipRegion.sXMin = x_start + 4;
		pDisplay->sClipRegion.sYMin = y_start + 4;
		pDisplay->sClipRegion.sXMax = ((x_start + x_len) - 4 - settings->Internals.Size.ScrollSize);
		pDisplay->sClipRegion.sYMax = ((y_start + y_len) - 4 - settings->Internals.Size.ScrollSize);
		//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
		//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
		//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
		//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);

		signed int x_str_location = x_start + 4;
		signed int y_str_location = y_start + 4;

		signed int Start = settings->SelStart;
		signed int Len = settings->SelLen;
		unsigned int command_return = 0;
		bool CommandReceived = false;

		if(settings->Internals.CursorDownInsideTextBox || settings->Internals.NeedEntireRefresh)
		{
			if(settings->Internals.CursorDownInsideTextBox) string_select_get(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, settings->Internals.Caption.WordWrap, -1, &settings->Internals.Start, &Start, &Len, settings->Internals.PenPushX, settings->Internals.PenPushY, control_comand->X, control_comand->Y, x_str_location - settings->Internals.Hscrollbar->Value, y_str_location - settings->Internals.Vscrollbar->Value, &command_return);

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoLeft && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(settings->Internals.Hscrollbar->Value > settings->Internals.Hscrollbar->Minimum)
				{
					settings->Internals.Hscrollbar->Value -= (command_return & (~ReturnCommand_gm)) >> 1;
					if(settings->Internals.Hscrollbar->Value < settings->Internals.Hscrollbar->Minimum)
					{
						settings->Internals.Hscrollbar->Value = settings->Internals.Hscrollbar->Minimum;
					}
					CommandReceived = true;
				}
			}

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoRight && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(settings->Internals.Hscrollbar->Value < settings->Internals.Hscrollbar->Maximum)
				{
					settings->Internals.Hscrollbar->Value += (command_return & (~ReturnCommand_gm)) >> 1;
					if(settings->Internals.Hscrollbar->Value > settings->Internals.Hscrollbar->Maximum)
					{
						settings->Internals.Hscrollbar->Value = settings->Internals.Hscrollbar->Maximum;
					}
					CommandReceived = true;
				}
			}

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoUp && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(settings->Internals.Vscrollbar->Value > settings->Internals.Vscrollbar->Minimum)
				{
					settings->Internals.Vscrollbar->Value -= (command_return & (~ReturnCommand_gm)) >> 1;
					if(settings->Internals.Vscrollbar->Value < settings->Internals.Vscrollbar->Minimum)
					{
						settings->Internals.Vscrollbar->Value = settings->Internals.Vscrollbar->Minimum;
					}
					CommandReceived = true;
				}
			}

			if((command_return & ReturnCommand_gm) == ReturnCommand_GoDn && (((command_return & (~ReturnCommand_gm)) >> 1) != 0))
			{
				if(settings->Internals.Vscrollbar->Value < settings->Internals.Vscrollbar->Maximum)
				{
					settings->Internals.Vscrollbar->Value += (command_return & (~ReturnCommand_gm)) >> 1;
					if(settings->Internals.Vscrollbar->Value > settings->Internals.Vscrollbar->Maximum)
					{
						settings->Internals.Vscrollbar->Value = settings->Internals.Vscrollbar->Maximum;
					}
					CommandReceived = true;
				}
			}

			if(CommandReceived || settings->Internals.NeedEntireRefresh)
			{
				pDisplay->sClipRegion = back_up_clip;
				tRectangle _back_up_clip = pDisplay->sClipRegion;
				pDisplay->sClipRegion.sXMin = x_start;
				pDisplay->sClipRegion.sYMin = y_start;
				pDisplay->sClipRegion.sXMax = x_start + x_len;
				pDisplay->sClipRegion.sYMax = y_start + y_len;
				//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
				//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
				//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
				//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
				clip_limit(&pDisplay->sClipRegion, &back_up_clip);
				ControlCommands ComandBeck = control_comand->Comand;
				control_comand->Comand = Control_Refresh;
				CursorState cursor = control_comand->Cursor;
				control_comand->Cursor = Cursor_Up;
				if(((command_return & ReturnCommand_gm) == ReturnCommand_GoUp || (command_return & ReturnCommand_gm) == ReturnCommand_GoDn) || settings->Internals.NeedEntireRefresh)
				{
					settings->Internals.Vscrollbar->Internals.NoPaintBackGround = true;
					scrollbar(settings->Internals.Vscrollbar, control_comand);
				}
				if(((command_return & ReturnCommand_gm) == ReturnCommand_GoLeft || (command_return & ReturnCommand_gm) == ReturnCommand_GoRight) || settings->Internals.NeedEntireRefresh)
				{
					settings->Internals.Hscrollbar->Internals.NoPaintBackGround = true;
					scrollbar(settings->Internals.Hscrollbar, control_comand);
				}
				control_comand->Comand = ComandBeck;
				control_comand->Cursor = cursor;

				pDisplay->sClipRegion = _back_up_clip;
			}
		}

		if(Start != settings->SelStart || Len != settings->SelLen || settings->Internals.Hscrollbar->Events.ValueChanged == true || settings->Internals.Vscrollbar->Events.ValueChanged == true || settings->Internals.NeedEntireRefresh == true|| settings->Internals.NeedEntireRepaint == true || CommandReceived != Control_Nop)
		{
			pDisplay->sClipRegion.sXMin = x_start + 4;
			pDisplay->sClipRegion.sYMin = y_start + 4;
			pDisplay->sClipRegion.sXMax = ((x_start + x_len) - 4 - settings->Internals.Size.ScrollSize);
			pDisplay->sClipRegion.sYMax = ((y_start + y_len) - 4 - settings->Internals.Size.ScrollSize);
			//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
			//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
			//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
			//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip);
			settings->Internals.Vscrollbar->Events.ValueChanged = false;
			settings->Internals.Hscrollbar->Events.ValueChanged = false;
			settings->SelStart = Start;
			settings->SelLen = Len;
			put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4 - settings->Internals.Size.ScrollSize, y_len - 4 - settings->Internals.Size.ScrollSize, true, color);
			put_string(pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, -1, settings->Color.Enabled.Ink.Pull, controlls_change_color(color, -2), false, true, settings->Internals.Caption.WordWrap, x_str_location - settings->Internals.Hscrollbar->Value, y_str_location - settings->Internals.Vscrollbar->Value, settings->SelStart, settings->SelLen);
		}
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
void textbox(tTextBox *settings, tControlCommandData* control_comand)
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
	if(settings->Internals.Control.Initiated == false)
	{
		settings->Internals.Position.X = settings->Position.X;
		settings->Internals.Position.Y = settings->Position.Y;
		settings->Internals.Size.X = settings->Size.X;
		settings->Internals.Size.Y = settings->Size.Y;

		settings->Internals.Vscrollbar = new_scrollbar(settings->Internals.pDisplay);
		settings->Internals.Hscrollbar = new_scrollbar(settings->Internals.pDisplay);
	}
	/* Verify if position on size has been modified */
	if(settings->Position.X != settings->Internals.Position.X ||
			settings->Position.Y != settings->Internals.Position.Y ||
				settings->Size.X != settings->Internals.Size.X ||
					settings->Size.Y != settings->Internals.Size.Y ||
						settings->Internals.Size.ScrollSize != settings->Size.ScrollSize ||
							settings->Internals.Caption.Font != settings->Caption.Font ||
								settings->Internals.Caption.Text != settings->Caption.Text ||
									settings->Internals.Caption.WordWrap != settings->Caption.WordWrap)
										settings->Internals.NeedEntireRefresh = true;

	//if(settings->Internals.Caption.Text != NULL && settings->Caption.Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Caption.Text) == NULL)
		//settings->Internals.NeedEntireRefresh = true;

	signed int X_StartBox = settings->Internals.Position.X;
	signed int Y_StartBox = settings->Internals.Position.Y;
	signed int X_LenBox = settings->Internals.Size.X;
	signed int Y_LenBox = settings->Internals.Size.Y;
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
		settings->Internals.Size.ScrollSize = settings->Size.ScrollSize;
		settings->Internals.Caption.Font = settings->Caption.Font;
		settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;


		tRectangle back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = settings->Internals.Position.X + 4;
		pDisplay->sClipRegion.sYMin = settings->Internals.Position.Y + 4;
		pDisplay->sClipRegion.sXMax = ((settings->Internals.Position.X + settings->Internals.Size.X) - 4 - settings->Internals.Size.ScrollSize);
		pDisplay->sClipRegion.sYMax = ((settings->Internals.Position.Y + settings->Internals.Size.Y) - 4 - settings->Internals.Size.ScrollSize);
		//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
		//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
		//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
		//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		StringProperties_t StrProperties = string_properties_get(settings->Internals.pDisplay, settings->Internals.Caption.Font, settings->Internals.Caption.Text, settings->Internals.Caption.WordWrap, -1);
		pDisplay->sClipRegion = back_up_clip;

		settings->Internals.Vscrollbar->Maximum = StrProperties.StringColsHeight_Pixels - (settings->Internals.Size.Y - 6 - settings->Size.ScrollSize);
		if(settings->Internals.Vscrollbar->Maximum < 0) settings->Internals.Vscrollbar->Maximum = 0;
		settings->Internals.Vscrollbar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		settings->Internals.Vscrollbar->Position.Y = settings->Internals.Position.Y + 2;
		settings->Internals.Vscrollbar->Size.X = settings->Size.ScrollSize;
		settings->Internals.Vscrollbar->Size.Y = settings->Internals.Size.Y - 4 - settings->Size.ScrollSize;

		settings->Internals.Hscrollbar->Maximum = StrProperties.StringRowsMaxLength_Pixels - (settings->Internals.Size.X - 6 - settings->Size.ScrollSize);
		if(settings->Internals.Hscrollbar->Maximum < 0) settings->Internals.Hscrollbar->Maximum = 0;
		settings->Internals.Hscrollbar->Position.X = settings->Internals.Position.X + 2;
		settings->Internals.Hscrollbar->Position.Y = (settings->Internals.Position.Y + settings->Internals.Size.Y) - 2 - settings->Size.ScrollSize;
		settings->Internals.Hscrollbar->Size.X = settings->Internals.Size.X - 4 - settings->Size.ScrollSize;
		settings->Internals.Hscrollbar->Size.Y = settings->Size.ScrollSize;

		back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = X_StartBox;
		pDisplay->sClipRegion.sYMin = Y_StartBox;
		pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
		pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
		//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
		//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
		//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		//ControlCommands ComandBeck = control_comand->Comand;
		if(settings->Internals.NeedEntireRepaint) control_comand->Comand = Control_Refresh;
		settings->Internals.Vscrollbar->Internals.NoPaintBackGround = true;
		scrollbar(settings->Internals.Vscrollbar, control_comand);
		settings->Internals.Hscrollbar->Internals.NoPaintBackGround = true;
		scrollbar(settings->Internals.Hscrollbar, control_comand);

		settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		settings->Caption.Text = settings->Internals.Caption.Text;
		//control_comand->Comand = Control_Entire_Refresh;
		//CursorState cursor = control_comand->Cursor;
		//if(!settings->Internals.IsChildren) control_comand->Cursor = Cursor_Up;
		paint_textbox(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		pDisplay->sClipRegion = back_up_clip;
		//control_comand->Cursor = cursor;
		//control_comand->Comand = ComandBeck;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		return;
	}

	/* Check if inside window */
	bool _inside_window = check_if_inside_box(X_StartBox + 2, Y_StartBox + 2, X_LenBox - 2 - settings->Internals.Size.ScrollSize, Y_LenBox - 2 - settings->Internals.Size.ScrollSize, control_comand->X, control_comand->Y);
	bool __inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!__inside_window) _inside_window = false;

	if(_inside_window == true && control_comand->Cursor == Cursor_Down) settings->Internals.CursorDownInsideTextBox = true;

	bool inside_window = check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	__inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!__inside_window) inside_window = false;
	if((control_comand->Cursor != Cursor_NoAction) &&
				(inside_window == true || settings->Internals.CursorDownInsideBox == true) &&
					settings->Enabled == true &&
						settings->Visible == true)
	{
		settings->Internals.OldStateCursor = control_comand->Cursor;

		if(control_comand->Cursor == Cursor_Down && inside_window == true)
		{
			settings->Events.CursorDown = true;
			settings->Internals.CursorDownInsideBox = true;
			settings->Internals.PenPushX = control_comand->X;
			settings->Internals.PenPushY = control_comand->Y;
			settings->Internals.Start = -1;
			if(settings->Events.OnDown.CallBack)
			{
				settings->Events.OnDown.CallbackReturnData = settings->Events.OnDown.CallBack(settings->Events.OnDown.CallbackData);
			}
		}
		if(control_comand->Cursor == Cursor_Up && inside_window == true && settings->Internals.CursorDownInsideBox == true)
		{
			settings->Events.CursorUp = true;
			if(settings->Events.OnUp.CallBack)
			{
				settings->Events.OnUp.CallbackReturnData = settings->Events.OnUp.CallBack(settings->Events.OnUp.CallbackData);
			}
		}
		if(control_comand->Cursor == Cursor_Move && inside_window == true && settings->Internals.CursorDownInsideBox == true)
		{
			settings->Events.CursorMove = true;
			if(settings->Events.OnMove.CallBack)
			{
				settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings->Events.OnMove.CallbackData);
			}
		}
		CursorState cursor = control_comand->Cursor;
		if(settings->Internals.CursorDownInsideTextBox) control_comand->Cursor = Cursor_Up;
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
		scrollbar(settings->Internals.Vscrollbar, control_comand);
		scrollbar(settings->Internals.Hscrollbar, control_comand);
		control_comand->Cursor = cursor;
		paint_textbox(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		pDisplay->sClipRegion = back_up_clip;
	}

	if(settings->Internals.CursorDownInsideBox == true && control_comand->Cursor == Cursor_Up) settings->Internals.CursorDownInsideBox = false;

	if(control_comand->Cursor == Cursor_Up) settings->Internals.CursorDownInsideTextBox = false;

	control_comand->CursorCoordonateUsed = settings->Internals.CursorDownInsideBox;
	return;
}
//#######################################################################################
tTextBox *new_textbox(tDisplay *ScreenDisplay)
{
	tTextBox* settings = (tTextBox*)calloc(1, sizeof(tTextBox));

	if(!settings || !ScreenDisplay) return NULL;

	settings->Internals.pDisplay = ScreenDisplay;
	settings->Caption.WordWrap = true;
	settings->Caption.Font = controls_color.DefaultFont;
	settings->Caption.Text = "Textbox 1 Salutare si voi ce mai faceti, eu fac bine";
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
	settings->Position.Y = 50;
	settings->Size.X = 70;
	settings->Size.Y = 70;
	settings->Size.ScrollSize = 15;
	settings->StateChangedOn = Cursor_Up;
	settings->Visible = true;

	return settings;
}
//#######################################################################################
bool free_textbox(tTextBox* settings)
{
	if(!settings) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	//progressbar(settings, &comand);
	if(settings->Internals.Caption.Text) free(settings->Internals.Caption.Text);
	if(settings->Caption.Text) free(settings->Caption.Text);
	if(settings) free(settings);
	return true;
}
//#######################################################################################
void textbox_set_caption(void* _settings, char* string)
{
	tTextBox* settings = _settings;
	if(settings->Internals.Caption.Text) free(settings->Internals.Caption.Text);
	settings->Internals.Caption.Text = (char*)malloc(strlen(string)+1);
	if(settings->Internals.Caption.Text) strcpy(settings->Internals.Caption.Text, string);
	settings->Internals.NeedEntireRefresh = true;
}
//#######################################################################################
void textbox_set_font(void* _settings, tFont* font)
{
	tTextBox* settings = _settings;
	settings->Internals.Caption.Font = font;
	settings->Internals.NeedEntireRefresh = true;
}
//#######################################################################################
void textbox_set_wordwrap(void* _settings, bool word_wrap)
{
	tTextBox* settings = _settings;
	settings->Internals.Caption.WordWrap = word_wrap;
	settings->Internals.NeedEntireRefresh = true;
}
//#######################################################################################
//#######################################################################################
//#######################################################################################
//#######################################################################################



