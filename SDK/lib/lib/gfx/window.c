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

#include "window.h"
#include "graphic_string.h"
#include "controls_definition.h"
#include "board_properties.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "buton.h"
#include "checkbox.h"
#include "listbox.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "textbox.h"
//#######################################################################################
void window(struct Window_s *settings, tControlCommandData *control_comand);
void* window_add_children(struct Window_s *settings, unsigned int children_type, char *children_name);
signed int window_get_children_index(struct Window_s *settings, char *name);
window_children_t *window_get_children_address(struct Window_s *settings, char *name);
bool window_set_list_of_childrens(struct Window_s *settings, window_children_t **list, unsigned int num_of_childrens);
//#######################################################################################
static void window_set_children_settings(struct Window_s *settings, bool call_childrens, bool transfer_settings, tControlCommandData* control_comand, bool refresh_childrens, ChildrenWindowSize_t *ChildrenWindowSize)
{
	unsigned int Tmp_Children_Cnt = 0;
	tRectangle sClipRegion;
	if(call_childrens == true && settings->Internals.ChildrensNr != 0)
	{
		sClipRegion = settings->Internals.pDisplay->sClipRegion;
		settings->Internals.pDisplay->sClipRegion.sXMin = settings->Position.X + 2;
		settings->Internals.pDisplay->sClipRegion.sXMax = (settings->Position.X + settings->Size.X) - 3 - settings->Internals.V_ScrollBar->Size.X;//settings->Internals.Size.ScrollBarSize;
		settings->Internals.pDisplay->sClipRegion.sYMin = settings->Position.Y + 1 + settings->Internals.Header.Size.Y;
		settings->Internals.pDisplay->sClipRegion.sYMax = (settings->Position.Y + settings->Size.Y) - 3 - settings->Internals.H_ScrollBar->Size.Y;//settings->Internals.Size.ScrollBarSize;
		clip_limit(&settings->Internals.pDisplay->sClipRegion, &sClipRegion);
	}
	//CursorState back = control_comand->Cursor;
	//if(settings->Internals.NeedEntireRefresh) control_comand->Cursor = Cursor_Up;

	ChildrenWindowSize->X = 0;
	ChildrenWindowSize->Y = 0;

	while(Tmp_Children_Cnt < settings->Internals.ChildrensNr && settings->Internals.ChildrensNr != 0)
	{
		if(ChildrenWindowSize->X < (settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X))
		{
			ChildrenWindowSize->X = settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X;
		}
		if(ChildrenWindowSize->Y < (settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y))
		{
			ChildrenWindowSize->Y = settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y;
		}
		if(settings->Internals.Childrens[Tmp_Children_Cnt])
		{
			unsigned int children_type = settings->Internals.Childrens[Tmp_Children_Cnt]->Type;
			void *children = settings->Internals.Childrens[Tmp_Children_Cnt]->Children;
			if(WindowButonChildren == children_type)
			{
				tButton *Buton_settings = (tButton *)children;
				if(refresh_childrens) Buton_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					Buton_settings->Color = settings->Internals.Childrens[Tmp_Children_Cnt]->Color;
					Buton_settings->Enabled = settings->Internals.Childrens[Tmp_Children_Cnt]->Enabled;
					Buton_settings->Visible = settings->Internals.Childrens[Tmp_Children_Cnt]->Visible;
					Buton_settings->StateChangedOn = settings->Internals.Childrens[Tmp_Children_Cnt]->StateChangedOn;
					Buton_settings->Caption = settings->Internals.Childrens[Tmp_Children_Cnt]->Caption;
					Buton_settings->Size.X = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X;
					Buton_settings->Size.Y = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y;
					Buton_settings->Position.X = settings->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + 3 + settings->Internals.Position.ChildrenPosition_X;
					Buton_settings->Position.Y = settings->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Header.Size.Y + 1 + settings->Internals.Position.ChildrenPosition_Y;
					Buton_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens) button(Buton_settings, control_comand);
			}
			else if(WindowCheckboxChildren == children_type)
			{
				tCheckBox *CheckBox_settings = (tCheckBox *)children;
				if(refresh_childrens) CheckBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					CheckBox_settings->Color = settings->Internals.Childrens[Tmp_Children_Cnt]->Color;
					CheckBox_settings->Enabled = settings->Internals.Childrens[Tmp_Children_Cnt]->Enabled;
					CheckBox_settings->Visible = settings->Internals.Childrens[Tmp_Children_Cnt]->Visible;
					CheckBox_settings->StateChangedOn = settings->Internals.Childrens[Tmp_Children_Cnt]->StateChangedOn;
					CheckBox_settings->Caption = settings->Internals.Childrens[Tmp_Children_Cnt]->Caption;
					CheckBox_settings->Size.X = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X;
					CheckBox_settings->Size.Y = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y;
					CheckBox_settings->Position.X = settings->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + 3 + settings->Internals.Position.ChildrenPosition_X;
					CheckBox_settings->Position.Y = settings->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Header.Size.Y + 1 + settings->Internals.Position.ChildrenPosition_Y;
					CheckBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens) checkbox(CheckBox_settings, control_comand);
			}
			else if(WindowListboxChildren == children_type)
			{
				tListBox *ListBox_settings = (tListBox *)children;
				if(refresh_childrens) ListBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ListBox_settings->Color = settings->Internals.Childrens[Tmp_Children_Cnt]->Color;
					ListBox_settings->Enabled = settings->Internals.Childrens[Tmp_Children_Cnt]->Enabled;
					ListBox_settings->Visible = settings->Internals.Childrens[Tmp_Children_Cnt]->Visible;
					ListBox_settings->StateChangedOn = settings->Internals.Childrens[Tmp_Children_Cnt]->StateChangedOn;
					//ListBox_settings->Caption = settings->Internals.Childrens[Tmp_Children_Cnt]->Caption;
					ListBox_settings->Size.X = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X;
					ListBox_settings->Size.Y = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y;
					ListBox_settings->Position.X = settings->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + 3 + settings->Internals.Position.ChildrenPosition_X;
					ListBox_settings->Position.Y = settings->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Header.Size.Y + 1 + settings->Internals.Position.ChildrenPosition_Y;
					ListBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens) listbox(ListBox_settings, control_comand);
			}
			else if(WindowProgressbarChildren == children_type)
			{
				tProgressBar *ProgressBar_settings = (tProgressBar *)children;
				if(refresh_childrens) ProgressBar_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ProgressBar_settings->Color = settings->Internals.Childrens[Tmp_Children_Cnt]->Color;
					ProgressBar_settings->Enabled = settings->Internals.Childrens[Tmp_Children_Cnt]->Enabled;
					ProgressBar_settings->Visible = settings->Internals.Childrens[Tmp_Children_Cnt]->Visible;
					ProgressBar_settings->StateChangedOn = settings->Internals.Childrens[Tmp_Children_Cnt]->StateChangedOn;
					ProgressBar_settings->Caption = settings->Internals.Childrens[Tmp_Children_Cnt]->Caption;
					ProgressBar_settings->Size.X = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X;
					ProgressBar_settings->Size.Y = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y;
					ProgressBar_settings->Position.X = settings->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + 3 + settings->Internals.Position.ChildrenPosition_X;
					ProgressBar_settings->Position.Y = settings->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Header.Size.Y + 1 + settings->Internals.Position.ChildrenPosition_Y;
					ProgressBar_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens) progressbar(ProgressBar_settings, control_comand);
			}
			else if(WindowScrollbarChildren == children_type)
			{
				tScrollBar *ScrollBar_settings = (tScrollBar *)children;
				if(refresh_childrens) ScrollBar_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ScrollBar_settings->Color = settings->Internals.Childrens[Tmp_Children_Cnt]->Color;
					ScrollBar_settings->Enabled = settings->Internals.Childrens[Tmp_Children_Cnt]->Enabled;
					ScrollBar_settings->Visible = settings->Internals.Childrens[Tmp_Children_Cnt]->Visible;
					ScrollBar_settings->StateChangedOn = settings->Internals.Childrens[Tmp_Children_Cnt]->StateChangedOn;
					//ScrollBar_settings->Caption = settings->Internals.Childrens[Tmp_Children_Cnt]->Caption;
					ScrollBar_settings->Size.X = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X;
					ScrollBar_settings->Size.Y = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y;
					ScrollBar_settings->Position.X = settings->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + 3 + settings->Internals.Position.ChildrenPosition_X;
					ScrollBar_settings->Position.Y = settings->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Header.Size.Y + 1 + settings->Internals.Position.ChildrenPosition_Y;
					ScrollBar_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens) scrollbar(ScrollBar_settings, control_comand);
			}
			else if(WindowTextboxChildren == children_type)
			{
				tTextBox *TextBox_settings = (tTextBox *)children;
				if(refresh_childrens) TextBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					TextBox_settings->Color = settings->Internals.Childrens[Tmp_Children_Cnt]->Color;
					TextBox_settings->Enabled = settings->Internals.Childrens[Tmp_Children_Cnt]->Enabled;
					TextBox_settings->Visible = settings->Internals.Childrens[Tmp_Children_Cnt]->Visible;
					TextBox_settings->StateChangedOn = settings->Internals.Childrens[Tmp_Children_Cnt]->StateChangedOn;
					TextBox_settings->Caption = settings->Internals.Childrens[Tmp_Children_Cnt]->Caption;
					TextBox_settings->Size.X = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.X;
					TextBox_settings->Size.Y = settings->Internals.Childrens[Tmp_Children_Cnt]->Size.Y;
					TextBox_settings->Position.X = settings->Position.X + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.X + 3 + settings->Internals.Position.ChildrenPosition_X;
					TextBox_settings->Position.Y = settings->Position.Y + settings->Internals.Childrens[Tmp_Children_Cnt]->Position.Y + settings->Internals.Header.Size.Y + 1 + settings->Internals.Position.ChildrenPosition_Y;
					TextBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens) textbox(TextBox_settings, control_comand);
			}
		}
		Tmp_Children_Cnt++;
	}
	//control_comand->Cursor = back;
	if(call_childrens == true && settings->Internals.ChildrensNr != 0) settings->Internals.pDisplay->sClipRegion = sClipRegion;
}
//#######################################################################################
void window(struct Window_s *settings, tControlCommandData* control_comand)
{
	if((control_comand->CursorCoordonateUsed == true || settings == NULL) && control_comand->Comand == Control_Nop) return;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse command */
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

		settings->Internals.Header.Close = new_button(settings->Internals.pDisplay);
		if(!settings->Internals.Header.Close) return;

		settings->Internals.Header.MaxMin = new_checkbox(settings->Internals.pDisplay);
		if(!settings->Internals.Header.MaxMin) return;
		if(settings->Internals.FullScreen) settings->Internals.Header.MaxMin->Cheched = false;
		else settings->Internals.Header.MaxMin->Cheched = true;

		settings->Internals.H_ScrollBar = new_scrollbar(settings->Internals.pDisplay);
		settings->Internals.V_ScrollBar = new_scrollbar(settings->Internals.pDisplay);

		settings->Internals.Header.Minimize = new_button(settings->Internals.pDisplay);
		if(!settings->Internals.Header.Minimize) return;

		ChildrenWindowSize_t ChildrenWindowSize;
		window_set_children_settings(settings, false, true, control_comand, false, &ChildrenWindowSize);
	}
	/* Verify if position on size has been modified */
	if(settings->Position.X != settings->Internals.Position.X ||
			settings->Position.Y != settings->Internals.Position.Y ||
				settings->Size.X != settings->Internals.Size.X ||
					settings->Size.Y != settings->Internals.Size.Y ||
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
		settings->Internals.OldStateVisible = settings->Visible;
		tRectangle back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = X_StartBox;
		pDisplay->sClipRegion.sYMin = Y_StartBox;
		pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
		pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		//if(!settings->Internals.NoPaintBackGround)
		//{
		put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, settings->Color.Scren);
		box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
		//}
		pDisplay->sClipRegion = back_up_clip;
	}
	/* Verify if is Entire refresh, entire repaint, or state changed */
	if(settings->Visible == true && (
			settings->Internals.NeedEntireRefresh == true ||
				settings->Internals.NeedEntireRepaint == true ||
					settings->Internals.Control.Initiated == false ||
						settings->Internals.OldStateEnabled != settings->Enabled ||
							settings->Internals.OldStateVisible != settings->Visible ||
								settings->Internals.Size.ScrollBarSize != settings->Size.ScrollBarSize))
	{
		/* Copy new locations and dimensions to actual locations and dimensions */
		settings->Internals.Position.X = settings->Position.X;
		settings->Internals.Position.Y = settings->Position.Y;
		settings->Internals.Size.X = settings->Size.X;
		settings->Internals.Size.Y = settings->Size.Y;
		settings->Internals.Caption.Font = settings->Caption.Font;
		settings->Internals.Caption.Text = settings->Caption.Text;
		settings->Internals.Caption.TextAlign = settings->Caption.TextAlign;
		settings->Internals.Caption.WordWrap = settings->Caption.WordWrap;
		settings->Internals.Size.ScrollBarSize = settings->Size.ScrollBarSize;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		settings->Caption.Text = settings->Internals.Caption.Text;

		unsigned int header_btn_size = settings->Internals.Header.Size.Y - 6;
		unsigned int header_btn_space = settings->Internals.Header.Size.Y - 5;

		settings->Internals.Header.Close->Size.X = header_btn_size;
		settings->Internals.Header.Close->Size.Y = header_btn_size;
		settings->Internals.Header.Close->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - header_btn_space - 1;
		settings->Internals.Header.Close->Position.Y = settings->Internals.Position.Y + 2;
		settings->Internals.Header.Close->Caption.Text = NULL;
		settings->Internals.Header.Close->Internals.IsChildren = true;
		settings->Internals.Header.Close->Internals.NoPaintBackGround = true;

		settings->Internals.Header.MaxMin->Size.X = header_btn_size;
		settings->Internals.Header.MaxMin->Size.Y = header_btn_size;
		settings->Internals.Header.MaxMin->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - (header_btn_space << 1) - 1;
		settings->Internals.Header.MaxMin->Position.Y = settings->Internals.Position.Y + 2;
		settings->Internals.Header.MaxMin->Caption.Text = NULL;
		settings->Internals.Header.MaxMin->Internals.NoPaintBackGround = true;

		settings->Internals.Header.Minimize->Size.X = header_btn_size;
		settings->Internals.Header.Minimize->Size.Y = header_btn_size;
		settings->Internals.Header.Minimize->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - ((header_btn_space << 1) + header_btn_space) - 1;
		settings->Internals.Header.Minimize->Position.Y = settings->Internals.Position.Y + 2;
		settings->Internals.Header.Minimize->Caption.Text = NULL;
		settings->Internals.Header.Minimize->Internals.NoPaintBackGround = true;
		settings->Internals.Header.Minimize->Internals.IsChildren = true;

		settings->Internals.H_ScrollBar->Position.X = settings->Internals.Position.X + 3;
		settings->Internals.H_ScrollBar->Position.Y = (settings->Internals.Position.Y + settings->Internals.Size.Y) - settings->Internals.Size.ScrollBarSize - 2;
		settings->Internals.H_ScrollBar->Size.X = settings->Internals.Size.X - 4 - settings->Internals.Size.ScrollBarSize;
		settings->Internals.H_ScrollBar->Size.Y = settings->Internals.Size.ScrollBarSize;
		settings->Internals.H_ScrollBar->Internals.NoPaintBackGround = true;

		settings->Internals.V_ScrollBar->Position.X = (settings->Internals.Position.X + settings->Internals.Size.X) - settings->Internals.Size.ScrollBarSize - 2;
		settings->Internals.V_ScrollBar->Position.Y = settings->Internals.Position.Y +  settings->Internals.Header.Size.Y;
		settings->Internals.V_ScrollBar->Size.X = settings->Internals.Size.ScrollBarSize;
		settings->Internals.V_ScrollBar->Size.Y = settings->Internals.Size.Y - 1 - settings->Internals.Size.ScrollBarSize - settings->Internals.Header.Size.Y;
		settings->Internals.V_ScrollBar->Internals.NoPaintBackGround = true;

		tRectangle back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = X_StartBox;
		pDisplay->sClipRegion.sYMin = Y_StartBox;
		pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
		pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, false, controlls_change_color(controls_color.Control_Color_Enabled_Border_Pull, - 3));
		put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, settings->Internals.Header.Size.Y - 2, true, controlls_change_color(controls_color.Control_Color_Enabled_Buton_Pull, - 2));
		put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + settings->Internals.Header.Size.Y, X_LenBox - 2, Y_LenBox - settings->Internals.Header.Size.Y - 1, true, controlls_change_color(controls_color.Scren, 1.2));
		box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);


		CursorState cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		//ControlCommands control_comand_comand = control_comand->Comand;
		ChildrenWindowSize_t ChildrenWindowSize;
		window_set_children_settings(settings, true, true, control_comand, true, &ChildrenWindowSize);
		settings->Internals.ChildrenWindowSize.X = ChildrenWindowSize.X;
		settings->Internals.ChildrenWindowSize.Y = ChildrenWindowSize.Y;
		settings->Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (settings->Size.X - 3/* - settings->Internals.Size.ScrollBarSize*/);
		settings->Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (settings->Size.Y - 3/* - settings->Internals.Size.ScrollBarSize - settings->Internals.Header.Size.Y*/);
		if(settings->Internals.V_ScrollBar->Maximum <= 0 && settings->Internals.H_ScrollBar->Maximum <= 0)
		{
			settings->Internals.V_ScrollBar->Maximum = 0;
			settings->Internals.V_ScrollBar->Size.X = 0;
			settings->Internals.H_ScrollBar->Maximum = 0;
			settings->Internals.H_ScrollBar->Size.Y = 0;
		}
		else
		{
			settings->Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (settings->Size.X - 4 - settings->Internals.Size.ScrollBarSize);
			settings->Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (settings->Size.Y - 4 - settings->Internals.Size.ScrollBarSize - settings->Internals.Header.Size.Y);
			if(settings->Internals.V_ScrollBar->Maximum <= 0)
			{
				settings->Internals.V_ScrollBar->Maximum = 0;
				settings->Internals.V_ScrollBar->Size.X = 0;
			}
			else settings->Internals.V_ScrollBar->Size.X = settings->Internals.Size.ScrollBarSize;
			if(settings->Internals.H_ScrollBar->Maximum <= 0)
			{
				settings->Internals.H_ScrollBar->Maximum = 0;
				settings->Internals.H_ScrollBar->Size.Y = 0;
			}
			else settings->Internals.H_ScrollBar->Size.Y = settings->Internals.Size.ScrollBarSize;
		}

		settings->Internals.V_ScrollBar->Size.Y = settings->Internals.Size.Y - 3 - settings->Internals.H_ScrollBar->Size.Y - settings->Internals.Header.Size.Y;
		settings->Internals.H_ScrollBar->Size.X = settings->Internals.Size.X - 6 - settings->Internals.V_ScrollBar->Size.X;

		control_comand->Cursor = Cursor_Up;
		window_set_children_settings(settings, false, true, control_comand, true, &ChildrenWindowSize);
		button(settings->Internals.Header.Close, control_comand);
		checkbox(settings->Internals.Header.MaxMin, control_comand);
		button(settings->Internals.Header.Minimize, control_comand);
		scrollbar(settings->Internals.H_ScrollBar, control_comand);
		scrollbar(settings->Internals.V_ScrollBar, control_comand);
		pDisplay->sClipRegion = back_up_clip;
		//control_comand->Comand = control_comand_comand;
		control_comand->Cursor = cursor;

		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.OldStateEnabled = settings->Enabled;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		control_comand->CursorCoordonateUsed = true;
		return;
	}

	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = X_StartBox;
	pDisplay->sClipRegion.sYMin = Y_StartBox;
	pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
	pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	button(settings->Internals.Header.Close, control_comand);
	checkbox(settings->Internals.Header.MaxMin, control_comand);
	bool full_screen_has_changed_state = false;
	if(settings->Internals.Header.MaxMin->Events.Checked)
	{
		settings->Internals.Header.MaxMin->Events.Checked = false;
		settings->Size.X = settings->SizeNormalScreen.X;
		settings->Size.Y = settings->SizeNormalScreen.Y;
		settings->Position.X = settings->Internals.NormalScreenPosition.X;
		settings->Position.Y = settings->Internals.NormalScreenPosition.Y;
		settings->Internals.FullScreen = false;
		full_screen_has_changed_state = true;
	}
	else if(settings->Internals.Header.MaxMin->Events.UnChecked)
	{
		settings->Internals.Header.MaxMin->Events.UnChecked = false;
		settings->Internals.NormalScreenPosition.X = settings->Position.X;
		settings->Internals.NormalScreenPosition.Y = settings->Position.Y;
		settings->Size.X = settings->SizeFullScreen.X;
		settings->Size.Y = settings->SizeFullScreen.Y;
		settings->Position.X = 0;
		settings->Position.Y = 0;
		settings->Internals.FullScreen = true;
		full_screen_has_changed_state = true;
	}
	button(settings->Internals.Header.Minimize, control_comand);
	scrollbar(settings->Internals.H_ScrollBar, control_comand);
	scrollbar(settings->Internals.V_ScrollBar, control_comand);
	ChildrenWindowSize_t ChildrenWindowSize;
	window_set_children_settings(settings, true, false, control_comand, false, &ChildrenWindowSize);
	if(settings->Internals.ChildrenWindowSize.X != ChildrenWindowSize.X || settings->Internals.ChildrenWindowSize.Y != ChildrenWindowSize.Y)
	{
		settings->Internals.ChildrenWindowSize.X = ChildrenWindowSize.X;
		settings->Internals.ChildrenWindowSize.Y = ChildrenWindowSize.Y;
		settings->Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (settings->Size.X - 3/* - settings->Internals.Size.ScrollBarSize*/);
		settings->Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (settings->Size.Y - 3/* - settings->Internals.Size.ScrollBarSize - settings->Internals.Header.Size.Y*/);
		if(settings->Internals.V_ScrollBar->Maximum <= 0 && settings->Internals.H_ScrollBar->Maximum <= 0)
		{
			settings->Internals.V_ScrollBar->Maximum = 0;
			settings->Internals.V_ScrollBar->Size.X = 0;
			settings->Internals.H_ScrollBar->Maximum = 0;
			settings->Internals.H_ScrollBar->Size.Y = 0;
		}
		else
		{
			settings->Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (settings->Size.X - 4 - settings->Internals.Size.ScrollBarSize);
			settings->Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (settings->Size.Y - 4 - settings->Internals.Size.ScrollBarSize - settings->Internals.Header.Size.Y);
			if(settings->Internals.V_ScrollBar->Maximum <= 0)
			{
				settings->Internals.V_ScrollBar->Maximum = 0;
				settings->Internals.V_ScrollBar->Size.X = 0;
			}
			else settings->Internals.V_ScrollBar->Size.X = settings->Internals.Size.ScrollBarSize;
			if(settings->Internals.H_ScrollBar->Maximum <= 0)
			{
				settings->Internals.H_ScrollBar->Maximum = 0;
				settings->Internals.H_ScrollBar->Size.Y = 0;
			}
			else settings->Internals.H_ScrollBar->Size.Y = settings->Internals.Size.ScrollBarSize;
		}

		settings->Internals.V_ScrollBar->Size.Y = settings->Internals.Size.Y - 3 - settings->Internals.H_ScrollBar->Size.Y - settings->Internals.Header.Size.Y;
		settings->Internals.H_ScrollBar->Size.X = settings->Internals.Size.X - 6 - settings->Internals.V_ScrollBar->Size.X;
	}
	if(settings->Internals.H_ScrollBar->Events.ValueChanged || settings->Internals.V_ScrollBar->Events.ValueChanged)
	{
		settings->Internals.H_ScrollBar->Events.ValueChanged = false;
		settings->Internals.V_ScrollBar->Events.ValueChanged = false;
		put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + settings->Internals.Header.Size.Y, X_LenBox - 3 - settings->Internals.V_ScrollBar->Size.X, Y_LenBox - settings->Internals.H_ScrollBar->Size.Y - 2 - settings->Internals.Size.ScrollBarSize, true, controlls_change_color(controls_color.Scren, 1.2));
		box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
		settings->Internals.Position.ChildrenPosition_X = -settings->Internals.H_ScrollBar->Value;
		settings->Internals.Position.ChildrenPosition_Y = -settings->Internals.V_ScrollBar->Value;
		CursorState cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		window_set_children_settings(settings, true, true, control_comand, true, &ChildrenWindowSize);
		control_comand->Cursor = cursor;
		control_comand->CursorCoordonateUsed = true;
	}
	pDisplay->sClipRegion = back_up_clip;

	if(full_screen_has_changed_state) return;

	if(check_if_inside_box(X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, settings->Internals.Header.Size.Y - 2, control_comand->X, control_comand->Y) && control_comand->CursorCoordonateUsed == false) settings->Internals.CursorDownOnHeader = true;
	
	if(settings->Internals.CursorDownOnHeader == true &&
		control_comand->CursorCoordonateUsed == false &&
			settings->Internals.FullScreen == false)
	{
		if(control_comand->Cursor == Cursor_Down)
		{
			settings->Internals.CursorDownOnHeader = true;
			settings->Internals.HeaderTouchDownPointX = control_comand->X;
			settings->Internals.HeaderTouchDownPointY = control_comand->Y;
			settings->Internals.WindowTouchDownPointX = settings->Position.X;
			settings->Internals.WindowTouchDownPointY = settings->Position.Y;
		}
		else if(control_comand->Cursor == Cursor_Move && settings->Internals.CursorDownOnHeader == true)
		{
			settings->Position.X = settings->Internals.WindowTouchDownPointX + (control_comand->X - settings->Internals.HeaderTouchDownPointX);
			settings->Position.Y = settings->Internals.WindowTouchDownPointY + (control_comand->Y - settings->Internals.HeaderTouchDownPointY);
		}
		if(control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction) settings->Internals.CursorDownOnHeader = false;

	}

}
//#######################################################################################
tWindow *new_window(tDisplay *ScreenDisplay)
{
	struct Window_s* settings = (struct Window_s*)calloc(1, sizeof(struct Window_s));

	if(!settings || !ScreenDisplay) return NULL;

	/*settings->Internals.Childrens = (window_children_t **)calloc(1, sizeof(settings->Internals.Childrens));
	if(!settings->Internals.Childrens)
	{
		if(settings) free(settings);
		return NULL;
	}*/
	settings->Internals.pDisplay = ScreenDisplay;
	settings->Caption.TextAlign = Align_Center;
	settings->Caption.WordWrap = false;
	settings->Caption.Font = controls_color.DefaultFont;
	settings->Caption.Text = "Window";
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

	settings->Position.X = 0;
	settings->Position.Y = 0;
	settings->Size.X = ScreenDisplay->Width;
	settings->Size.Y = ScreenDisplay->Height;
	settings->Size.ScrollBarSize = 20;
	settings->SizeFullScreen.X = ScreenDisplay->Width;
	settings->SizeFullScreen.Y = ScreenDisplay->Height;
	settings->SizeNormalScreen.X = 200;
	settings->SizeNormalScreen.Y = 150;
	settings->StateChangedOn = Cursor_Up;
	settings->Visible = true;
	settings->Enabled = true;

	settings->Internals.Header.Size.Y = 20;
	settings->Internals.FullScreen = true;

	settings->idle = window;
	settings->add_children = window_add_children;
	settings->get_children_address = window_get_children_address;
	settings->get_children_index = window_get_children_index;
	settings->set_list_of_childrens = window_set_list_of_childrens;
	return settings;
}
//#######################################################################################
void* window_add_children(struct Window_s *settings, unsigned int children_type, char *children_name)
{
	if(!settings) return NULL;
	void *children_addr = NULL;
	switch(children_type)
	{
	case WindowButonChildren:
		children_addr = (void *)new_button(settings->Internals.pDisplay);
		break;
	case WindowCheckboxChildren:
		children_addr = (void *)new_checkbox(settings->Internals.pDisplay);
		break;
	case WindowListboxChildren:
		children_addr = (void *)new_listbox(settings->Internals.pDisplay);
		break;
	case WindowProgressbarChildren:
		children_addr = (void *)new_progressbar(settings->Internals.pDisplay);
		break;
	case WindowScrollbarChildren:
		children_addr = (void *)new_scrollbar(settings->Internals.pDisplay);
		break;
	case WindowTextboxChildren:
		children_addr = (void *)new_textbox(settings->Internals.pDisplay);
		break;
	default:
		return NULL;
	}
	if(!children_addr) return NULL;
	unsigned int added_children = (unsigned int)settings->Internals.ChildrensNr;
	if(!added_children)
	{
		settings->Internals.Childrens = (window_children_t **)calloc(1, sizeof(window_children_t *));
		if(!settings->Internals.Childrens) goto window_can_not_add_children;
	}
	else
	{
		window_children_t **new_pointer_of_childrens = (window_children_t **)realloc(settings->Internals.Childrens, sizeof(window_children_t *) * (added_children + 1));
		if(!new_pointer_of_childrens) goto window_can_not_add_children;
		settings->Internals.Childrens = new_pointer_of_childrens;
	}
	settings->Internals.Childrens[added_children] = (window_children_t *)calloc(1, sizeof(window_children_t));
	if(!settings->Internals.Childrens[added_children]) goto window_can_not_add_children;
	settings->Internals.Childrens[added_children]->Children = children_addr;
	settings->Internals.Childrens[added_children]->ChildrenName = children_name;
	settings->Internals.Childrens[added_children]->Type = children_type;
	settings->Internals.Childrens[added_children]->Caption.Text = children_name;
	settings->Internals.Childrens[added_children]->Caption.Font = settings->Caption.Font;
	settings->Internals.Childrens[added_children]->Caption.TextAlign = settings->Caption.TextAlign;
	settings->Internals.Childrens[added_children]->Caption.WordWrap = settings->Caption.WordWrap;
	settings->Internals.Childrens[added_children]->Enabled = settings->Enabled;
	settings->Internals.Childrens[added_children]->Visible = settings->Visible;
	settings->Internals.Childrens[added_children]->Color = settings->Color;
	switch(children_type)
	{
	case WindowButonChildren:
		settings->Internals.Childrens[added_children]->Position.X = settings->Position.X;
		settings->Internals.Childrens[added_children]->Position.Y = settings->Position.Y;
		settings->Internals.Childrens[added_children]->Size.X = 40;//settings->Size.X;
		settings->Internals.Childrens[added_children]->Size.Y = 20;//settings->Size.Y;
		settings->Internals.Childrens[added_children]->StateChangedOn = settings->StateChangedOn;
		break;
	case WindowCheckboxChildren:
		settings->Internals.Childrens[added_children]->Position.X = settings->Position.X + 45;
		settings->Internals.Childrens[added_children]->Position.Y = settings->Position.Y;
		settings->Internals.Childrens[added_children]->Size.X = 60;//settings->Size.X;
		settings->Internals.Childrens[added_children]->Size.Y = 20;//settings->Size.Y;
		settings->Internals.Childrens[added_children]->StateChangedOn = settings->StateChangedOn;
		break;
	case WindowListboxChildren:
		settings->Internals.Childrens[added_children]->Position.X = settings->Position.X + 110;
		settings->Internals.Childrens[added_children]->Position.Y = settings->Position.Y;
		settings->Internals.Childrens[added_children]->Size.X = 200;//settings->Size.X;
		settings->Internals.Childrens[added_children]->Size.Y = 70;//settings->Size.Y;
		settings->Internals.Childrens[added_children]->StateChangedOn = settings->StateChangedOn;
		break;
	case WindowProgressbarChildren:
		settings->Internals.Childrens[added_children]->Position.X = settings->Position.X;
		settings->Internals.Childrens[added_children]->Position.Y = settings->Position.Y + 25;
		settings->Internals.Childrens[added_children]->Size.X = 105;//settings->Size.X;
		settings->Internals.Childrens[added_children]->Size.Y = 20;//settings->Size.Y;
		settings->Internals.Childrens[added_children]->StateChangedOn = Cursor_Move;//settings->StateChangedOn;
		break;
	case WindowScrollbarChildren:
		settings->Internals.Childrens[added_children]->Position.X = settings->Position.X;
		settings->Internals.Childrens[added_children]->Position.Y = settings->Position.Y + 50;
		settings->Internals.Childrens[added_children]->Size.X = 105;//settings->Size.X;
		settings->Internals.Childrens[added_children]->Size.Y = 20;//settings->Size.Y;
		settings->Internals.Childrens[added_children]->StateChangedOn = settings->StateChangedOn;
		break;
	case WindowTextboxChildren:
		settings->Internals.Childrens[added_children]->Position.X = settings->Position.X ;
		settings->Internals.Childrens[added_children]->Position.Y = settings->Position.Y + 75;
		settings->Internals.Childrens[added_children]->Size.X = 310;//settings->Size.X;
		settings->Internals.Childrens[added_children]->Size.Y = 160;//settings->Size.Y;
		settings->Internals.Childrens[added_children]->StateChangedOn = settings->StateChangedOn;
		settings->Internals.Childrens[added_children]->Caption.Text = "Multiplatform SDK to create standalone applications\n\r1\n\r2\n\r3\n\r4\n\r5\n\r6\n\r7\n\r8\n\r9\n\r10\n\r11\n\r12\n\r13\n\r14\n\r15\n\r16\n\r17\n\r18";
		break;
	}
	settings->Internals.ChildrensNr++;
	ChildrenWindowSize_t ChildrenWindowSize;
	window_set_children_settings(settings, false, true, NULL, false, &ChildrenWindowSize);
	return children_addr;

window_can_not_add_children:
	switch(children_type)
	{
	case WindowButonChildren:
		free_button(children_addr);
		break;
	case WindowCheckboxChildren:
		free_checkbox(children_addr);
		break;
	case WindowListboxChildren:
		free_listbox(children_addr);
		break;
	case WindowProgressbarChildren:
		free_progressbar(children_addr);
		break;
	case WindowScrollbarChildren:
		free_scrollbar(children_addr);
		break;
	case WindowTextboxChildren:
		free_textbox(children_addr);
		break;
	}
	return NULL;
}
//#######################################################################################
signed int window_get_children_index(struct Window_s *settings, char *name)
{
	unsigned int CntChildrens = 0;
	unsigned int NemeLength = strlen(name);
	for(; CntChildrens < settings->Internals.ChildrensNr; CntChildrens++)
	{
		if(strlen(settings->Internals.Childrens[CntChildrens]->ChildrenName) == NemeLength)
		{
			if(!strcmp(settings->Internals.Childrens[CntChildrens]->ChildrenName, name))
			{
				return CntChildrens;
			}
		}
	}
	return -1;
}
//#######################################################################################
window_children_t *window_get_children_address(struct Window_s *settings, char *name)
{
	signed int TmpChildrenIndex = window_get_children_index(settings, name);
	if(TmpChildrenIndex < 0) return NULL;
	else return settings->Internals.Childrens[TmpChildrenIndex];
}
//#######################################################################################
bool window_set_list_of_childrens(struct Window_s *settings, window_children_t **list, unsigned int num_of_childrens)
{
	if(settings == 0 && settings->Internals.ChildrensNr != 0) return false;
	settings->Internals.Childrens = (window_children_t **)calloc(1, sizeof(settings->Internals.Childrens));
	if(!settings->Internals.Childrens) return false;
	settings->Internals.Childrens = list;
	return true;
}
//#######################################################################################
