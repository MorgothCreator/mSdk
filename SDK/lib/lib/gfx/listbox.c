/*
 *  lib/gfx/window.c
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
#include <stdlib.h>
#include "listbox.h"
#include "listbox_def.h"
#include "window_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "scrollbar.h"
#include "scrollbar_def.h"
#include "controls_definition.h"
#include "sys/plat_properties.h"
#include "lib/string_lib.h"
#include "gfx_gui_paint.h"
//#######################################################################################
static bool paint_listbox_item(tListBox *_settings, listbox_item* settings, void *pDisplay, signed int x_start, signed int y_start, tControlCommandData* control_comand, bool Pushed, bool Paint)
{
	if(!settings)
		return false;
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	tWindow *ParentWindow = (tWindow*)_settings->Internals.ParentWindow;
	bool inside_window = check_if_inside_box(x_start, y_start, settings->Size.X, settings->Size.Y, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;
	//if(!inside_window && !EntireRefresh && !settings->CursorDownInsideBox) return false;
	//if(inside_window == true/* && control_comand->Cursor == Cursor_Down*/) settings->CursorDownInsideBox = true;
	if(Paint)
	{
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = x_start;
		LcdStruct->sClipRegion.sYMin = y_start;
		LcdStruct->sClipRegion.sXMax = x_start + settings->Size.X;
		LcdStruct->sClipRegion.sYMax = y_start + settings->Size.Y;
		clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
		if((!_settings->Enabled || !ParentWindow->Internals.OldStateEnabled) && _settings->Internals.Control.Initiated == true)
			gui_put_item(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y, settings->Color.Disabled.Buton, settings->Color.Disabled.Border, Cursor_NoAction,PAINT_STYLE_ROUNDED_CORNERS , false);
		else {
			if(Pushed == true)
				gui_put_item(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y, controls_color.Control_Color_Enabled_Buton_Push, controls_color.Control_Color_Enabled_Border_Push, Cursor_Down,PAINT_STYLE_ROUNDED_CORNERS , true);
			else
				gui_put_item(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y, controls_color.Control_Color_Enabled_Buton_Pull, controls_color.Control_Color_Enabled_Border_Pull, Cursor_Up,PAINT_STYLE_ROUNDED_CORNERS , true);
		}
		if(settings->Caption.Text->text || settings->Caption.Text->len)
		{
			LcdStruct->sClipRegion.sXMin = x_start + 4;
			LcdStruct->sClipRegion.sYMin = y_start + 4;
			LcdStruct->sClipRegion.sXMax = ((x_start + settings->Size.X) - 4);
			LcdStruct->sClipRegion.sYMax = ((y_start + settings->Size.Y) - 4);
			clip_limit(&LcdStruct->sClipRegion, &back_up_clip);

			signed int x_str_location = x_start + 4;
			signed int y_str_location = y_start + 4;
			if(settings->Caption.TextAlign == Align_Center)
			{
				StringProperties_t str_properties = string_properties_get(pDisplay, settings->Caption.Font, settings->Caption.Text->text, settings->Caption.WordWrap, -1);
				x_str_location = x_start + ((settings->Size.X>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
				y_str_location = y_start + ((settings->Size.Y>>1)-(str_properties.StringColsHeight_Pixels>>1));
			}

			print_string_properties properties;
			memset(&properties, 0, sizeof(print_string_properties));
			properties.pDisplay = pDisplay;
			properties.pFont = settings->Caption.Font;
			properties.pcString = String.Clone(properties.pcString, settings->Caption.Text);
			properties.lLength = -1;
			//properties.foreground_color = settings->Color.Enabled.Ink.Push;
			//properties.background_color = settings->Color.Enabled.Buton.Push;
			properties.ulOpaque = false;
			properties.ulVisible = true;
			properties.WordWrap = settings->Caption.WordWrap;
			properties.lX = x_str_location;
			properties.lY = y_str_location;
			properties._SelStart = 0;
			properties._SelLen = 0;
			if(Pushed == true) {
				properties.foreground_color = settings->Color.Enabled.Ink.Push;
				properties.background_color = settings->Color.Enabled.Buton.Push;
			}
			else {
				properties.foreground_color = settings->Color.Enabled.Ink.Pull;
				properties.background_color = settings->Color.Enabled.Buton.Pull;
			}
			put_string(&properties);
			str_free(properties.pcString);
		}
		LcdStruct->sClipRegion.sXMin = x_start;
		LcdStruct->sClipRegion.sYMin = y_start;
		LcdStruct->sClipRegion.sXMax = x_start + settings->Size.X;
		LcdStruct->sClipRegion.sYMax = y_start + settings->Size.Y;
		clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
		LcdStruct->lcd_func.box_cache_clean(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y);
		LcdStruct->sClipRegion = back_up_clip;
		control_comand->WindowRefresh |= true;
	}
	return inside_window;
}
//#######################################################################################
void listbox(tListBox *settings, tControlCommandData* control_comand)
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
		settings->Internals.ScrollBar = new_scrollbar(settings->Internals.ParentWindow);
		tScrollBar *ScrollBar = settings->Internals.ScrollBar;


		ScrollBar->Position.X = settings->Internals.Size.X - 2 - settings->Size.ScrollSize;
		ScrollBar->Position.Y = 2;
		ScrollBar->Internals.PositionOffset.X = settings->Internals.Position.X - ParentWindow->Internals.Position.X;
		ScrollBar->Internals.PositionOffset.Y = settings->Internals.Position.Y - ParentWindow->Internals.Position.Y;

		//ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		//ScrollBar->Position.Y = settings->Internals.Position.Y + 2;
		ScrollBar->Size.X = settings->Size.ScrollSize;
		ScrollBar->Size.Y = settings->Internals.Size.Y - 4;
		ScrollBar->Maximum = settings->ItemsCount - ((settings->Size.Y - 4) / settings->Size.ItemSizeY);
		ScrollBar->Internals.NoPaintBackGround = true;

	}
	/* Verify if position on size has been modified */
	if(ParentWindow)
	{
		if((settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X) != settings->Internals.Position.X ||
				(settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y) != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.Size.ItemSizeY != settings->Size.ItemSizeY ||
								settings->Internals.Size.ScrollSize != settings->Size.ScrollSize ||
									settings->Internals.Caption.Font != settings->Caption.Font ||
									settings->Caption.Text->modifyed == true ||
											settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
												settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
													settings->Internals.OldStateEnabled != settings->Enabled ||
														settings->Internals.Size.MinScrollBtnSize != settings->Size.MinScrollBtnSize ||
															ParentWindow->Internals.OldStateEnabled != settings->Internals.ParentWindowStateEnabled)
		{
			settings->Internals.NeedEntireRefresh = true;
			settings->Caption.Text->modifyed = false;
		}
	}
	else
	{
		if(settings->Position.X != settings->Internals.Position.X ||
				settings->Position.Y != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.Size.ItemSizeY != settings->Size.ItemSizeY ||
								settings->Internals.Size.ScrollSize != settings->Size.ScrollSize ||
									settings->Internals.Caption.Font != settings->Caption.Font ||
									settings->Caption.Text->modifyed == true ||
											settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
												settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
													settings->Internals.Size.MinScrollBtnSize != settings->Size.MinScrollBtnSize ||
														settings->Internals.OldStateEnabled != settings->Enabled)
		{
			settings->Internals.NeedEntireRefresh = true;
			settings->Caption.Text->modifyed = false;
		}
	}
	//if(settings->Internals.Caption.Text != NULL && settings->Caption.Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Caption.Text) == NULL)
		//settings->Internals.NeedEntireRefresh = true;

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
		settings->Internals.Size.ItemSizeY = settings->Size.ItemSizeY;
		settings->Internals.Size.ScrollSize = settings->Size.ScrollSize;
		settings->Internals.Caption.Font = settings->Caption.Font;
		//settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;

		//settings->Internals.ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		//settings->Internals.ScrollBar->Position.Y = settings->Internals.Position.Y + 2;
		settings->Internals.ScrollBar->Internals.PositionOffset.X = settings->Internals.Position.X - ParentWindow->Internals.Position.X;
		settings->Internals.ScrollBar->Internals.PositionOffset.Y = settings->Internals.Position.Y - ParentWindow->Internals.Position.Y;
		settings->Internals.ScrollBar->Size.X = settings->Size.ScrollSize;
		settings->Internals.ScrollBar->Size.Y = settings->Internals.Size.Y - 4;
		settings->Internals.ScrollBar->Maximum = settings->ItemsCount - ((settings->Size.Y - 4) / settings->Size.ItemSizeY);
		settings->Internals.ScrollBar->Enabled = settings->Enabled;
		settings->Internals.ScrollBar->Size.MinBtnSize = settings->Size.MinScrollBtnSize;
		settings->Internals.Size.MinScrollBtnSize = settings->Size.MinScrollBtnSize;
		if(settings->Size.X == 0 || settings->Size.Y == 0) return;

		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		unsigned int CntDisplayItems;
		for(CntDisplayItems = 0; CntDisplayItems < settings->ItemsCount ; CntDisplayItems++)
		{
			settings->Items[CntDisplayItems]->Size.X = settings->AlwaisShowScrollbar? settings->Size.X - 4 : settings->Size.X - settings->Internals.Size.ScrollSize - 5;
			settings->Items[CntDisplayItems]->Size.Y = settings->Internals.Size.ItemSizeY;
		}

		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
		unsigned int color = controls_color.Control_Color_Enabled_Border_Pull;
		if((!settings->Enabled || !ParentWindow->Internals.OldStateEnabled) && settings->Internals.Control.Initiated == true) {
			color = settings->Color.Disabled.Border;
			LcdStruct->lcd_func.put_horizontal_line(pDisplay, X_StartBox, X_LenBox, Y_StartBox, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
			LcdStruct->lcd_func.put_horizontal_line(pDisplay, X_StartBox, X_LenBox, (Y_StartBox + Y_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->lcd_func.put_vertical_line(pDisplay, Y_StartBox, Y_LenBox, X_StartBox, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
			LcdStruct->lcd_func.put_vertical_line(pDisplay, Y_StartBox, Y_LenBox, (X_StartBox + X_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		} else {
		//put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, false, controlls_change_color(color, -3));
			LcdStruct->lcd_func.put_horizontal_line(pDisplay, X_StartBox, X_LenBox, Y_StartBox, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
			LcdStruct->lcd_func.put_horizontal_line(pDisplay, X_StartBox, X_LenBox, (Y_StartBox + Y_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));

			LcdStruct->lcd_func.put_vertical_line(pDisplay, Y_StartBox, Y_LenBox, X_StartBox, 1, controlls_change_color(color, +BORDER_LINE_ONE_LIGHT));
			LcdStruct->lcd_func.put_vertical_line(pDisplay, Y_StartBox, Y_LenBox, (X_StartBox + X_LenBox) - 1, 1, controlls_change_color(color, -BORDER_LINE_ONE_DARK));
		}


		if(settings->Enabled)
			LcdStruct->lcd_func.put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, Y_LenBox - 2, true, settings->Color.Enabled.BackGround);
		else
			LcdStruct->lcd_func.put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, Y_LenBox - 2, true, settings->Color.Disabled.BackGround);

		LcdStruct->lcd_func.box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);


		CursorState cursor = control_comand->Cursor;
		//if(!settings->Internals.IsChildren) control_comand->Cursor = Cursor_Up;
		for(CntDisplayItems = settings->Internals.ItemStartOnBox; CntDisplayItems < ((settings->Size.Y - 4) / settings->Size.ItemSizeY) + settings->Internals.ItemStartOnBox; CntDisplayItems++)
		{
			if(CntDisplayItems >= settings->ItemsCount) break;
			bool Pushed = false;
			if(settings->SelectedItem == CntDisplayItems) Pushed = true;
			else if(settings->Internals.IsChildren == false || settings->SelectedItem != CntDisplayItems) Pushed = false;
			paint_listbox_item(settings, settings->Items[CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((CntDisplayItems - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, Pushed, true);
		}
		control_comand->Cursor = cursor;
		if(settings->Enabled)
			settings->Internals.ScrollBar->Color.Scren = settings->Color.Enabled.BackGround;
		else
			settings->Internals.ScrollBar->Color.Scren = settings->Color.Disabled.BackGround;
		settings->Internals.ScrollBar->Color.Enabled.BackGround = settings->Color.Enabled.BackGround;;
		settings->Internals.ScrollBar->Color.Disabled.BackGround = settings->Color.Disabled.BackGround;;

		settings->Internals.ScrollBar->Internals.NeedEntireRefresh = true;
		scrollbar(settings->Internals.ScrollBar, control_comand);

		LcdStruct->sClipRegion = back_up_clip;
		settings->Internals.ParentWindowStateEnabled = ParentWindow->Internals.OldStateEnabled;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.OldStateEnabled = settings->Enabled;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		control_comand->WindowRefresh |= true;
		return;
	}

	scrollbar(settings->Internals.ScrollBar, control_comand);
	if(settings->Internals.ScrollBar->Events.ValueChanged)
	{
		settings->Internals.ScrollBar->Events.ValueChanged = false;
		settings->Internals.ItemStartOnBox = settings->Internals.ScrollBar->Value;
	}
	/* Check if inside window */
	bool inside_window = check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMin, LcdStruct->sClipRegion.sXMax - LcdStruct->sClipRegion.sXMin, LcdStruct->sClipRegion.sYMax - LcdStruct->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;
	if(control_comand->Cursor == Cursor_Down && inside_window == true) settings->Internals.CursorDownInsideBox = true;

	if(((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			settings->Internals.OldStateCursor != control_comand->Cursor &&
				settings->Internals.CursorDownInsideBox == true &&
					settings->Enabled == true &&
						settings->Visible == true &&
							control_comand->CursorCoordonateUsed == false) ||
								settings->Internals.ItemStartOnBox != settings->Internals.OldItemStartOnBox)
	{
		settings->Internals.OldStateCursor = control_comand->Cursor;

		unsigned int CntDisplayItems;
		bool items_position_has_changed = false;
		if(settings->Internals.ItemStartOnBox != settings->Internals.OldItemStartOnBox)
		{
			settings->Internals.OldItemStartOnBox = settings->Internals.ItemStartOnBox;
			items_position_has_changed = true;
		}
		tRectangle back_up_clip = LcdStruct->sClipRegion;
		LcdStruct->sClipRegion.sXMin = X_StartBox;
		LcdStruct->sClipRegion.sYMin = Y_StartBox;
		LcdStruct->sClipRegion.sXMax = X_StartBox + X_LenBox;
		LcdStruct->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&LcdStruct->sClipRegion, &back_up_clip);
		unsigned int EndDisplayedItems = ((settings->Size.Y - 4) / settings->Size.ItemSizeY) + settings->Internals.ItemStartOnBox;
		for(CntDisplayItems = settings->Internals.ItemStartOnBox; CntDisplayItems < EndDisplayedItems; CntDisplayItems++)
		{
			if(CntDisplayItems >= settings->ItemsCount) break;
			if(items_position_has_changed)
			{
				bool Pushed = false;
				if(settings->SelectedItem == CntDisplayItems) Pushed = true;
				else if(settings->Internals.IsChildren == false || settings->SelectedItem != CntDisplayItems) Pushed = false;
				paint_listbox_item(settings, settings->Items[CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((CntDisplayItems - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, Pushed, true);
			}
			else
			{
				if( control_comand->Cursor == settings->StateChangedOn && paint_listbox_item(settings, settings->Items[CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((CntDisplayItems - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, false, false))
				{
					unsigned int _CntDisplayItems = CntDisplayItems;
					if(settings->SelectedItem < EndDisplayedItems && settings->SelectedItem >= settings->Internals.ItemStartOnBox)
					{
						paint_listbox_item(settings, settings->Items[settings->SelectedItem], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((settings->SelectedItem - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, false, true);
					}
					settings->SelectedItem = _CntDisplayItems;
					paint_listbox_item(settings, settings->Items[_CntDisplayItems], pDisplay, X_StartBox + 2, Y_StartBox + 2 + ((_CntDisplayItems - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, true, true);
					break;
				}
			}
		}
		LcdStruct->sClipRegion = back_up_clip;
	}
	if(control_comand->Cursor == Cursor_Down && inside_window == true && settings->Internals.CursorDownInsideBox == true)settings->Events.CursorDown = true;
	if(settings->Events.OnDown.CallBack && control_comand->Cursor == Cursor_Down && inside_window == true && settings->Internals.CursorDownInsideBox == true)
	{
		settings->Events.OnDown.CallbackReturnData = settings->Events.OnDown.CallBack(settings->Events.OnDown.CallbackData);
	}
	if(control_comand->Cursor == Cursor_Up && inside_window == true && settings->Internals.CursorDownInsideBox == true)settings->Events.CursorUp = true;
	if(settings->Events.OnUp.CallBack && control_comand->Cursor == Cursor_Up && inside_window == true && settings->Internals.CursorDownInsideBox == true)
	{
		settings->Events.OnUp.CallbackReturnData = settings->Events.OnUp.CallBack(settings->Events.OnUp.CallbackData);
	}
	if(control_comand->Cursor == Cursor_Move && inside_window == true)settings->Events.CursorMove = true;
	if(settings->Events.OnMove.CallBack && control_comand->Cursor == Cursor_Move && inside_window == true && settings->Internals.CursorDownInsideBox == true)
	{
		settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings->Events.OnMove.CallbackData);
	}

	if(control_comand->Cursor && settings->Internals.CursorDownInsideBox) control_comand->CursorCoordonateUsed |= true;
	if(control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction) settings->Internals.CursorDownInsideBox = false;
	//control_comand->CursorCoordonateUsed = settings->Internals.CursorDownInsideBox;
	//control_comand->WindowRefresh |= true;
	return;
}
//#######################################################################################
void* listbox_item_insert(void* _settings, char* text, unsigned int location)
{
	if(!_settings) return NULL;
	tListBox* settings = _settings;
	listbox_item* item_settings = (listbox_item*)calloc(1, sizeof(listbox_item));
	if(!item_settings)
		return false;

	memcpy((void*)&item_settings->Color, (void*)&settings->Color, sizeof(item_settings->Color));

	memcpy((void*)&item_settings->Caption, (void*)&settings->Caption, sizeof(item_settings->Caption));
	//item_settings->Caption.Text->initialized = false;
	item_settings->Size.X = settings->AlwaisShowScrollbar? settings->Size.X - 4 : settings->Size.X - settings->Internals.Size.ScrollSize - 5;
	item_settings->Size.Y = settings->Internals.Size.ItemSizeY;

	item_settings->Caption.Text = str_set(NULL, text);
	str_remove_new_line(item_settings->Caption.Text);

	//item_settings->Caption.Font = settings->Internals.Caption.Font;
	//item_settings->Caption.TextAlign = settings->Internals.Caption.TextAlign;
	//item_settings->Caption.WordWrap = settings->Internals.Caption.WordWrap;

	//item_settings->Size.X = settings->Size.X;
	//item_settings->Size.Y = settings->Size.ItemSizeY;

	listbox_item **Tmp = (listbox_item **)realloc(settings->Items, sizeof(void*) * (settings->ItemsCount + 1));
	if(!Tmp)
	{
		//free(item_settings->Caption.Text.text);
		free(item_settings);
		return NULL;
	}
	unsigned int TmpCntItemsToMove = settings->ItemsCount;
	for(; TmpCntItemsToMove > location; TmpCntItemsToMove --)
	{
		Tmp[TmpCntItemsToMove] = Tmp[TmpCntItemsToMove - 1];
	}
	Tmp[location] = item_settings;
	settings->Items = Tmp;
	settings->ItemsCount++;
	settings->Internals.NeedEntireRefresh = true;
	return _settings;
}
//#######################################################################################
void* listbox_item_add(void* _settings, char* text)
{
	tListBox* settings = _settings;
	return listbox_item_insert(_settings, text, settings->ItemsCount);
}
//#######################################################################################
bool listbox_item_remove(void* _settings, unsigned int location)
{
	tListBox* settings = _settings;
	if(location >= settings->ItemsCount) return false;
	settings->Items[location]->Caption.Text = str_free(settings->Items[location]->Caption.Text);
	if(settings->Items[location])
		free(settings->Items[location]);
	if(settings->ItemsCount - 1 != 0)
	{
		unsigned int TmpCntItemsToMove = location +1;
		for(; TmpCntItemsToMove < settings->ItemsCount; TmpCntItemsToMove++)
		{
			settings->Items[TmpCntItemsToMove - 1] = settings->Items[TmpCntItemsToMove];
		}
		listbox_item **Temp = (listbox_item **)realloc(settings->Items, sizeof(void*) * (settings->ItemsCount - 1));
		if(!Temp) return false;
		settings->Items = Temp;
		settings->ItemsCount--;
		if(settings->SelectedItem > settings->ItemsCount) settings->SelectedItem = settings->ItemsCount - 1;
		if(location < settings->SelectedItem)
		{
			settings->SelectedItem--;
			settings->Internals.ScrollBar->Value = settings->SelectedItem;
		}
		if(settings->Internals.ItemStartOnBox) settings->Internals.ItemStartOnBox--;
		settings->Internals.NeedEntireRefresh = true;
	}
	else listbox_item_remove_all(_settings);
	return true;
}
//#######################################################################################
bool listbox_item_remove_all(void* _settings)
{
	tListBox* settings = _settings;
	unsigned int TmpCntItemsToMove = settings->ItemsCount;
	for(; TmpCntItemsToMove > 0; TmpCntItemsToMove--)
	{
		if(settings->Items[TmpCntItemsToMove - 1]->Caption.Text->text)
			free(settings->Items[TmpCntItemsToMove - 1]->Caption.Text->text);
		if(settings->Items[TmpCntItemsToMove - 1])
			free(settings->Items[TmpCntItemsToMove - 1]);
	}
	settings->SelectedItem = 0;
	settings->ItemsCount = 0;
	if(settings->Items) free(settings->Items);
	listbox_item ** TempPtr = (listbox_item **)calloc(1, sizeof(void*));
	if(TempPtr)
	{
		if(settings->Items) free(settings->Items);
		settings->Items = TempPtr;
	}
	else settings->Items[0] = NULL;
	settings->Internals.ItemStartOnBox = 0;
	settings->Internals.NeedEntireRefresh = true;
	return true;
}
//#######################################################################################
tListBox *new_listbox(void *ParentWindow)
{
	tListBox* settings = (tListBox*)calloc(1, sizeof(tListBox));

	if(!settings || !ParentWindow) return NULL;
	settings->Internals.ParentWindow = ParentWindow;

	tWindow *_ParentWindow = (tWindow *)ParentWindow;
	settings->Internals.pDisplay = _ParentWindow->Internals.pDisplay;

	settings->Caption.TextAlign = Align_Left;
	settings->Caption.WordWrap = true;
	settings->Caption.Font = controls_color.DefaultFont;

	settings->Caption.Text = str_set(settings->Caption.Text, "Item..");
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

	settings->Items = (listbox_item **)calloc(1, sizeof(void*));
	settings->Enabled = true;
	settings->Position.X = 50;
	settings->Position.Y = 75;
	settings->Size.X = 200;
	settings->Size.Y = 60;
	settings->Size.ItemSizeY = 20;
	settings->StateChangedOn = Cursor_Up;
	settings->Visible = true;
	settings->Size.ScrollSize = 20;
#ifdef USE_FULL_GRAPHICS
	settings->Item.add = listbox_item_add;
	settings->Item.insert = listbox_item_insert;
	settings->Item.remove = listbox_item_remove;
	settings->Item.remove_all = listbox_item_remove_all;
	settings->free = free_listbox;
#endif
	//settings->AlwaisShowScrollbar = false;
	return settings;
}
//#######################################################################################
bool free_listbox(void* _settings)
{
	tListBox* settings = _settings;
	if(!settings) return false;

	if(!free_scrollbar(settings->Internals.ScrollBar)) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	listbox(settings, &comand);

	unsigned int TmpCntItemsToMove = 0;
	for(; TmpCntItemsToMove > settings->ItemsCount; TmpCntItemsToMove++)
	{
		char *Tmp = settings->Items[TmpCntItemsToMove]->Caption.Text->text;
		if(Tmp)
			free(Tmp);
		if(settings->Items[TmpCntItemsToMove])
			free(settings->Items[TmpCntItemsToMove]);
	}

	if(settings->Internals.Caption.Text->text)
		free(settings->Internals.Caption.Text->text);
	if(settings->Caption.Text->text)
		free(settings->Caption.Text->text);
	if(settings) free(settings);
	return true;
}
//#######################################################################################



