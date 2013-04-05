/*
 *  lib/gfx/scrollbar.c
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
#include "scrollbar.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "../generic.h"
#include "controls_definition.h"
#include "buton.h"
#include "board_properties.h"
//#######################################################################################
static void paint_scrollbar(tScrollBar* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand)
{
	unsigned int color = 0;
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
	if(settings->Internals.NeedEntireRefresh == true || settings->Internals.NeedEntireRepaint == true || settings->Internals.Control.Initiated == false)
	{
		color = controls_color.Control_Color_Enabled_Border_Push;
		put_rectangle(pDisplay, x_start, y_start, x_len, y_len, false, controlls_change_color(color, -2));
		put_rectangle(pDisplay, x_start + 1, y_start + 1, x_len - 2, y_len - 2, true, color);
	}

	signed int ValueBetweenMinAndMax = (settings->Internals.OldMaximum - settings->Internals.OldMinimum);


	tButton* ButtonSettings = settings->Internals.BtnSettings;
	tButton* BtnUpSettings = settings->Internals.BtnUpSettings;
	tButton* BtnDnSettings = settings->Internals.BtnDnSettings;

	if(settings->Size.X < settings->Size.Y)
	{
		BtnUpSettings->Position.X = settings->Internals.Position.X + 2;
		BtnUpSettings->Position.Y = settings->Internals.Position.Y + 2;
		BtnUpSettings->Size.X = settings->Internals.Size.X - 4;
		BtnUpSettings->Size.Y = settings->Internals.Size.X - 4;

		BtnDnSettings->Position.X = settings->Internals.Position.X + 2;
		BtnDnSettings->Position.Y = (settings->Internals.Position.Y + settings->Internals.Size.Y) - (settings->Internals.Size.X - 2);
		BtnDnSettings->Size.X = settings->Internals.Size.X - 4;
		BtnDnSettings->Size.Y = settings->Internals.Size.X - 4;
	}
	else
	{
		/* Left */
		BtnUpSettings->Position.X = settings->Internals.Position.X + 2;
		BtnUpSettings->Position.Y = settings->Internals.Position.Y + 2;
		BtnUpSettings->Size.X = settings->Internals.Size.Y - 4;
		BtnUpSettings->Size.Y = settings->Internals.Size.Y - 4;
		/* Right */
		BtnDnSettings->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - (settings->Internals.Size.Y - 2);
		BtnDnSettings->Position.Y = settings->Internals.Position.Y + 2;
		BtnDnSettings->Size.X = settings->Internals.Size.Y - 4;
		BtnDnSettings->Size.Y = settings->Internals.Size.Y - 4;
	}

	if(control_comand->Cursor != Cursor_NoAction || settings->Internals.NeedEntireRefresh == true || settings->Internals.NeedEntireRepaint == true)
	{
		BtnUpSettings->Internals.NeedEntireRefresh = settings->Internals.NeedEntireRefresh;
		CursorState back = control_comand->Cursor;
		if(BtnUpSettings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
		button((void*)BtnUpSettings, control_comand);
		control_comand->Cursor = back;
	}

	if(control_comand->Cursor != Cursor_NoAction || settings->Internals.NeedEntireRefresh == true || settings->Internals.NeedEntireRepaint == true)
	{
		BtnDnSettings->Internals.NeedEntireRefresh = settings->Internals.NeedEntireRefresh;
		CursorState back = control_comand->Cursor;
		if(BtnDnSettings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
		button((void*)BtnDnSettings, control_comand);
		control_comand->Cursor = back;
	}

	signed int BtnSize = 0;

	if(settings->Size.X < settings->Size.Y) BtnSize = settings->Size.Y - ((settings->Size.X - 2)<<1) - 4 - ValueBetweenMinAndMax;
	else 									BtnSize = settings->Size.X - ((settings->Size.Y - 2)<<1) - 4 - ValueBetweenMinAndMax;

	if(BtnSize < settings->Size.MinBtnSize) BtnSize = settings->Size.MinBtnSize;

	bool scroll_internal_modified = false;
	if(BtnUpSettings->Events.CursorDown || BtnUpSettings->Events.CursorMove || settings->Internals.OldValue != settings->Value)
	{
		BtnUpSettings->Events.CursorDown = false;
		BtnUpSettings->Events.CursorMove = false;
		if(settings->Value > settings->Minimum)
		{
			if(settings->Internals.OldValue == settings->Value)
			{
				settings->Value--;
				scroll_internal_modified = true;
				//settings->Internals.NeedEntireRefresh = true;
			}
			else
			{
				settings->Internals.NeedEntireRefresh = false;
				control_comand->CursorCoordonateUsed = true;
			}
		}
	}
	if(BtnDnSettings->Events.CursorDown || BtnDnSettings->Events.CursorMove || settings->Internals.OldValue != settings->Value)
	{
		BtnDnSettings->Events.CursorDown = false;
		BtnDnSettings->Events.CursorMove = false;
		if(settings->Value < settings->Maximum)
		{
			if(settings->Internals.OldValue == settings->Value)
			{
				settings->Value++;
				scroll_internal_modified = true;
				//settings->Internals.NeedEntireRefresh = true;
			}
			else
			{
				settings->Internals.NeedEntireRefresh = false;
				control_comand->CursorCoordonateUsed = true;
			}
		}
	}

	bool CursorBtnSelfModified = false;
	if(ValueBetweenMinAndMax < 1)
	{
		ValueBetweenMinAndMax = 1;
		if(settings->Size.X < settings->Size.Y)
		{
			ButtonSettings->Position.X = settings->Position.X + 2;
			ButtonSettings->Position.Y = settings->Position.Y + 2 + (settings->Size.X - 2);
			ButtonSettings->Size.X = settings->Size.X - 4;
			ButtonSettings->Size.Y = settings->Size.Y - 4 - ((settings->Size.X - 2)<<1);
		}
		else
		{
			ButtonSettings->Position.X = settings->Position.X + 2 + (settings->Size.Y - 2);
			ButtonSettings->Position.Y = settings->Position.Y + 2;
			ButtonSettings->Size.X = settings->Size.X - 4 - ((settings->Size.Y - 2)<<1);
			ButtonSettings->Size.Y = settings->Size.Y - 4;
		}
	}
	else
	{
		if(settings->Size.X < settings->Size.Y)
		{
			ButtonSettings->Size.Y = BtnSize;
			//signed int Steps = settings->Size.Y - 4 - BtnSize;
			ButtonSettings->Position.X = settings->Position.X + 2;
			//ButtonSettings->Position.Y = settings->Position.Y + 2;
			ButtonSettings->Size.X = settings->Size.X - 4;
			//ButtonSettings->Size.Y = settings->Size.Y - 4;
			if(ButtonSettings->Events.CursorDown == true)
			{
				settings->Internals.CoordonateOfTouchDown = control_comand->Y;
				settings->Internals.CoordonateOfButtonDown = ButtonSettings->Position.Y;
				ButtonSettings->Events.CursorDown = false;
				settings->Internals.OffsetButtonCoord = ButtonSettings->Position.Y - settings->Position.Y - (settings->Size.X - 2);
			}
			else if(control_comand->Cursor == Cursor_Move && settings->Internals.CursorDownInsideBox == true && ButtonSettings->Internals.CursorDownInsideBox == true)
			{
				ButtonSettings->Position.Y = settings->Internals.CoordonateOfButtonDown + (control_comand->Y - settings->Internals.CoordonateOfTouchDown);

				if(ButtonSettings->Position.Y < settings->Position.Y + (settings->Size.X - 2) + 2) ButtonSettings->Position.Y = settings->Position.Y + 2 + (settings->Size.X - 2);
				else if((ButtonSettings->Position.Y + ButtonSettings->Size.Y) > (settings->Position.Y + settings->Size.Y) - 2  - (settings->Size.X - 2)) ButtonSettings->Position.Y = ((settings->Position.Y + settings->Size.Y) - 2) - ButtonSettings->Size.Y   - (settings->Size.X - 2);

				settings->Internals.OffsetButtonCoord = ButtonSettings->Position.Y - settings->Position.Y - (settings->Size.X - 2);

				//if(BtnSize > settings->Size.MinBtnSize) settings->Value = (ButtonSettings->Position.Y - (settings->Position.Y + 2)) + settings->Minimum;
				/*else */
				settings->Value = percentage_to(settings->Minimum, settings->Maximum, settings->Size.Y - 4 - BtnSize - ((settings->Size.X - 2)<<1), (ButtonSettings->Position.Y - (settings->Position.Y + 2) - (settings->Size.X - 2)));

				CursorBtnSelfModified = true;
			}
		}
		else
		{
			ButtonSettings->Size.X = BtnSize;
			//signed int Steps = settings->Size.X - 4 - BtnSize;
			ButtonSettings->Position.Y = settings->Position.Y + 2;
			//ButtonSettings->Position.X = settings->Position.X + 2;
			ButtonSettings->Size.Y = settings->Size.Y - 4;
			//ButtonSettings->Size.X = settings->Size.X - 4;
			if(ButtonSettings->Events.CursorDown == true)
			{
				settings->Internals.CoordonateOfTouchDown = control_comand->X;
				settings->Internals.CoordonateOfButtonDown = ButtonSettings->Position.X;
				ButtonSettings->Events.CursorDown = false;
				settings->Internals.OffsetButtonCoord = ButtonSettings->Position.X - settings->Position.X - (settings->Size.Y - 2);
			}
			else if(control_comand->Cursor == Cursor_Move && settings->Internals.CursorDownInsideBox == true && ButtonSettings->Internals.CursorDownInsideBox == true)
			{
				ButtonSettings->Position.X = settings->Internals.CoordonateOfButtonDown + (control_comand->X - settings->Internals.CoordonateOfTouchDown);

				if(ButtonSettings->Position.X < settings->Position.X + (settings->Size.Y - 2) + 2) ButtonSettings->Position.X = settings->Position.X + 2 + (settings->Size.Y - 2);
				else if((ButtonSettings->Position.X + ButtonSettings->Size.X) > (settings->Position.X + settings->Size.X) - 2  - (settings->Size.Y - 2)) ButtonSettings->Position.X = ((settings->Position.X + settings->Size.X) - 2) - ButtonSettings->Size.X   - (settings->Size.Y - 2);

				settings->Internals.OffsetButtonCoord = ButtonSettings->Position.X - settings->Position.X - (settings->Size.Y - 2);

				//if(BtnSize > settings->Size.MinBtnSize) settings->Value = (ButtonSettings->Position.X - (settings->Position.X + 2)) + settings->Minimum;
				/*else */
				settings->Value = percentage_to(settings->Minimum, settings->Maximum, settings->Size.X - 4 - BtnSize - ((settings->Size.Y - 2)<<1), (ButtonSettings->Position.X - (settings->Position.X + 2) - (settings->Size.Y - 2)));

				CursorBtnSelfModified = true;
			}
		}
	}
	if(settings->Internals.OldValue != settings->Value)
	{
		settings->Internals.OldValue = settings->Value;
		settings->Events.ValueChanged = true;
		if(settings->Events.OnValueChanged.CallBack)
		{
			settings->Events.OnValueChanged.CallbackReturnData = settings->Events.OnValueChanged.CallBack(settings->Events.OnValueChanged.CallbackData);
		}
	}

	if(settings->Enabled) ButtonSettings->Color.Scren = settings->Color.Enabled.Buton.Pull;
	else  ButtonSettings->Color.Scren = settings->Color.Disabled.Buton;
	if(BtnSize > settings->Size.MinBtnSize)
	{
		if(settings->Size.X < settings->Size.Y) ButtonSettings->Position.Y = (settings->Value + (settings->Position.Y + 2) + (settings->Size.X - 2)) - settings->Minimum;
		else 									ButtonSettings->Position.X = (settings->Value + (settings->Position.X + 2) + (settings->Size.Y - 2)) - settings->Minimum;
	}
	else
	{
		if(settings->Size.X < settings->Size.Y) ButtonSettings->Position.Y = (settings->Position.Y + 2) + (settings->Size.X - 2) + to_percentage(settings->Minimum, settings->Maximum, settings->Size.Y - ((settings->Size.X - 2)<<1) - 4 - settings->Size.MinBtnSize, settings->Value);
		else 									ButtonSettings->Position.X = (settings->Position.X + 2) + (settings->Size.Y - 2) + to_percentage(settings->Minimum, settings->Maximum, settings->Size.X - ((settings->Size.Y - 2)<<1) - 4 - settings->Size.MinBtnSize, settings->Value);
	}

	if(settings->Internals.NeedEntireRefresh == true || settings->Internals.NeedEntireRepaint == true)
	{
		ButtonSettings->Internals.NoPaintBackGround = true;
		ButtonSettings->Internals.NeedEntireRefresh = true;
		CursorState back = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		button((void*)ButtonSettings, control_comand);
		control_comand->Cursor = back;
		ButtonSettings->Internals.NoPaintBackGround = false;
	}
	else if(ButtonSettings->Position.X != ButtonSettings->Internals.Position.X || ButtonSettings->Position.Y != ButtonSettings->Internals.Position.Y)
	{
		ButtonSettings->Internals.NeedEntireRefresh = true;
		CursorState back = control_comand->Cursor;
		if(scroll_internal_modified) control_comand->Cursor = Cursor_Up;
		button((void*)ButtonSettings, control_comand);
		control_comand->Cursor = back;
		ButtonSettings->Internals.NoPaintBackGround = false;
	}
	else
	{
		CursorState back = control_comand->Cursor;
		if(CursorBtnSelfModified)  control_comand->Cursor = Cursor_Down;
		button((void*)ButtonSettings, control_comand);
		control_comand->Cursor = back;
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
	settings->Internals.NeedEntireRefresh = false;
}
//#######################################################################################
void scrollbar(tScrollBar *settings, tControlCommandData* control_comand)
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
		settings->Internals.OldMinimum = settings->Minimum;
		settings->Internals.OldMaximum = settings->Maximum;
		settings->Internals.OldValue = settings->Value;

		settings->Internals.BtnSettings = new_button(settings->Internals.pDisplay);
		tButton* ButtonSettings = settings->Internals.BtnSettings;
		ButtonSettings->Internals.NoPaintBackGround = false;
		ButtonSettings->Internals.ContinuouslyPushTimerDisabled = true;

		settings->Internals.BtnUpSettings = new_button(settings->Internals.pDisplay);
		tButton* BtnUpSettings = settings->Internals.BtnUpSettings;
		BtnUpSettings->Internals.NoPaintBackGround = true;

		settings->Internals.BtnDnSettings = new_button(settings->Internals.pDisplay);
		tButton* BtnDnSettings = settings->Internals.BtnDnSettings;
		BtnDnSettings->Internals.NoPaintBackGround = true;
		if(settings->Size.X < settings->Size.Y)
		{
			settings->Internals.BtnSettings->Position.Y = settings->Internals.Position.Y + 2 + (settings->Size.X - 2);
			settings->Internals.OffsetButtonCoord = ButtonSettings->Position.Y - settings->Position.Y - (settings->Size.X - 2);

			BtnUpSettings->Position.X = settings->Internals.Position.X + 2;
			BtnUpSettings->Position.Y = settings->Internals.Position.Y + 2;
			BtnUpSettings->Size.X = settings->Internals.Size.X - 4;
			BtnUpSettings->Size.Y = settings->Internals.Size.X - 4;

			BtnDnSettings->Position.X = settings->Internals.Position.X + 2;
			BtnDnSettings->Position.Y = (settings->Internals.Position.Y + settings->Internals.Size.Y) - (settings->Internals.Size.X - 2);
			BtnDnSettings->Size.X = settings->Internals.Size.X - 4;
			BtnDnSettings->Size.Y = settings->Internals.Size.X - 4;
		}
		else
		{
			settings->Internals.BtnSettings->Position.X = settings->Internals.Position.X + 2 + (settings->Size.Y - 2);
			settings->Internals.OffsetButtonCoord = ButtonSettings->Position.X - settings->Position.X - (settings->Size.Y - 2);
			/* Left */
			BtnUpSettings->Position.X = settings->Internals.Position.X + 2;
			BtnUpSettings->Position.Y = settings->Internals.Position.Y + 2;
			BtnUpSettings->Size.X = settings->Internals.Size.Y - 4;
			BtnUpSettings->Size.Y = settings->Internals.Size.Y - 4;
			/* Right */
			BtnDnSettings->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - (settings->Internals.Size.Y - 2);
			BtnDnSettings->Position.Y = settings->Internals.Position.Y + 2;
			BtnDnSettings->Size.X = settings->Internals.Size.Y - 4;
			BtnDnSettings->Size.Y = settings->Internals.Size.Y - 4;
		}
		//settings->Internals.BtnSettings->Color.Scren = settings->Color.Enabled.Buton.Pull;
	}
	/* Verify if position on size has been modified */
	if(settings->Position.X != settings->Internals.Position.X ||
			settings->Position.Y != settings->Internals.Position.Y ||
				settings->Size.X != settings->Internals.Size.X ||
					settings->Size.Y != settings->Internals.Size.Y ||
						settings->Size.MinBtnSize != settings->Internals.Size.MinBtnSize ||
							settings->Minimum != settings->Internals.OldMinimum||
								settings->Maximum != settings->Internals.OldMaximum ||
									settings->Value != settings->Internals.OldValue)
										settings->Internals.NeedEntireRefresh = true;

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
		settings->Internals.Size.MinBtnSize = settings->Size.MinBtnSize;
		settings->Internals.OldMinimum = settings->Minimum;
		settings->Internals.OldMaximum = settings->Maximum;
		if(settings->Value > settings->Maximum) settings->Value = settings->Internals.OldMaximum;
		if(settings->Value < settings->Minimum) settings->Value = settings->Internals.OldMinimum;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		//CursorState _cursor = control_comand->Cursor;
		//control_comand->Cursor = Cursor_Up;
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
		paint_scrollbar(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		pDisplay->sClipRegion = back_up_clip;
		//control_comand->Cursor = _cursor;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
	}
	if(settings->Internals.OldStateCursor != control_comand->Cursor &&
					settings->Enabled == true &&
						settings->Visible == true)
	{
		settings->Internals.OldStateCursor = control_comand->Cursor;
	}
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
	if(control_comand->Cursor)paint_scrollbar(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
	pDisplay->sClipRegion = back_up_clip;
	bool inside_window = check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;
	if(inside_window == true && control_comand->Cursor == Cursor_Down) settings->Internals.CursorDownInsideBox = true;
	if(settings->Internals.CursorDownInsideBox == true && control_comand->Cursor == Cursor_Up) settings->Internals.CursorDownInsideBox = false;
	control_comand->CursorCoordonateUsed = settings->Internals.CursorDownInsideBox;
}
//#######################################################################################
tScrollBar *new_scrollbar(tDisplay *ScreenDisplay)
{
	tScrollBar* settings = (tScrollBar*)calloc(1, sizeof(tScrollBar));

	if(!settings || !ScreenDisplay) return NULL;

	settings->Internals.pDisplay = ScreenDisplay;

	settings->Color.Scren = controls_color.Scren;
	settings->Color.Enabled.BackGround = controls_color.Control_Color_Enabled_BackGround;
	settings->Color.Enabled.Border.Pull = controls_color.Control_Color_Enabled_Border_Pull;
	settings->Color.Enabled.Border.Push = controls_color.Control_Color_Enabled_Border_Push;
	settings->Color.Enabled.Buton.Pull = controls_color.Control_Color_Enabled_Buton_Pull;
	settings->Color.Enabled.Buton.Push = controls_color.Control_Color_Enabled_Buton_Push;

	settings->Color.Disabled.BackGround = controls_color.Control_Color_Disabled_BackGround;
	settings->Color.Disabled.Border = controls_color.Control_Color_Disabled_Border_Pull;
	settings->Color.Disabled.Buton = controls_color.Control_Color_Disabled_Buton_Pull;

	settings->Enabled = true;
	settings->Position.X = 30;
	settings->Position.Y = 100;
	settings->Size.X = 20;
	settings->Size.Y = 60;
	settings->Size.MinBtnSize = 9;
	settings->StateChangedOn = Cursor_Up;
	settings->Visible = true;
	return settings;
}
//#######################################################################################
bool free_scrollbar(tScrollBar *settings)
{
	if(!settings) return false;

	if(!free_button(settings->Internals.BtnDnSettings)) return false;
	if(!free_button(settings->Internals.BtnSettings)) return false;
	if(!free_button(settings->Internals.BtnUpSettings)) return false;

	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	scrollbar(settings, &comand);
	if(settings) free(settings);
	return true;
}
//#######################################################################################
