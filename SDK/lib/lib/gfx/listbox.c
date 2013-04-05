/*
 *  lib/gfx/window.c
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
#include "listbox.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "scrollbar.h"
#include "controls_definition.h"
#include "board_properties.h"
//#######################################################################################
static bool paint_listbox_item(listbox_item* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, tControlCommandData* control_comand, bool EntireRefresh)
{
	if(!settings) return false;
	bool inside_window = check_if_inside_box(x_start, y_start, settings->Size.X, settings->Size.Y, control_comand->X, control_comand->Y);
	bool _inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;
	if(!inside_window && !EntireRefresh && !settings->CursorDownInsideBox) return false;
	if(inside_window == true && control_comand->Cursor == Cursor_Down) settings->CursorDownInsideBox = true;
	unsigned int color = 0;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + settings->Size.X;
	pDisplay->sClipRegion.sYMax = y_start + settings->Size.Y;
	//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
	//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
	//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
	//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	if(control_comand->Cursor == Cursor_Down) color = controls_color.Control_Color_Enabled_Border_Push;
	else if(control_comand->Cursor == Cursor_Move) color = controls_color.Control_Color_Enabled_Border_Push;
	else if(control_comand->Cursor == Cursor_Up) color = controls_color.Control_Color_Enabled_Border_Pull;
	else color = controls_color.Control_Color_Enabled_Border_Pull;
	put_rectangle(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y, false, controlls_change_color(color, -3));
	put_rectangle(pDisplay, x_start + 1, y_start + 1, settings->Size.X - 2, settings->Size.Y - 2, false, controlls_change_color(color, -2));
	if(control_comand->Cursor == Cursor_Down) color = controls_color.Control_Color_Enabled_Buton_Push;
	else if(control_comand->Cursor == Cursor_Move) color = controls_color.Control_Color_Enabled_Buton_Push;
	else if(control_comand->Cursor == Cursor_Up) color = controls_color.Control_Color_Enabled_Buton_Pull;
	else color = controls_color.Control_Color_Enabled_Buton_Pull;
	put_rectangle(pDisplay, x_start + 2, y_start + 2, settings->Size.X - 4, settings->Size.Y - 4, true, color);
	if(settings->Caption.Text)
	{
		pDisplay->sClipRegion.sXMin = x_start + 4;
		pDisplay->sClipRegion.sYMin = y_start + 4;
		pDisplay->sClipRegion.sXMax = ((x_start + settings->Size.X) - 4);
		pDisplay->sClipRegion.sYMax = ((y_start + settings->Size.Y) - 4);
		//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
		//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
		//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
		//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);

		signed int x_str_location = x_start + 4;
		signed int y_str_location = y_start + 4;
		if(settings->Caption.TextAlign == Align_Center)
		{
			StringProperties_t str_properties = string_properties_get(pDisplay, settings->Caption.Font, settings->Caption.Text, settings->Caption.WordWrap, -1);
			x_str_location = x_start + ((settings->Size.X>>1)-(str_properties.StringRowsMaxLength_Pixels>>1));
			y_str_location = y_start + ((settings->Size.Y>>1)-(str_properties.StringColsHeight_Pixels>>1));
		}

		if(control_comand->Cursor == Cursor_Down) put_string(pDisplay, settings->Caption.Font, settings->Caption.Text, -1, settings->Color.Enabled.Ink.Push, settings->Color.Enabled.Buton.Push, false, true, settings->Caption.WordWrap, x_str_location, y_str_location, 0, 0);
		else if(control_comand->Cursor == Cursor_Move) put_string(pDisplay, settings->Caption.Font, settings->Caption.Text, -1, settings->Color.Enabled.Ink.Push, settings->Color.Enabled.Buton.Push, false, true, settings->Caption.WordWrap, x_str_location, y_str_location, 0, 0);
		else if(control_comand->Cursor == Cursor_Up) put_string(pDisplay, settings->Caption.Font, settings->Caption.Text, -1, settings->Color.Enabled.Ink.Pull, settings->Color.Enabled.Buton.Pull, false, true, settings->Caption.WordWrap, x_str_location, y_str_location, 0, 0);
		else put_string(pDisplay, settings->Caption.Font, settings->Caption.Text, -1, settings->Color.Enabled.Ink.Pull, settings->Color.Enabled.Buton.Pull, false, true, settings->Caption.WordWrap, x_str_location, y_str_location, 0, 0);
	}
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + settings->Size.X;
	pDisplay->sClipRegion.sYMax = y_start + settings->Size.Y;
	//if(pDisplay->sClipRegion.sXMin < back_up_clip.sXMin) pDisplay->sClipRegion.sXMin = back_up_clip.sXMin;
	//if(pDisplay->sClipRegion.sYMin < back_up_clip.sYMin) pDisplay->sClipRegion.sYMin = back_up_clip.sYMin;
	//if(pDisplay->sClipRegion.sXMax >= back_up_clip.sXMax) pDisplay->sClipRegion.sXMax = back_up_clip.sXMax;
	//if(pDisplay->sClipRegion.sYMax >= back_up_clip.sYMax) pDisplay->sClipRegion.sYMax = back_up_clip.sYMax;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	box_cache_clean(pDisplay, x_start, y_start, settings->Size.X, settings->Size.Y);
	pDisplay->sClipRegion = back_up_clip;
	//if(control_comand->Cursor == Cursor_Down) ;
	//if(control_comand->Cursor == Cursor_Move) ;
	if(settings->CursorDownInsideBox == true && control_comand->Cursor == Cursor_Up)
	{
		settings->CursorDownInsideBox = false;
		return true;
	}
	else return false;
}
//#######################################################################################
void listbox(tListBox *settings, tControlCommandData* control_comand)
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
		settings->Internals.ScrollBar = new_scrollbar(settings->Internals.pDisplay);
		tScrollBar *ScrollBar = settings->Internals.ScrollBar;
		ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		ScrollBar->Position.Y = settings->Internals.Position.Y + 2;
		ScrollBar->Size.X = settings->Size.ScrollSize;
		ScrollBar->Size.Y = settings->Internals.Size.Y - 4;
		ScrollBar->Maximum = settings->ItemsCount - ((settings->Size.Y - 4) / settings->Size.ItemSizeY);
		ScrollBar->Internals.NoPaintBackGround = true;
	}
	/* Verify if position on size has been modified */
	if(settings->Position.X != settings->Internals.Position.X ||
			settings->Position.Y != settings->Internals.Position.Y ||
				settings->Size.X != settings->Internals.Size.X ||
					settings->Size.Y != settings->Internals.Size.Y ||
						settings->Internals.Size.ItemSizeY != settings->Size.ItemSizeY ||
							settings->Internals.Size.ScrollSize != settings->Size.ScrollSize ||
								settings->Internals.Caption.Font != settings->Caption.Font ||
									settings->Internals.Caption.Text != settings->Caption.Text ||
										settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
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
		settings->Internals.Size.ItemSizeY = settings->Size.ItemSizeY;
		settings->Internals.Size.ScrollSize = settings->Size.ScrollSize;
		settings->Internals.Caption.Font = settings->Caption.Font;
		settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;

		settings->Internals.ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		settings->Internals.ScrollBar->Position.Y = settings->Internals.Position.Y + 2;
		settings->Internals.ScrollBar->Size.X = settings->Size.ScrollSize;
		settings->Internals.ScrollBar->Size.Y = settings->Internals.Size.Y - 4;
		settings->Internals.ScrollBar->Maximum = settings->ItemsCount - ((settings->Size.Y - 4) / settings->Size.ItemSizeY);

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
		unsigned int color = controls_color.Control_Color_Enabled_Border_Pull;
		put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, false, controlls_change_color(color, -3));
		put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, Y_LenBox - 2, true, controlls_change_color(color, -2));
		box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);


		CursorState cursor = control_comand->Cursor;
		if(!settings->Internals.IsChildren) control_comand->Cursor = Cursor_Up;
		for(CntDisplayItems = settings->Internals.ItemStartOnBox; CntDisplayItems < ((settings->Size.Y - 4) / settings->Size.ItemSizeY) + settings->Internals.ItemStartOnBox; CntDisplayItems++)
		{
			if(CntDisplayItems >= settings->ItemsCount) break;
			paint_listbox_item(settings->Items[CntDisplayItems], pDisplay, X_StartBox + 2, settings->Position.Y + 2 + ((CntDisplayItems - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, true);
		}
		control_comand->Cursor = cursor;

		//settings->Internals.ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - 2 - settings->Size.ScrollSize;
		//settings->Internals.ScrollBar->Position.Y = settings->Internals.Position.Y + 2;
		//settings->Internals.ScrollBar->Size.X = settings->Size.ScrollSize;
		//settings->Internals.ScrollBar->Size.Y = settings->Internals.Size.Y - 4;
		//settings->Internals.ScrollBar->Maximum = settings->ItemsCount - ((settings->Size.Y - 4) / settings->Size.ItemSizeY);

		settings->Internals.ScrollBar->Internals.NeedEntireRefresh = true;
		scrollbar(settings->Internals.ScrollBar, control_comand);

		pDisplay->sClipRegion = back_up_clip;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.OldStateEnabled = settings->Enabled;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
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
	bool _inside_window = check_if_inside_box(pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMax - pDisplay->sClipRegion.sYMin, control_comand->X, control_comand->Y);
	if(!_inside_window) inside_window = false;

	if(((control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_Down) &&
			settings->Internals.OldStateCursor != control_comand->Cursor &&
				(inside_window == true || settings->Internals.CursorDownInsideBox == true) &&
					settings->Enabled == true &&
						settings->Visible == true) || settings->Internals.ItemStartOnBox != settings->Internals.OldItemStartOnBox)
	{
		if(control_comand->Cursor == Cursor_Down && inside_window == true) settings->Internals.CursorDownInsideBox = true;
		settings->Internals.OldStateCursor = control_comand->Cursor;

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
		unsigned int CntDisplayItems;
		bool items_position_has_changed = false;
		if(settings->Internals.ItemStartOnBox != settings->Internals.OldItemStartOnBox)
		{
			settings->Internals.OldItemStartOnBox = settings->Internals.ItemStartOnBox;
			items_position_has_changed = true;
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
		for(CntDisplayItems = settings->Internals.ItemStartOnBox; CntDisplayItems < ((settings->Size.Y - 4) / settings->Size.ItemSizeY) + settings->Internals.ItemStartOnBox; CntDisplayItems++)
		{
			if(CntDisplayItems >= settings->ItemsCount) break;
			if(items_position_has_changed)
			{
				CursorState cursor = control_comand->Cursor;
				if(!settings->Internals.IsChildren) control_comand->Cursor = Cursor_Up;
				paint_listbox_item(settings->Items[CntDisplayItems], pDisplay, X_StartBox + 2, settings->Position.Y + 2 + ((CntDisplayItems - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, true);
				control_comand->Cursor = cursor;
			}
			else
			{
				if(paint_listbox_item(settings->Items[CntDisplayItems], pDisplay, X_StartBox + 2, settings->Position.Y + 2 + ((CntDisplayItems - settings->Internals.ItemStartOnBox) * settings->Size.ItemSizeY), control_comand, false)) settings->SelectedItem = CntDisplayItems;
			}
		}
		pDisplay->sClipRegion = back_up_clip;
	}
	if(control_comand->Cursor == Cursor_Move && inside_window == true)settings->Events.CursorMove = true;
	if(settings->Events.OnMove.CallBack && control_comand->Cursor == Cursor_Move && inside_window == true && settings->Internals.CursorDownInsideBox == true)
	{
		settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings->Events.OnMove.CallbackData);
	}

	if(settings->Internals.CursorDownInsideBox == true && control_comand->Cursor == Cursor_Up) settings->Internals.CursorDownInsideBox = false;
	control_comand->CursorCoordonateUsed = settings->Internals.CursorDownInsideBox;
	return;
}
//#######################################################################################
bool listbox_item_add(void* _settings, char* text)
{
	if(!_settings) return false;
	tListBox* settings = _settings;
	listbox_item* item_settings = (listbox_item*)calloc(1, sizeof(listbox_item));
	if(!item_settings) return false;

	memcpy((void*)&item_settings->Color, (void*)&settings->Color, sizeof(item_settings->Color));

	memcpy((void*)&item_settings->Caption, (void*)&settings->Caption, sizeof(item_settings->Caption));

	item_settings->Size.X = settings->AlwaisShowScrollbar? settings->Size.X - 4 : settings->Size.X - settings->Internals.Size.ScrollSize - 5;
	item_settings->Size.Y = settings->Internals.Size.ItemSizeY;

	item_settings->Caption.Text = malloc(strlen(text) + 1);
	if(!item_settings->Caption.Text)
	{
		free(item_settings);
		return false;
	}
	strcpy(item_settings->Caption.Text, text);

	//item_settings->Caption.Font = settings->Internals.Caption.Font;
	//item_settings->Caption.TextAlign = settings->Internals.Caption.TextAlign;
	//item_settings->Caption.WordWrap = settings->Internals.Caption.WordWrap;

	//item_settings->Size.X = settings->Size.X;
	//item_settings->Size.Y = settings->Size.ItemSizeY;

	listbox_item **Tmp = (listbox_item **)realloc(settings->Items, sizeof(void*) * (settings->ItemsCount + 1));
	if(!Tmp)
	{
		free(item_settings->Caption.Text);
		free(item_settings);
		return false;
	}
	Tmp[settings->ItemsCount] = item_settings;
	settings->Items = Tmp;
	settings->ItemsCount++;
	return true;
}
//#######################################################################################
tListBox *new_listbox(tDisplay *ScreenDisplay)
{
	tListBox* settings = (tListBox*)calloc(1, sizeof(tListBox));

	if(!settings || !ScreenDisplay) return NULL;

	settings->Internals.pDisplay = ScreenDisplay;
	settings->Caption.TextAlign = Align_Left;
	settings->Caption.WordWrap = true;
	settings->Caption.Font = controls_color.DefaultFont;
	settings->Caption.Text = "Item..";
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
	//settings->AlwaisShowScrollbar = false;
	return settings;
}
//#######################################################################################
bool free_listbox(tListBox *settings)
{
	if(!settings) return false;

	if(!free_scrollbar(settings->Internals.ScrollBar)) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	listbox(settings, &comand);
	if(settings->Internals.Caption.Text) free(settings->Internals.Caption.Text);
	if(settings->Caption.Text) free(settings->Caption.Text);
	if(settings) free(settings);
	return true;
}
//#######################################################################################



