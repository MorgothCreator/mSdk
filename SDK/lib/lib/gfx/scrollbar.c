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
#include <stdlib.h>
#include "scrollbar.h"
#include "scrollbar_def.h"
#include "button.h"
#include "button_def.h"
#include "window_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "../generic.h"
#include "controls_definition.h"
#include "sys/plat_properties.h"
#include "gfx_gui_paint.h"
//#######################################################################################
static void paint_scrollbar(tScrollBar* settings, void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
	if(settings->Internals.NeedEntireRefresh == true || settings->Internals.NeedEntireRepaint == true || settings->Internals.Control.Initiated == false)
	{
		if(settings->Enabled)
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, settings->Color.Enabled.BackGround, controls_color.Control_Color_Enabled_Border_Push, Cursor_Down,PAINT_STYLE_ROUNDED_CORNERS , true);
		else
			gui_put_item(pDisplay, x_start, y_start, x_len, y_len, settings->Color.Disabled.BackGround, settings->Color.Disabled.Border, Cursor_NoAction,PAINT_STYLE_ROUNDED_CORNERS , true);
		control_comand->WindowRefresh |= true;
	}

	signed int ValueBetweenMinAndMax = (settings->Internals.OldMaximum - settings->Internals.OldMinimum);


	tButton* ButtonSettings = settings->Internals.BtnSettings;
	tButton* BtnUpSettings = settings->Internals.BtnUpSettings;
	tButton* BtnDnSettings = settings->Internals.BtnDnSettings;

	if(settings->Size.X < settings->Size.Y)
	{
		BtnUpSettings->Position.X = 2;
		BtnUpSettings->Position.Y = 2;
		BtnUpSettings->Internals.PositionOffset.X = x_start - ParentWindow->Internals.Position.X;
		BtnUpSettings->Internals.PositionOffset.Y = y_start - ParentWindow->Internals.Position.Y;
		BtnUpSettings->Size.X = settings->Internals.Size.X - 4;
		BtnUpSettings->Size.Y = settings->Internals.Size.X - 4;

		BtnDnSettings->Position.X = 2;
		BtnDnSettings->Position.Y = settings->Internals.Size.Y - (settings->Internals.Size.X - 2);
		BtnDnSettings->Internals.PositionOffset.X = x_start - ParentWindow->Internals.Position.X;
		BtnDnSettings->Internals.PositionOffset.Y = y_start - ParentWindow->Internals.Position.Y;
		BtnDnSettings->Size.X = settings->Internals.Size.X - 4;
		BtnDnSettings->Size.Y = settings->Internals.Size.X - 4;
	}
	else
	{
		/* Left */
		BtnUpSettings->Position.X = 2;
		BtnUpSettings->Position.Y = 2;
		BtnUpSettings->Internals.PositionOffset.X = x_start - ParentWindow->Internals.Position.X;
		BtnUpSettings->Internals.PositionOffset.Y = y_start - ParentWindow->Internals.Position.Y;
		BtnUpSettings->Size.X = settings->Internals.Size.Y - 4;
		BtnUpSettings->Size.Y = settings->Internals.Size.Y - 4;
		/* Right */
		BtnDnSettings->Position.X = settings->Internals.Size.X - (settings->Internals.Size.Y - 2);
		BtnDnSettings->Position.Y = 2;
		BtnDnSettings->Internals.PositionOffset.X = x_start - ParentWindow->Internals.Position.X;
		BtnDnSettings->Internals.PositionOffset.Y = y_start - ParentWindow->Internals.Position.Y;
		BtnDnSettings->Size.X = settings->Internals.Size.Y - 4;
		BtnDnSettings->Size.Y = settings->Internals.Size.Y - 4;
	}

	if(control_comand->Cursor != Cursor_NoAction || settings->Internals.NeedEntireRefresh == true || settings->Internals.NeedEntireRepaint == true)
	{
		BtnUpSettings->Internals.NeedEntireRefresh = settings->Internals.NeedEntireRefresh;
		BtnUpSettings->Enabled = settings->Enabled;
		CursorState back = control_comand->Cursor;
		if(BtnUpSettings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
		button((void*)BtnUpSettings, control_comand);
		//control_comand->Cursor = back;

		BtnDnSettings->Internals.NeedEntireRefresh = settings->Internals.NeedEntireRefresh;
		BtnDnSettings->Enabled = settings->Enabled;
		//CursorState back = control_comand->Cursor;
		//if(BtnDnSettings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;
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
				//control_comand->WindowRefresh |= true;
				//settings->Internals.NeedEntireRefresh = true;
			}
			else
			{
				settings->Internals.NeedEntireRefresh = false;
				//control_comand->CursorCoordonateUsed = true;
				//control_comand->WindowRefresh |= true;
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
				//control_comand->WindowRefresh |= true;
				//settings->Internals.NeedEntireRefresh = true;
			}
			else
			{
				settings->Internals.NeedEntireRefresh = false;
				//control_comand->CursorCoordonateUsed = true;
				//control_comand->WindowRefresh |= true;
			}
		}
	}

	bool CursorBtnSelfModified = false;
	if(ValueBetweenMinAndMax < 1)
	{
		ValueBetweenMinAndMax = 1;
		if(settings->Size.X < settings->Size.Y)
		{
			ButtonSettings->Position.X = 2;
			ButtonSettings->Position.Y = settings->Size.X;
			//ButtonSettings->Position.X = settings->Position.X + 2;
			//ButtonSettings->Position.Y = settings->Position.Y + 2 + (settings->Size.X - 2);
			ButtonSettings->Size.X = settings->Size.X - 4;
			ButtonSettings->Size.Y = settings->Size.Y - 4 - ((settings->Size.X - 2)<<1);
		}
		else
		{
			ButtonSettings->Position.X = settings->Size.Y;
			ButtonSettings->Position.Y = 2;
			//ButtonSettings->Position.X = settings->Position.X + 2 + (settings->Size.Y - 2);
			//ButtonSettings->Position.Y = settings->Position.Y + 2;
			ButtonSettings->Size.X = settings->Size.X - 4 - ((settings->Size.Y - 2)<<1);
			ButtonSettings->Size.Y = settings->Size.Y - 4;
		}
	}
	else
	{
		if(control_comand->CursorCoordonateUsed == false&&
				settings->Enabled == true &&
					settings->Visible == true)
		{
			if(settings->Size.X < settings->Size.Y)
			{
				ButtonSettings->Size.Y = BtnSize;
				////signed int Steps = settings->Size.Y - 4 - BtnSize;
				//ButtonSettings->Position.X = settings->Position.X + 2;
				////ButtonSettings->Position.Y = settings->Position.Y + 2;
				ButtonSettings->Size.X = settings->Size.X - 4;
				//ButtonSettings->Size.Y = settings->Size.Y - 4;
				if(ButtonSettings->Events.CursorDown == true)
				{
					settings->Internals.CoordonateOfTouchDown = control_comand->Y;
					settings->Internals.CoordonateOfButtonDown = ButtonSettings->Internals.Position.Y - settings->Internals.PositionOffset.Y;
					ButtonSettings->Events.CursorDown = false;
				}
				else if(control_comand->Cursor == Cursor_Move && settings->Internals.CursorDownInsideBox == true && ButtonSettings->Internals.CursorDownInsideBox == true)
				{
					ButtonSettings->Position.Y = settings->Internals.CoordonateOfButtonDown + ((control_comand->Y - (ParentWindow->Internals.Position.Y)) - settings->Internals.CoordonateOfTouchDown);

					if(ButtonSettings->Position.Y < settings->Position.Y + (settings->Size.X - 2) + 2) ButtonSettings->Position.Y = settings->Position.Y + 2 + (settings->Size.X - 2);
					else if((ButtonSettings->Position.Y + ButtonSettings->Size.Y) > (settings->Position.Y + settings->Size.Y) - 2  - (settings->Size.X - 2)) ButtonSettings->Position.Y = ((settings->Position.Y + settings->Size.Y) - 2) - ButtonSettings->Size.Y   - (settings->Size.X - 2);

					settings->Value = percentage_to(settings->Minimum, settings->Maximum, settings->Size.Y - 4 - BtnSize - ((settings->Size.X - 2)<<1), (ButtonSettings->Position.Y - (settings->Position.Y + 2) - (settings->Size.X - 2)));

					CursorBtnSelfModified = true;
				}
			}
			else
			{
				ButtonSettings->Size.X = BtnSize;
				////signed int Steps = settings->Size.X - 4 - BtnSize;
				//ButtonSettings->Position.Y = settings->Position.Y + 2;
				////ButtonSettings->Position.X = settings->Position.X + 2;
				ButtonSettings->Size.Y = settings->Size.Y - 4;
				//ButtonSettings->Size.X = settings->Size.X - 4;
				if(ButtonSettings->Events.CursorDown == true)
				{
					settings->Internals.CoordonateOfTouchDown = control_comand->X;
					settings->Internals.CoordonateOfButtonDown = ButtonSettings->Internals.Position.X;
					ButtonSettings->Events.CursorDown = false;
				}
				else if(control_comand->Cursor == Cursor_Move && settings->Internals.CursorDownInsideBox == true && ButtonSettings->Internals.CursorDownInsideBox == true)
				{
					ButtonSettings->Position.X = settings->Internals.CoordonateOfButtonDown + ((control_comand->X - (ParentWindow->Internals.Position.X)) - settings->Internals.CoordonateOfTouchDown);

					if(ButtonSettings->Position.X < settings->Position.X + (settings->Size.Y - 2) + 2) ButtonSettings->Position.X = settings->Position.X + 2 + (settings->Size.Y - 2);
					else if((ButtonSettings->Position.X + ButtonSettings->Size.X) > (settings->Position.X + settings->Size.X) - 2  - (settings->Size.Y - 2)) ButtonSettings->Position.X = ((settings->Position.X + settings->Size.X) - 2) - ButtonSettings->Size.X   - (settings->Size.Y - 2);

					settings->Value = percentage_to(settings->Minimum, settings->Maximum, settings->Size.X - 4 - BtnSize - ((settings->Size.Y - 2)<<1), (ButtonSettings->Position.X - (settings->Position.X + 2) - (settings->Size.Y - 2)));

					CursorBtnSelfModified = true;
				}
			}
		}
	}

	bool ValueIsChangedExternaly = false;
	if(settings->Internals.OldValue != settings->Value)
	{
		settings->Internals.OldValue = settings->Value;
		//settings->Events.ValueChanged = true;
		ValueIsChangedExternaly = true;
		settings->Events.ValueChanged = true;
		if(settings->Events.OnValueChanged.CallBack)
		{
			settings->Events.OnValueChanged.CallbackReturnData = settings->Events.OnValueChanged.CallBack(settings->Events.OnValueChanged.CallbackData);
		}
	}

	//if(settings->Enabled) ButtonSettings->Color.Scren = settings->Color.Enabled.BackGround;
	//else  ButtonSettings->Color.Scren = settings->Color.Disabled.BackGround;
	ButtonSettings->Color.Scren = settings->Color.Scren;
	if(BtnSize > settings->Size.MinBtnSize)
	{
		if(settings->Size.X < settings->Size.Y) ButtonSettings->Position.Y = (settings->Value + 2 + (settings->Size.X - 2)) - settings->Minimum;
		else 									ButtonSettings->Position.X = (settings->Value + 2 + (settings->Size.Y - 2)) - settings->Minimum;
	}
	else
	{
		if(settings->Size.X < settings->Size.Y) ButtonSettings->Position.Y = 2 + (settings->Size.X - 2) + to_percentage(settings->Minimum, settings->Maximum, settings->Size.Y - ((settings->Size.X - 2)<<1) - 4 - settings->Size.MinBtnSize, settings->Value);
		else 									ButtonSettings->Position.X = 2 + (settings->Size.Y - 2) + to_percentage(settings->Minimum, settings->Maximum, settings->Size.X - ((settings->Size.Y - 2)<<1) - 4 - settings->Size.MinBtnSize, settings->Value);
	}

	if(settings->Internals.NeedEntireRefresh == true || settings->Internals.NeedEntireRepaint == true)
	{
		ButtonSettings->Internals.NoPaintBackGround = true;
		ButtonSettings->Internals.NeedEntireRefresh = true;
		ButtonSettings->Enabled = settings->Enabled;
		CursorState back = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		button((void*)ButtonSettings, control_comand);
		control_comand->Cursor = back;
		ButtonSettings->Internals.NoPaintBackGround = false;
	}
	else if(ButtonSettings->Position.X + ParentWindow->Internals.Position.X + ButtonSettings->Internals.PositionOffset.X != ButtonSettings->Internals.Position.X ||
			ButtonSettings->Position.Y + ParentWindow->Internals.Position.Y + ButtonSettings->Internals.PositionOffset.Y != ButtonSettings->Internals.Position.Y)
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
		if(ValueIsChangedExternaly)
		{
			ButtonSettings->Internals.NeedEntireRefresh = true;
			control_comand->Cursor = Cursor_Up;
		}
		button((void*)ButtonSettings, control_comand);
		control_comand->Cursor = back;
	}

	LcdStruct->sClipRegion.sXMin = x_start;
	LcdStruct->sClipRegion.sYMin = y_start;
	LcdStruct->sClipRegion.sXMax = x_start + x_len;
	LcdStruct->sClipRegion.sYMax = y_start + y_len;
	clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, x_start, y_start, x_len, y_len);
	LcdStruct->sClipRegion = back_up_clip;
	settings->Internals.NeedEntireRefresh = false;
}
//#######################################################################################
void scrollbar(tScrollBar *settings, tControlCommandData* control_comand)
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
		settings->Internals.OldMinimum = settings->Minimum;
		settings->Internals.OldMaximum = settings->Maximum;
		settings->Internals.OldValue = settings->Value;

		settings->Internals.BtnSettings = new_button(settings->Internals.ParentWindow);
		tButton* ButtonSettings = settings->Internals.BtnSettings;
		ButtonSettings->Internals.NoPaintBackGround = false;
		ButtonSettings->Internals.ContinuouslyPushTimerDisabled = true;
		ButtonSettings->Enabled = settings->Enabled;
		ButtonSettings->Color.Scren = settings->Color.Scren;
		ButtonSettings->Color.Enabled.BackGround = settings->Color.Enabled.BackGround;
		ButtonSettings->Caption.Text = str_clear(ButtonSettings->Caption.Text);

		settings->Internals.BtnUpSettings = new_button(settings->Internals.ParentWindow);
		tButton* BtnUpSettings = settings->Internals.BtnUpSettings;
		BtnUpSettings->Internals.NoPaintBackGround = true;
		BtnUpSettings->Enabled = settings->Enabled;
		BtnUpSettings->Color.Scren = settings->Color.Scren;
		BtnUpSettings->Color.Enabled.BackGround = settings->Color.Enabled.BackGround;
		BtnUpSettings->Caption.Text = str_clear(BtnUpSettings->Caption.Text);

		settings->Internals.BtnDnSettings = new_button(settings->Internals.ParentWindow);
		tButton* BtnDnSettings = settings->Internals.BtnDnSettings;
		BtnDnSettings->Internals.NoPaintBackGround = true;
		BtnDnSettings->Enabled = settings->Enabled;
		BtnDnSettings->Color.Scren = settings->Color.Scren;
		BtnDnSettings->Color.Enabled.BackGround = settings->Color.Enabled.BackGround;
		BtnDnSettings->Caption.Text = str_clear(BtnDnSettings->Caption.Text);

		if(settings->Size.X < settings->Size.Y)
		{
			settings->Internals.BtnSettings->Position.X = 2;
			settings->Internals.BtnSettings->Position.Y = settings->Size.X;
			settings->Internals.BtnSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y - ParentWindow->Internals.Position.Y;
			settings->Internals.BtnSettings->Internals.PositionOffset.X = settings->Internals.Position.X - ParentWindow->Internals.Position.X;
			settings->Internals.BtnSettings->Size.X = settings->Internals.Size.X - 4;
			//settings->Internals.BtnSettings->Size.Y = settings->Internals.Size.X - 4;

			BtnUpSettings->Position.X = 2;
			BtnUpSettings->Position.Y = 2;
			BtnUpSettings->Internals.PositionOffset.X = settings->Internals.Position.X;
			BtnUpSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y;
			BtnUpSettings->Size.X = settings->Internals.Size.X - 4;
			BtnUpSettings->Size.Y = settings->Internals.Size.X - 4;

			BtnDnSettings->Position.X = 2;
			BtnDnSettings->Position.Y = settings->Internals.Size.Y - (settings->Internals.Size.X - 2);
			BtnDnSettings->Internals.PositionOffset.X = settings->Internals.Position.X;
			BtnDnSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y;
			BtnDnSettings->Size.X = settings->Internals.Size.X - 4;
			BtnDnSettings->Size.Y = settings->Internals.Size.X - 4;
		}
		else
		{
			settings->Internals.BtnSettings->Position.X = settings->Size.Y;
			settings->Internals.BtnSettings->Position.Y = 2;
			settings->Internals.BtnSettings->Internals.PositionOffset.X = settings->Internals.Position.X - ParentWindow->Internals.Position.X;
			settings->Internals.BtnSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y - ParentWindow->Internals.Position.Y;
			//settings->Internals.BtnSettings->Size.X = settings->Internals.Size.Y - 4;
			settings->Internals.BtnSettings->Size.Y = settings->Internals.Size.Y - 4;
			/* Left */
			BtnUpSettings->Position.X = 2;
			BtnUpSettings->Position.Y = 2;
			BtnUpSettings->Internals.PositionOffset.X = settings->Internals.Position.X;
			BtnUpSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y;
			BtnUpSettings->Size.X = settings->Internals.Size.Y - 4;
			BtnUpSettings->Size.Y = settings->Internals.Size.Y - 4;
			/* Right */
			BtnDnSettings->Position.X = settings->Internals.Size.X - (settings->Internals.Size.Y - 2);
			BtnDnSettings->Position.Y = 2;
			BtnDnSettings->Internals.PositionOffset.X = settings->Internals.Position.X;
			BtnDnSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y;
			BtnDnSettings->Size.X = settings->Internals.Size.Y - 4;
			BtnDnSettings->Size.Y = settings->Internals.Size.Y - 4;
		}
		//settings->Internals.BtnSettings->Color.Scren = settings->Color.Enabled.Buton.Pull;
	}
	if(settings->Value > settings->Maximum) settings->Value = settings->Maximum;
	if(settings->Value < settings->Minimum) settings->Value = settings->Minimum;
	/* Verify if position on size has been modified */
	if(ParentWindow)
	{
		if(((settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X) != settings->Internals.Position.X ||
				(settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y) != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Size.MinBtnSize != settings->Internals.Size.MinBtnSize ||
								settings->Minimum != settings->Internals.OldMinimum||
									settings->Maximum != settings->Internals.OldMaximum ||
										settings->Value != settings->Internals.OldValue ||
											settings->Internals.OldStateEnabled != settings->Enabled ||
												ParentWindow->Internals.OldStateEnabled != settings->Internals.ParentWindowStateEnabled) &&
													settings->Visible == true)
														settings->Internals.NeedEntireRefresh = true;
	}
	else
	{
		if((settings->Position.X != settings->Internals.Position.X ||
			settings->Position.Y != settings->Internals.Position.Y ||
				settings->Size.X != settings->Internals.Size.X ||
					settings->Size.Y != settings->Internals.Size.Y ||
						settings->Size.MinBtnSize != settings->Internals.Size.MinBtnSize ||
							settings->Minimum != settings->Internals.OldMinimum||
								settings->Maximum != settings->Internals.OldMaximum ||
									settings->Internals.OldStateEnabled != settings->Enabled ||
										settings->Value != settings->Internals.OldValue) &&
											settings->Visible == true)
												settings->Internals.NeedEntireRefresh = true;
	}

	signed int X_StartBox = settings->Internals.Position.X;
	signed int Y_StartBox = settings->Internals.Position.Y;
	signed int X_LenBox = settings->Internals.Size.X;
	signed int Y_LenBox = settings->Internals.Size.Y;
	void *pDisplay = settings->Internals.pDisplay;
	tDisplay* LcdStruct = (tDisplay *) pDisplay;

	/*Clear background of box with actual painted dimensions and positions if they been changed*/
	if(settings->Internals.NeedEntireRefresh == true || settings->Internals.OldStateVisible != settings->Visible)
	{
		if(!settings->Internals.NoPaintBackGround || !settings->Visible)
		{
			settings->Internals.OldStateVisible = settings->Visible;
			tRectangle back_up_clip = LcdStruct->sClipRegion;
			LcdStruct->sClipRegion.sXMin = X_StartBox;
			LcdStruct->sClipRegion.sYMin = Y_StartBox;
			LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
			LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
			clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
			LcdStruct->lcd_func.put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, settings->Color.Scren);
			LcdStruct->lcd_func.box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			LcdStruct->sClipRegion = back_up_clip;
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
		settings->Internals.Size.MinBtnSize = settings->Size.MinBtnSize;
		settings->Internals.OldMinimum = settings->Minimum;
		settings->Internals.OldMaximum = settings->Maximum;
		if((settings->Size.X == 0 || settings->Size.Y == 0) && settings->Internals.Control.Initiated == true ) return;
		if(settings->Value > settings->Maximum) settings->Value = settings->Internals.OldMaximum;
		if(settings->Value < settings->Minimum) settings->Value = settings->Internals.OldMinimum;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		//CursorState _cursor = control_comand->Cursor;
		//control_comand->Cursor = Cursor_Up;
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&LcdStruct->sClipRegion, &back_up_clip);

		if(settings->Size.X < settings->Size.Y)
		{
			settings->Internals.BtnSettings->Position.X = 2;
			settings->Internals.BtnSettings->Position.Y = settings->Size.X;
			settings->Internals.BtnSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y - ParentWindow->Internals.Position.Y;
			settings->Internals.BtnSettings->Internals.PositionOffset.X = settings->Internals.Position.X - ParentWindow->Internals.Position.X;
		}
		else
		{
			settings->Internals.BtnSettings->Position.X = settings->Size.Y;
			settings->Internals.BtnSettings->Position.Y = 2;
			settings->Internals.BtnSettings->Internals.PositionOffset.X = settings->Internals.Position.X - ParentWindow->Internals.Position.X;
			settings->Internals.BtnSettings->Internals.PositionOffset.Y = settings->Internals.Position.Y - ParentWindow->Internals.Position.Y;
		}

		paint_scrollbar(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
		LcdStruct->sClipRegion = back_up_clip;
		//control_comand->Cursor = _cursor;
		settings->Internals.ParentWindowStateEnabled = ParentWindow->Internals.OldStateEnabled;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.OldStateEnabled = settings->Enabled;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		control_comand->WindowRefresh |= true;
	}
	if(settings->Internals.OldStateCursor != control_comand->Cursor &&
					settings->Enabled == true &&
						settings->Visible == true)
	{
		settings->Internals.OldStateCursor = control_comand->Cursor;
	}
	tRectangle back_up_clip = LcdStruct->sClipRegion;
	LcdStruct->sClipRegion.sXMin = X_StartBox;
	LcdStruct->sClipRegion.sYMin = Y_StartBox;
	LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
	LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
	if(control_comand->Cursor)paint_scrollbar(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand);
	LcdStruct->sClipRegion = back_up_clip;
	bool inside_window = check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;
	if(inside_window == true && control_comand->Cursor == Cursor_Down) settings->Internals.CursorDownInsideBox = true;
	if(settings->Internals.CursorDownInsideBox == true && (control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction)) settings->Internals.CursorDownInsideBox = false;
	//control_comand->WindowRefresh |= true;
	//control_comand->CursorCoordonateUsed |= settings->Internals.CursorDownInsideBox;
}
//#######################################################################################
tScrollBar *new_scrollbar(void *ParentWindow)
{
	tScrollBar* settings = (tScrollBar*)calloc(1, sizeof(tScrollBar));

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
