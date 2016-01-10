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
#include "window_def.h"
#include "graphic_string.h"
#include "controls_definition.h"
#include "sys/plat_properties.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "button.h"
#include "checkbox.h"
#include "listbox.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "textbox.h"
#include "picturebox.h"
#include "keyboard.h"
#include "resource/fonts.h"
//#######################################################################################
void window(struct Window_s *settings, tControlCommandData *control_comand);
void* window_add_children(struct Window_s *settings, unsigned int children_type, char *children_name);
signed int window_get_children_index(struct Window_s *settings, char *name);
window_children_t *window_get_children_address(struct Window_s *settings, char *name);
bool window_set_list_of_childrens(struct Window_s *settings, window_children_t **list, unsigned int num_of_childrens);
//#######################################################################################
void window_set_children_settings(tWindow *settings, bool call_childrens, bool transfer_settings, tControlCommandData* control_comand, bool refresh_childrens, ChildrenWindowSize_t *ChildrenWindowSize)
{
	unsigned int Tmp_Children_Cnt = 0;
	tRectangle sClipRegion;
	tWindow *ParentWindow = (tWindow *)settings->Internals.ParentWindow;
	signed int ParentHeaderSize = 0;
	if(ParentWindow->Internals.HideHeader) ParentHeaderSize = ParentWindow->Internals.Header.Size.Y;
	signed int HeaderSize = 0;
	if(settings->Internals.HideHeader == false) HeaderSize = settings->Internals.Header.Size.Y;
	if(ParentWindow != NULL && settings != ParentWindow)
	{
		if(settings->Internals.V_ScrollBar && settings->Internals.H_ScrollBar)
		{
			sClipRegion = settings->Internals.pDisplay->sClipRegion;
			settings->Internals.pDisplay->sClipRegion.sXMin = (settings->Position.X + 5 + ParentWindow->Internals.Position.X) - ParentWindow->Internals.H_ScrollBar->Value;
			settings->Internals.pDisplay->sClipRegion.sXMax = (settings->Position.X + settings->Size.X + ParentWindow->Internals.Position.X + 1) - 0 - settings->Internals.V_ScrollBar->Size.X - ParentWindow->Internals.H_ScrollBar->Value;
			settings->Internals.pDisplay->sClipRegion.sYMin = (settings->Position.Y + 1 + HeaderSize + ParentHeaderSize + ParentWindow->Internals.Position.Y) - ParentWindow->Internals.V_ScrollBar->Value;
			settings->Internals.pDisplay->sClipRegion.sYMax = (settings->Position.Y + HeaderSize + settings->Size.Y + ParentWindow->Internals.Position.Y) - 0 - settings->Internals.H_ScrollBar->Size.Y - ParentWindow->Internals.V_ScrollBar->Value;
			clip_limit(&settings->Internals.pDisplay->sClipRegion, &sClipRegion);
		}
	}
	else
	{
		if(settings->Internals.V_ScrollBar && settings->Internals.H_ScrollBar)
		{
			sClipRegion = settings->Internals.pDisplay->sClipRegion;
			settings->Internals.pDisplay->sClipRegion.sXMin = settings->Position.X + 2;
			settings->Internals.pDisplay->sClipRegion.sXMax = (settings->Position.X + settings->Size.X) - 2 - settings->Internals.V_ScrollBar->Size.X;
			settings->Internals.pDisplay->sClipRegion.sYMin = settings->Position.Y + 1 + HeaderSize;
			settings->Internals.pDisplay->sClipRegion.sYMax = (settings->Position.Y + settings->Size.Y) - 2 - settings->Internals.H_ScrollBar->Size.Y;
			clip_limit(&settings->Internals.pDisplay->sClipRegion, &sClipRegion);
		}
	}
	ChildrenWindowSize->X = 0;
	ChildrenWindowSize->Y = 0;

	CursorState back = Cursor_NoAction;
	if(control_comand) back = control_comand->Cursor;
	if(control_comand && (settings->Internals.CursorDownOnHeader || settings->Internals.CursorDownOnResizeBtn || refresh_childrens)) control_comand->Cursor = Cursor_Up;
	Tmp_Children_Cnt = 0;
	while(Tmp_Children_Cnt < settings->Internals.ChildrensNr && settings->Internals.ChildrensNr != 0)
	{
		if(settings->Internals.Childrens[Tmp_Children_Cnt] && settings->Internals.Childrens[Tmp_Children_Cnt]->TabLocNr == settings->SelectedTab)
		{
			unsigned int children_type = settings->Internals.Childrens[Tmp_Children_Cnt]->Type;
			void *children = settings->Internals.Childrens[Tmp_Children_Cnt]->Children;
			if(WindowWindowChildren == children_type || WindowTabGroupChildren == children_type)
			{
				tWindow *Window_settings = (tWindow *)children;
				/*if(control_comand->Cursor == Cursor_Down &&
						check_if_inside_box(Window_settings->Internals.Position.X,
							Window_settings->Internals.Position.Y,
								Window_settings->Internals.Position.X +  Window_settings->Internals.Size.X,
									Window_settings->Internals.Position.Y + Window_settings->Internals.Size.Y,
										control_comand->X, control_comand->Y))
					settings->Internals.CursorDownInsideChildrenWindow = true;*/
				tRectangle sClipRegion;
				sClipRegion.sXMin = Window_settings->Internals.Position.X;
				sClipRegion.sYMin = Window_settings->Internals.Position.Y;
				sClipRegion.sXMax = Window_settings->Internals.Position.X +  Window_settings->Internals.Size.X;
				sClipRegion.sYMax = Window_settings->Internals.Position.Y +  Window_settings->Internals.Size.Y;
				clip_limit(&sClipRegion, &Window_settings->WindowMoveLimits);
				CursorState Cursor = Cursor_NoAction;
				if(control_comand) Cursor = control_comand->Cursor;
				if(Cursor == Cursor_Down &&
						check_if_inside_box(sClipRegion.sXMin,
							sClipRegion.sYMin,
								sClipRegion.sXMax,
									sClipRegion.sYMax,
										control_comand->X,
											control_comand->Y))
					settings->Internals.CursorDownInsideChildrenWindow = true;
			}
		}
		Tmp_Children_Cnt++;
	}
	CursorState _back;
	if(control_comand)
	{
		_back = control_comand->Cursor;
		if(settings->Internals.CursorDownInsideChildrenWindow) control_comand->Cursor = Cursor_NoAction;
	}
	//bool _back_cursor_used = control_comand->CursorCoordonateUsed;
	Tmp_Children_Cnt = 0;
	bool back_need_window_refrash = false;
	if(control_comand)
	{
		back_need_window_refrash = control_comand->WindowRefresh;
		control_comand->WindowRefresh = false;
	}
	while(Tmp_Children_Cnt < settings->Internals.ChildrensNr && settings->Internals.ChildrensNr != 0)
	{
		if(settings->Internals.Childrens[Tmp_Children_Cnt] && settings->Internals.Childrens[Tmp_Children_Cnt]->TabLocNr == settings->SelectedTab)
		{
			unsigned int children_type = settings->Internals.Childrens[Tmp_Children_Cnt]->Type;
			void *children = settings->Internals.Childrens[Tmp_Children_Cnt]->Children;
			if(WindowButtonChildren == children_type)
			{
				tButton *Buton_settings = (tButton *)children;
				if(Buton_settings->Visible)
				{
					if(ChildrenWindowSize->X < (Buton_settings->Position.X + Buton_settings->Size.X)) ChildrenWindowSize->X = Buton_settings->Position.X + Buton_settings->Size.X;
					if(ChildrenWindowSize->Y < (Buton_settings->Position.Y + Buton_settings->Size.Y)) ChildrenWindowSize->Y = Buton_settings->Position.Y + Buton_settings->Size.Y;
				}
				if(refresh_childrens) Buton_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					Buton_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					Buton_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					Buton_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(Buton_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || Buton_settings->Visible == false)
					{
						button(Buton_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = Buton_settings->Visible;
				}
			}
			else if(WindowCheckboxChildren == children_type)
			{
				tCheckBox *CheckBox_settings = (tCheckBox *)children;
				if(CheckBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (CheckBox_settings->Position.X + CheckBox_settings->Size.X)) ChildrenWindowSize->X = CheckBox_settings->Position.X + CheckBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (CheckBox_settings->Position.Y + CheckBox_settings->Size.Y)) ChildrenWindowSize->Y = CheckBox_settings->Position.Y + CheckBox_settings->Size.Y;
				}
				if(refresh_childrens) CheckBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					CheckBox_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					CheckBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					CheckBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(CheckBox_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || CheckBox_settings->Visible == false)
					{
						checkbox(CheckBox_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = CheckBox_settings->Visible;
				}
			}
			else if(WindowListboxChildren == children_type)
			{
				tListBox *ListBox_settings = (tListBox *)children;
				if(ListBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (ListBox_settings->Position.X + ListBox_settings->Size.X)) ChildrenWindowSize->X = ListBox_settings->Position.X + ListBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (ListBox_settings->Position.Y + ListBox_settings->Size.Y)) ChildrenWindowSize->Y = ListBox_settings->Position.Y + ListBox_settings->Size.Y;
				}
				if(refresh_childrens) ListBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ListBox_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					ListBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					ListBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(ListBox_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || ListBox_settings->Visible == false)
					{
						listbox(ListBox_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = ListBox_settings->Visible;
				}
			}
			else if(WindowProgressbarChildren == children_type)
			{
				tProgressBar *ProgressBar_settings = (tProgressBar *)children;
				if(ProgressBar_settings->Visible)
				{
					if(ChildrenWindowSize->X < (ProgressBar_settings->Position.X + ProgressBar_settings->Size.X)) ChildrenWindowSize->X = ProgressBar_settings->Position.X + ProgressBar_settings->Size.X;
					if(ChildrenWindowSize->Y < (ProgressBar_settings->Position.Y + ProgressBar_settings->Size.Y)) ChildrenWindowSize->Y = ProgressBar_settings->Position.Y + ProgressBar_settings->Size.Y;
				}
				if(refresh_childrens) ProgressBar_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ProgressBar_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					ProgressBar_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					ProgressBar_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(ProgressBar_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || ProgressBar_settings->Visible == false)
					{
						progressbar(ProgressBar_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = ProgressBar_settings->Visible;
				}
			}
			else if(WindowScrollbarChildren == children_type)
			{
				tScrollBar *ScrollBar_settings = (tScrollBar *)children;
				if(ScrollBar_settings->Visible)
				{
					if(ChildrenWindowSize->X < (ScrollBar_settings->Position.X + ScrollBar_settings->Size.X)) ChildrenWindowSize->X = ScrollBar_settings->Position.X + ScrollBar_settings->Size.X;
					if(ChildrenWindowSize->Y < (ScrollBar_settings->Position.Y + ScrollBar_settings->Size.Y)) ChildrenWindowSize->Y = ScrollBar_settings->Position.Y + ScrollBar_settings->Size.Y;
				}
				if(refresh_childrens) ScrollBar_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					ScrollBar_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					ScrollBar_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					ScrollBar_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(ScrollBar_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || ScrollBar_settings->Visible == false)
					{
						scrollbar(ScrollBar_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = ScrollBar_settings->Visible;
				}
			}
			else if(WindowTextboxChildren == children_type)
			{
				tTextBox *TextBox_settings = (tTextBox *)children;
				if(TextBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (TextBox_settings->Position.X + TextBox_settings->Size.X)) ChildrenWindowSize->X = TextBox_settings->Position.X + TextBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (TextBox_settings->Position.Y + TextBox_settings->Size.Y)) ChildrenWindowSize->Y = TextBox_settings->Position.Y + TextBox_settings->Size.Y;
				}
				if(refresh_childrens) TextBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					TextBox_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					TextBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					TextBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(TextBox_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || TextBox_settings->Visible == false)
					{
						textbox(TextBox_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = TextBox_settings->Visible;
				}
			}
			else if(WindowPictureboxChildren == children_type)
			{
				tPictureBox *PictureBox_settings = (tPictureBox *)children;
				if(PictureBox_settings->Visible)
				{
					if(ChildrenWindowSize->X < (PictureBox_settings->Position.X + PictureBox_settings->Size.X)) ChildrenWindowSize->X = PictureBox_settings->Position.X + PictureBox_settings->Size.X;
					if(ChildrenWindowSize->Y < (PictureBox_settings->Position.Y + PictureBox_settings->Size.Y)) ChildrenWindowSize->Y = PictureBox_settings->Position.Y + PictureBox_settings->Size.Y;
				}
				if(refresh_childrens) PictureBox_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					PictureBox_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					PictureBox_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					PictureBox_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(PictureBox_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || PictureBox_settings->Visible == false)
					{
						picturebox(PictureBox_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = PictureBox_settings->Visible;
				}
			}
			else if(WindowKeyboardChildren == children_type)
			{
				tVKbd_Qwerty *Keyboard_settings = (tVKbd_Qwerty *)children;
				if(Keyboard_settings->Visible)
				{
					if(ChildrenWindowSize->X < (Keyboard_settings->Position.X + Keyboard_settings->Size.X)) ChildrenWindowSize->X = Keyboard_settings->Position.X + Keyboard_settings->Size.X;
					if(ChildrenWindowSize->Y < (Keyboard_settings->Position.Y + Keyboard_settings->Size.Y)) ChildrenWindowSize->Y = Keyboard_settings->Position.Y + Keyboard_settings->Size.Y;
				}
				if(refresh_childrens) Keyboard_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					Keyboard_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					Keyboard_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					Keyboard_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(Keyboard_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || Keyboard_settings->Visible == false)
					{
						v_keyboard(Keyboard_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = Keyboard_settings->Visible;
				}
			}
		}
		Tmp_Children_Cnt++;
	}
	if(control_comand) control_comand->Cursor = _back;
	//if(ParentWindow->Internals.CursorDownInsideChildrenWindow) control_comand->CursorCoordonateUsed = _back_cursor_used;
	Tmp_Children_Cnt = 0;
	while(Tmp_Children_Cnt < settings->Internals.ChildrensNr && settings->Internals.ChildrensNr != 0)
	{
		if(settings->Internals.Childrens[Tmp_Children_Cnt] && settings->Internals.Childrens[Tmp_Children_Cnt]->TabLocNr == settings->SelectedTab)
		{
			unsigned int children_type = settings->Internals.Childrens[Tmp_Children_Cnt]->Type;
			void *children = settings->Internals.Childrens[Tmp_Children_Cnt]->Children;
			if(WindowWindowChildren == children_type || WindowTabGroupChildren == children_type)
			{
				tWindow *Window_settings = (tWindow *)children;
				if(Window_settings->Visible)
				{
					if(ChildrenWindowSize->X < (Window_settings->Position.X + Window_settings->Size.X)) ChildrenWindowSize->X = Window_settings->Position.X + Window_settings->Size.X;
					if(ChildrenWindowSize->Y < (Window_settings->Position.Y + Window_settings->Size.Y)) ChildrenWindowSize->Y = Window_settings->Position.Y + Window_settings->Size.Y;
				}
				bool tmp_win_rfsh = false;
				if(control_comand) tmp_win_rfsh = control_comand->WindowRefresh;
				if(refresh_childrens || tmp_win_rfsh) Window_settings->Internals.NeedEntireRefresh = true;
				if(transfer_settings)
				{
					Window_settings->Internals.PositionOffset.X = 3 + settings->Internals.Position.ChildrenPosition_X;
					Window_settings->Internals.PositionOffset.Y = HeaderSize + 1 + settings->Internals.Position.ChildrenPosition_Y;
					Window_settings->Internals.NoPaintBackGround = true;
				}
				if(call_childrens)
				{
					if(Window_settings->Visible == settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible || Window_settings->Visible == false)
					{
						window(Window_settings, control_comand);
					}
					settings->Internals.Childrens[Tmp_Children_Cnt]->Internals.OldStateVisible = Window_settings->Visible;
				}
			}
		}
		Tmp_Children_Cnt++;
	}
	if(control_comand) control_comand->WindowRefresh |= back_need_window_refrash;
	if(_back == Cursor_Up || _back == Cursor_NoAction) settings->Internals.CursorDownInsideChildrenWindow = false;
	if(control_comand) control_comand->Cursor = back;
	if(settings->Internals.V_ScrollBar && settings->Internals.H_ScrollBar) settings->Internals.pDisplay->sClipRegion = sClipRegion;
}
//#######################################################################################
void window(struct Window_s *settings, tControlCommandData* control_comand)
{
	if((/*control_comand->CursorCoordonateUsed == true || */settings == NULL) /*&& control_comand->Comand == Control_Nop*/) return;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse command */
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
		settings->Internals.WindowMoveLimits.sXMin = settings->WindowMoveLimits.sXMin;
		settings->Internals.WindowMoveLimits.sYMin = settings->WindowMoveLimits.sYMin;
		settings->Internals.WindowMoveLimits.sXMax = settings->WindowMoveLimits.sXMax;
		settings->Internals.WindowMoveLimits.sYMax = settings->WindowMoveLimits.sYMax;
		if(settings->Internals.FullScreen)
		{
			settings->Position.X = settings->Internals.WindowMoveLimits.sXMin;
			settings->Position.Y = settings->Internals.WindowMoveLimits.sYMin;
			settings->Size.X = settings->WindowMoveLimits.sXMax - settings->Internals.WindowMoveLimits.sXMin;
			settings->Size.Y = settings->WindowMoveLimits.sYMax - settings->Internals.WindowMoveLimits.sYMin;
		}
		if(ParentWindow != NULL && settings != ParentWindow)
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
		settings->Internals.NormalScreenPosition.X = settings->Position.X;
		settings->Internals.NormalScreenPosition.Y = settings->Position.Y;
		settings->Internals.Header.Close = new_button(settings->Internals.ParentWindow);
		if(!settings->Internals.Header.Close) return;

		settings->Internals.Header.MaxMin = new_checkbox(settings->Internals.ParentWindow);
		if(!settings->Internals.Header.MaxMin) return;
		if(settings->Internals.FullScreen) settings->Internals.Header.MaxMin->Cheched = false;
		else settings->Internals.Header.MaxMin->Cheched = true;

		settings->Internals.H_ScrollBar = new_scrollbar(settings->Internals.ParentWindow);
		settings->Internals.V_ScrollBar = new_scrollbar(settings->Internals.ParentWindow);

		settings->Internals.Header.Minimize = new_button(settings->Internals.ParentWindow);
		if(!settings->Internals.Header.Minimize) return;

		settings->Internals.Header.TabGroupScrollLeft = new_button(settings->Internals.ParentWindow);
		if(!settings->Internals.Header.TabGroupScrollLeft) return;

		settings->Internals.Header.TabGroupScrollRight = new_button(settings->Internals.ParentWindow);
		if(!settings->Internals.Header.TabGroupScrollRight) return;

		//settings->Internals.Header.TabGroupTabsList = (tCheckBox **)calloc(1, sizeof(tCheckBox *));
		//if(!settings->Internals.Header.TabGroupTabsList) return;

		//settings->Internals.Header.TabGroupTabsList[0] = new_checkbox(settings->Internals.ParentWindow);
		//if(!settings->Internals.Header.TabGroupTabsList[0]) return;
		//settings->Internals.Header.TabGroupTabsList[0]->Cheched = true;
		//settings->Internals.Header.TabGroupTabsList[0]->Caption.Font = (tFont *)&g_sFontCm12;
		//settings->Internals.Header.TabGroupTabsList[0]->Color.Scren = controls_color.Control_Color_Enabled_Buton_Push;
		//settings->Internals.Header.TabGroupTabsListNr = 0;


		/*settings->Internals.Header.Resize = new_button(settings->Internals.ParentWindow);
		if(!settings->Internals.Header.Resize) return;*/
		if(settings->Internals.FullScreen == false)
		{
			settings->SizeNormalScreen.X = settings->Size.X;
			settings->SizeNormalScreen.Y = settings->Size.Y;
		}

		ChildrenWindowSize_t ChildrenWindowSize;
		window_set_children_settings(settings, false, true, control_comand, false, &ChildrenWindowSize);
		settings->Internals.NeedEntireRefresh = true;
	}
	/* Verify if position on size has been modified */
	if(ParentWindow != NULL && settings != ParentWindow)
	{
		if((settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X) != settings->Internals.Position.X ||
				(settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y) != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.Caption.Font != settings->Caption.Font ||
								settings->Internals.Caption.Text != settings->Caption.Text ||
									settings->Internals.Caption.TextAlign != settings->Caption.TextAlign ||
										settings->Internals.Caption.WordWrap != settings->Caption.WordWrap ||
											settings->Internals.WindowMoveLimits.sXMin != settings->WindowMoveLimits.sXMin ||
												settings->Internals.WindowMoveLimits.sYMin != settings->WindowMoveLimits.sYMin ||
													settings->Internals.WindowMoveLimits.sXMax != settings->WindowMoveLimits.sXMax ||
														settings->Internals.WindowMoveLimits.sYMax != settings->WindowMoveLimits.sYMax ||
															settings->Internals.HideHeader != settings->HideHeader ||
																settings->Internals.OldTabGroupMode != settings->Internals.TabGroupMode)
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
											settings->Internals.WindowMoveLimits.sXMin != settings->WindowMoveLimits.sXMin ||
												settings->Internals.WindowMoveLimits.sYMin != settings->WindowMoveLimits.sYMin ||
													settings->Internals.WindowMoveLimits.sXMax != settings->WindowMoveLimits.sXMax ||
														settings->Internals.WindowMoveLimits.sYMax != settings->WindowMoveLimits.sYMax ||
															settings->Internals.HideHeader != settings->HideHeader ||
																settings->Internals.OldTabGroupMode != settings->Internals.TabGroupMode)
			settings->Internals.NeedEntireRefresh = true;
	}

	//if(settings->Internals.Caption.Text != NULL && settings->Caption.Text != NULL && strcmp(settings->Internals.Caption.Text, settings->Caption.Text) == NULL)
		//settings->Internals.NeedEntireRefresh = true;

	signed int X_StartBox = settings->Internals.Position.X;
	signed int Y_StartBox = settings->Internals.Position.Y;
	signed int X_LenBox = settings->Internals.Size.X;
	signed int Y_LenBox = settings->Internals.Size.Y;
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
			clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
			put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, true, settings->Color.Scren);
			box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);
			pDisplay->sClipRegion = back_up_clip;
			if(!settings->Visible) return;
		}
		settings->Internals.NeedEntireRefresh = true;
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
		if(ParentWindow != NULL && settings != ParentWindow)
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
		settings->Internals.Size.ScrollBarSize = settings->Size.ScrollBarSize;
		settings->Internals.WindowMoveLimits.sXMin = settings->WindowMoveLimits.sXMin;
		settings->Internals.WindowMoveLimits.sYMin = settings->WindowMoveLimits.sYMin;
		settings->Internals.WindowMoveLimits.sXMax = settings->WindowMoveLimits.sXMax;
		settings->Internals.WindowMoveLimits.sYMax = settings->WindowMoveLimits.sYMax;
		settings->Internals.OldTabGroupMode = settings->Internals.TabGroupMode;
		settings->Internals.HideHeader = settings->HideHeader;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;
		settings->Internals.Caption.Text = gfx_change_str(settings->Internals.Caption.Text, settings->Caption.Text);
		settings->Caption.Text = settings->Internals.Caption.Text;

		signed int HeaderSize = 0;
		if(settings->Internals.HideHeader == false) HeaderSize = settings->Internals.Header.Size.Y;
		unsigned int header_btn_size = HeaderSize - 6;
		unsigned int header_btn_space = HeaderSize - 5;

		settings->Internals.Header.Close->Size.X = header_btn_size;
		settings->Internals.Header.Close->Size.Y = header_btn_size;
		if(ParentWindow != NULL && settings != ParentWindow)
		{
			settings->Internals.Header.Close->Position.X = (settings->Internals.Size.X + settings->Internals.Position.X) - ParentWindow->Internals.Position.X - header_btn_space - 1;
			settings->Internals.Header.Close->Position.Y = (2 + settings->Internals.Position.Y) - ParentWindow->Internals.Position.Y;
		}
		else
		{
			settings->Internals.Header.Close->Position.X = (settings->Internals.Size.X) - header_btn_space - 1;
			settings->Internals.Header.Close->Position.Y = 2;
		}
		settings->Internals.Header.Close->Caption.Text = NULL;
		settings->Internals.Header.Close->Internals.IsChildren = true;
		settings->Internals.Header.Close->Internals.NoPaintBackGround = true;
		settings->Internals.Header.Close->Internals.NeedEntireRefresh = true;

		settings->Internals.Header.MaxMin->Size.X = header_btn_size;
		settings->Internals.Header.MaxMin->Size.Y = header_btn_size;
		if(ParentWindow != NULL && settings != ParentWindow)
		{
			settings->Internals.Header.MaxMin->Position.X = (settings->Internals.Size.X + settings->Internals.Position.X) - ParentWindow->Internals.Position.X - (header_btn_space << 1) - 1;
			settings->Internals.Header.MaxMin->Position.Y = (2 + settings->Internals.Position.Y) - ParentWindow->Internals.Position.Y;
		}
		else
		{
			settings->Internals.Header.MaxMin->Position.X = (settings->Internals.Size.X) - (header_btn_space << 1) - 1;
			settings->Internals.Header.MaxMin->Position.Y = 2;
		}
		settings->Internals.Header.MaxMin->Caption.Text = NULL;
		settings->Internals.Header.MaxMin->Internals.NoPaintBackGround = true;
		settings->Internals.Header.MaxMin->Internals.NeedEntireRefresh = true;

		settings->Internals.Header.Minimize->Size.X = header_btn_size;
		settings->Internals.Header.Minimize->Size.Y = header_btn_size;
		if(ParentWindow != NULL && settings != ParentWindow)
		{
			settings->Internals.Header.Minimize->Position.X = (settings->Internals.Size.X + settings->Internals.Position.X) - ParentWindow->Internals.Position.X - ((header_btn_space << 1) + header_btn_space) - 1;
			settings->Internals.Header.Minimize->Position.Y = (2 + settings->Internals.Position.Y) - ParentWindow->Internals.Position.Y;
		}
		else
		{
			settings->Internals.Header.Minimize->Position.X = (settings->Internals.Size.X) - ((header_btn_space << 1) + header_btn_space) - 1;
			settings->Internals.Header.Minimize->Position.Y = 2;
		}
		settings->Internals.Header.Minimize->Caption.Text = NULL;
		settings->Internals.Header.Minimize->Internals.NoPaintBackGround = true;
		settings->Internals.Header.Minimize->Internals.IsChildren = true;
		settings->Internals.Header.Minimize->Internals.NeedEntireRefresh = true;

		settings->Internals.Header.TabGroupScrollLeft->Size.X = header_btn_size;
		settings->Internals.Header.TabGroupScrollLeft->Size.Y = header_btn_size;
		if(ParentWindow != NULL && settings != ParentWindow)
		{
			settings->Internals.Header.TabGroupScrollLeft->Position.X = (settings->Internals.Position.X + 2) - ParentWindow->Internals.Position.X;
			settings->Internals.Header.TabGroupScrollLeft->Position.Y = (2 + settings->Internals.Position.Y) - ParentWindow->Internals.Position.Y;
		}
		else
		{
			settings->Internals.Header.TabGroupScrollLeft->Position.X = (settings->Internals.Position.X + 2);
			settings->Internals.Header.TabGroupScrollLeft->Position.Y = 2;
		}
		settings->Internals.Header.TabGroupScrollLeft->Caption.Text = "L";
		settings->Internals.Header.TabGroupScrollLeft->Caption.WordWrap = false;
		//settings->Internals.Header.TabGroupScrollLeft->Caption.Font = &g_sFontCm12;
		settings->Internals.Header.TabGroupScrollLeft->Internals.NoPaintBackGround = true;
		settings->Internals.Header.TabGroupScrollLeft->Internals.IsChildren = true;
		settings->Internals.Header.TabGroupScrollLeft->Internals.NeedEntireRefresh = true;

		settings->Internals.Header.TabGroupScrollRight->Size.X = header_btn_size;
		settings->Internals.Header.TabGroupScrollRight->Size.Y = header_btn_size;
		if(ParentWindow != NULL && settings != ParentWindow)
		{
			settings->Internals.Header.TabGroupScrollRight->Position.X = (settings->Internals.Size.X + settings->Internals.Position.X) - ParentWindow->Internals.Position.X - header_btn_space - 1;
			settings->Internals.Header.TabGroupScrollRight->Position.Y = (2 + settings->Internals.Position.Y) - ParentWindow->Internals.Position.Y;
		}
		else
		{
			settings->Internals.Header.TabGroupScrollRight->Position.X = (settings->Internals.Size.X) - header_btn_space - 1;
			settings->Internals.Header.TabGroupScrollRight->Position.Y = 2;
		}
		settings->Internals.Header.TabGroupScrollRight->Caption.Text = "R";
		settings->Internals.Header.TabGroupScrollRight->Caption.WordWrap = false;
		//settings->Internals.Header.TabGroupScrollRight->Caption.Font = &g_sFontCm12;
		settings->Internals.Header.TabGroupScrollRight->Internals.NoPaintBackGround = true;
		settings->Internals.Header.TabGroupScrollRight->Internals.IsChildren = true;
		settings->Internals.Header.TabGroupScrollRight->Internals.NeedEntireRefresh = true;

		if(ParentWindow != NULL && settings != ParentWindow)
		{
			settings->Internals.H_ScrollBar->Position.X = (3 + settings->Internals.Position.X) - ParentWindow->Internals.Position.X;
			settings->Internals.H_ScrollBar->Position.Y = (settings->Internals.Size.Y + settings->Internals.Position.Y) - ParentWindow->Internals.Position.Y - settings->Internals.Size.ScrollBarSize - 1;
		}
		else
		{
			settings->Internals.H_ScrollBar->Position.X = 3;
			settings->Internals.H_ScrollBar->Position.Y = (settings->Internals.Size.Y) - settings->Internals.Size.ScrollBarSize - 2;
		}
		settings->Internals.H_ScrollBar->Internals.NeedEntireRefresh = true;
		settings->Internals.H_ScrollBar->Internals.NoPaintBackGround = true;

		if(ParentWindow != NULL && settings != ParentWindow)
		{
			settings->Internals.V_ScrollBar->Position.X = (settings->Internals.Size.X + settings->Internals.Position.X) - ParentWindow->Internals.Position.X - settings->Internals.Size.ScrollBarSize - 2;
			settings->Internals.V_ScrollBar->Position.Y = (HeaderSize + settings->Internals.Position.Y) - ParentWindow->Internals.Position.Y;
		}
		else
		{
			settings->Internals.V_ScrollBar->Position.X = (settings->Internals.Size.X) - settings->Internals.Size.ScrollBarSize - 2;
			settings->Internals.V_ScrollBar->Position.Y = HeaderSize;
		}
		settings->Internals.V_ScrollBar->Internals.NeedEntireRefresh = true;
		settings->Internals.V_ScrollBar->Internals.NoPaintBackGround = true;

		tRectangle back_up_clip = pDisplay->sClipRegion;
		pDisplay->sClipRegion.sXMin = X_StartBox;
		pDisplay->sClipRegion.sYMin = Y_StartBox;
		pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
		pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
		clip_limit(&pDisplay->sClipRegion, &back_up_clip);
		clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
		put_horizontal_line(pDisplay, X_StartBox, X_LenBox, Y_StartBox, 1, controlls_change_color(settings->WindowColor.Enabled.WindowBorder, +BORDER_LINE_TWO_DARK));
		put_horizontal_line(pDisplay, X_StartBox, X_LenBox, (Y_StartBox + Y_LenBox) - 1, 1, controlls_change_color(settings->WindowColor.Enabled.WindowBorder, -BORDER_LINE_TWO_DARK));

		put_vertical_line(pDisplay, Y_StartBox, Y_LenBox, X_StartBox, 1, controlls_change_color(settings->WindowColor.Enabled.WindowBorder, +BORDER_LINE_TWO_DARK));
		put_vertical_line(pDisplay, Y_StartBox, Y_LenBox, (X_StartBox + X_LenBox) - 1, 1, controlls_change_color(settings->WindowColor.Enabled.WindowBorder, -BORDER_LINE_TWO_DARK));

		//put_rectangle(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, false, settings->WindowColor.Enabled.WindowBorder);
		put_rectangle(pDisplay, X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, HeaderSize - 1, true, settings->WindowColor.Enabled.WindowHeader);
		put_rectangle(pDisplay, X_StartBox + 1, (Y_StartBox + HeaderSize) - 1, X_LenBox - 2, Y_LenBox - HeaderSize - 0, true, controls_color.Scren);
		box_cache_clean(pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox);


		CursorState cursor = control_comand->Cursor;
		control_comand->Cursor = Cursor_Up;
		//ControlCommands control_comand_comand = control_comand->Comand;
		ChildrenWindowSize_t ChildrenWindowSize;
		window_set_children_settings(settings, false, false, control_comand, false, &ChildrenWindowSize);
		settings->Internals.ChildrenWindowSize.X = ChildrenWindowSize.X;
		settings->Internals.ChildrenWindowSize.Y = ChildrenWindowSize.Y;

		settings->Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (settings->Size.X - 8 - settings->Internals.Size.ScrollBarSize);
		settings->Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (settings->Size.Y - 6 - settings->Internals.Size.ScrollBarSize - HeaderSize);
		if((settings->Internals.V_ScrollBar->Maximum > 0 && settings->HideVScroll == false/* || settings->ShowVScroll == true*/)/* && settings->AllowVScroll*/)
		{
			settings->Internals.V_ScrollBar->Size.X = settings->Internals.Size.ScrollBarSize;
		}
		else
		{
			settings->Internals.V_ScrollBar->Maximum = 0;
			settings->Internals.V_ScrollBar->Value = 0;
			settings->Internals.V_ScrollBar->Size.X = 0;
		}
		if((settings->Internals.H_ScrollBar->Maximum > 0 && settings->HideHScroll == false/* || settings->ShowHScroll == true*/)/* && settings->AllowHScroll*/)
		{
			settings->Internals.H_ScrollBar->Size.Y = settings->Internals.Size.ScrollBarSize;
		}
		else
		{
			settings->Internals.H_ScrollBar->Maximum = 0;
			settings->Internals.H_ScrollBar->Value = 0;
			settings->Internals.H_ScrollBar->Size.Y = 0;
		}

		settings->Internals.V_ScrollBar->Size.Y = settings->Internals.Size.Y - 2 - settings->Internals.Size.ScrollBarSize - HeaderSize;
		settings->Internals.H_ScrollBar->Size.X = settings->Internals.Size.X - 5 - settings->Internals.Size.ScrollBarSize;


		control_comand->Cursor = Cursor_Up;
		window_set_children_settings(settings, true, true, control_comand, true, &ChildrenWindowSize);
		control_comand->Cursor = Cursor_Up;
		window_set_children_settings(settings, true, true, control_comand, true, &ChildrenWindowSize);
		if(settings->Internals.OldTabGroupMode == false)
		{
			if(settings->Internals.HideHeader == false) {
				button(settings->Internals.Header.Close, control_comand);
				checkbox(settings->Internals.Header.MaxMin, control_comand);
				button(settings->Internals.Header.Minimize, control_comand);
			}
		}
		else
		{
			button(settings->Internals.Header.TabGroupScrollLeft, control_comand);
			button(settings->Internals.Header.TabGroupScrollRight, control_comand);
		}
		scrollbar(settings->Internals.H_ScrollBar, control_comand);
		scrollbar(settings->Internals.V_ScrollBar, control_comand);

		if(settings->Internals.OldTabGroupMode)
		{
			tRectangle back_up_clip_header = pDisplay->sClipRegion;
			signed int _X_StartBox_, _Y_StartBox_, _X_LenBox_, _Y_LenBox_;;
			_X_StartBox_ = (settings->Internals.Position.X + header_btn_space + 2);
			_X_LenBox_ = (settings->Internals.Size.X + settings->Internals.Position.X) - header_btn_space - 2;
			_Y_StartBox_ = (2 + settings->Internals.Position.Y);
			_Y_LenBox_ = (2 + settings->Internals.Position.Y) + header_btn_size;
			pDisplay->sClipRegion.sXMin = _X_StartBox_;
			pDisplay->sClipRegion.sXMax = _X_LenBox_;
			pDisplay->sClipRegion.sYMin = _Y_StartBox_;
			pDisplay->sClipRegion.sYMax = _Y_LenBox_;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip_header);
			clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
			put_rectangle(pDisplay, _X_StartBox_, _Y_StartBox_, _X_LenBox_ - _X_StartBox_, _Y_LenBox_ - _Y_StartBox_, false, settings->WindowColor.Enabled.WindowBorder);
			pDisplay->sClipRegion.sXMin = _X_StartBox_ + 1;
			pDisplay->sClipRegion.sXMax = _X_LenBox_ - 1;
			pDisplay->sClipRegion.sYMin = _Y_StartBox_ + 1;
			pDisplay->sClipRegion.sYMax = _Y_LenBox_ - 1;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip_header);
			clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
			unsigned int TabGroupCnt = 0;
			while(TabGroupCnt < settings->Internals.Header.TabGroupTabsListNr && settings->Internals.Header.TabGroupTabsList[TabGroupCnt])
			{
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.X = ((_X_StartBox_ + 1 + (settings->Internals.Header.Size.TabGroupSelectorSize * TabGroupCnt) - settings->Internals.Header.TabGroupSelectorPosition) - ParentWindow->Internals.Position.X);
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.Y = (_Y_StartBox_ + 1) - ParentWindow->Internals.Position.Y;
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.X = settings->Internals.Header.Size.TabGroupSelectorSize;
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.Y = _Y_LenBox_ - _Y_StartBox_ - 2;
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Internals.NeedEntireRefresh = true;
				checkbox(settings->Internals.Header.TabGroupTabsList[TabGroupCnt], control_comand);
				TabGroupCnt++;
			}

			//pDisplay->sClipRegion = back_up_clip_header;
		}
		pDisplay->sClipRegion = back_up_clip;
		//control_comand->Comand = control_comand_comand;
		control_comand->Cursor = cursor;

		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.OldStateEnabled = settings->Enabled;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		//control_comand->WindowRefresh |= true;
		return;
	}

	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = X_StartBox;
	pDisplay->sClipRegion.sYMin = Y_StartBox;
	pDisplay->sClipRegion.sXMax = X_StartBox + X_LenBox;
	pDisplay->sClipRegion.sYMax = Y_StartBox + Y_LenBox;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
	if(settings->Internals.OldTabGroupMode == false)
	{
		if(settings->Internals.HideHeader == false) {
			button(settings->Internals.Header.Close, control_comand);
			checkbox(settings->Internals.Header.MaxMin, control_comand);
			button(settings->Internals.Header.Minimize, control_comand);
		}
	}
	else
	{
		button(settings->Internals.Header.TabGroupScrollLeft, control_comand);
		button(settings->Internals.Header.TabGroupScrollRight, control_comand);
		signed int HeaderSize = 0;
		if(settings->Internals.HideHeader == false) HeaderSize = settings->Internals.Header.Size.Y;
		unsigned int header_btn_size = HeaderSize - 6;
		unsigned int header_btn_space = HeaderSize - 5;
		bool TabGroupSelectorPositionHasChanged = false;
		if(settings->Internals.Header.TabGroupScrollRight->Events.CursorDown || settings->Internals.Header.TabGroupScrollRight->Events.CursorMove)
		{
			settings->Internals.Header.TabGroupScrollRight->Events.CursorDown = false;
			settings->Internals.Header.TabGroupScrollRight->Events.CursorMove = false;
			int len_of_selection_tab = settings->Internals.Header.Size.TabGroupSelectorSize * settings->Internals.Header.TabGroupTabsListNr;
			if(len_of_selection_tab > settings->Internals.Size.X - (header_btn_space * 2) - 2)
			{
				if(settings->Internals.Header.TabGroupSelectorPosition < len_of_selection_tab - (settings->Internals.Size.X - (header_btn_space * 2)) - 2)
				{
					settings->Internals.Header.TabGroupSelectorPosition++;
					TabGroupSelectorPositionHasChanged = true;
				}
			}
		}
		if(settings->Internals.Header.TabGroupScrollLeft->Events.CursorDown || settings->Internals.Header.TabGroupScrollLeft->Events.CursorMove)
		{
			settings->Internals.Header.TabGroupScrollLeft->Events.CursorDown = false;
			settings->Internals.Header.TabGroupScrollLeft->Events.CursorMove = false;
			int len_of_selection_tab = settings->Internals.Header.Size.TabGroupSelectorSize * settings->Internals.Header.TabGroupTabsListNr;
			if(len_of_selection_tab > settings->Internals.Size.X - header_btn_space - 2)
			{
				if(settings->Internals.Header.TabGroupSelectorPosition > 0)
				{
					settings->Internals.Header.TabGroupSelectorPosition--;
					TabGroupSelectorPositionHasChanged = true;
				}
			}
		}
		signed int _X_StartBox_, _Y_StartBox_, _X_LenBox_, _Y_LenBox_;;
		_X_StartBox_ = (settings->Internals.Position.X + header_btn_space + 2);
		_X_LenBox_ = (settings->Internals.Size.X + settings->Internals.Position.X) - header_btn_space - 2;
		_Y_StartBox_ = (2 + settings->Internals.Position.Y);
		_Y_LenBox_ = (2 + settings->Internals.Position.Y) + header_btn_size;
		tRectangle back_up_clip_header = pDisplay->sClipRegion;
		if(TabGroupSelectorPositionHasChanged || settings->Internals.Header.OldTabGroupTabsListNr != settings->Internals.Header.TabGroupTabsListNr)
		{
			pDisplay->sClipRegion.sXMin = _X_StartBox_;
			pDisplay->sClipRegion.sXMax = _X_LenBox_;
			pDisplay->sClipRegion.sYMin = _Y_StartBox_;
			pDisplay->sClipRegion.sYMax = _Y_LenBox_;
			//clip_limit(&pDisplay->sClipRegion, &back_up_clip_header);
			clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
			put_rectangle(pDisplay, _X_StartBox_, _Y_StartBox_, _X_LenBox_ - _X_StartBox_, _Y_LenBox_ - _Y_StartBox_, false, settings->WindowColor.Enabled.WindowBorder);
			pDisplay->sClipRegion.sXMin = _X_StartBox_ + 1;
			pDisplay->sClipRegion.sXMax = _X_LenBox_ - 1;
			pDisplay->sClipRegion.sYMin = _Y_StartBox_ + 1;
			pDisplay->sClipRegion.sYMax = _Y_LenBox_ - 1;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip_header);
			clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
			unsigned int TabGroupCnt = 0;
			CursorState cursor = control_comand->Cursor;
			control_comand->Cursor = Cursor_Up;
			while(TabGroupCnt < settings->Internals.Header.TabGroupTabsListNr && settings->Internals.Header.TabGroupTabsList[TabGroupCnt])
			{
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.X = ((_X_StartBox_ + 1 + (settings->Internals.Header.Size.TabGroupSelectorSize * TabGroupCnt) - settings->Internals.Header.TabGroupSelectorPosition) - ParentWindow->Internals.Position.X);
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Position.Y = (_Y_StartBox_ + 1) - ParentWindow->Internals.Position.Y;
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.X = settings->Internals.Header.Size.TabGroupSelectorSize;
				settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Size.Y = _Y_LenBox_ - _Y_StartBox_ - 2;
				if(settings->Internals.Header.OldTabGroupTabsListNr != settings->Internals.Header.TabGroupTabsListNr) settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Internals.NeedEntireRefresh = true;
				checkbox(settings->Internals.Header.TabGroupTabsList[TabGroupCnt], control_comand);
				TabGroupCnt++;
			}
			control_comand->Cursor = cursor;
			settings->Internals.Header.OldTabGroupTabsListNr = settings->Internals.Header.TabGroupTabsListNr;
			//pDisplay->sClipRegion = back_up_clip_header;
		}
		else
		{
			pDisplay->sClipRegion.sXMin = _X_StartBox_ + 1;
			pDisplay->sClipRegion.sXMax = _X_LenBox_ - 1;
			pDisplay->sClipRegion.sYMin = _Y_StartBox_ + 1;
			pDisplay->sClipRegion.sYMax = _Y_LenBox_ - 1;
			clip_limit(&pDisplay->sClipRegion, &back_up_clip_header);
			clip_limit(&pDisplay->sClipRegion, &settings->WindowMoveLimits);
			unsigned int TabGroupCnt = 0;
			while(TabGroupCnt < settings->Internals.Header.TabGroupTabsListNr && settings->Internals.Header.TabGroupTabsList[TabGroupCnt]) {
				checkbox(settings->Internals.Header.TabGroupTabsList[TabGroupCnt], control_comand);
				if(settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Events.Checked) {
					settings->Internals.Header.TabGroupTabsList[TabGroupCnt]->Events.Checked = false;
					//settings->Internals.TabChanged = true;
					settings->Internals.NeedEntireRefresh = true;
					settings->SelectedTab = TabGroupCnt;
					unsigned int TabGroupCntUncheck = 0;
					for(; TabGroupCntUncheck < settings->Internals.Header.TabGroupTabsListNr; TabGroupCntUncheck++) {
						if(TabGroupCntUncheck != TabGroupCnt) settings->Internals.Header.TabGroupTabsList[TabGroupCntUncheck]->Cheched = false;
					}
				}
				TabGroupCnt++;
			}
			pDisplay->sClipRegion = back_up_clip_header;
		}
	}

	bool full_screen_has_changed_state = false;
	if(settings->Internals.Header.MaxMin->Events.Checked)
	{
		settings->Internals.Header.MaxMin->Events.Checked = false;
		settings->Size.X = settings->SizeNormalScreen.X;
		settings->Size.Y = settings->SizeNormalScreen.Y;
		settings->Position.X = settings->Internals.NormalScreenPosition.X;
		settings->Position.Y = settings->Internals.NormalScreenPosition.Y;
		settings->Internals.FullScreen = false;
		settings->Internals.Position.ChildrenPosition_X = 0;
		settings->Internals.Position.ChildrenPosition_Y = 0;
		ParentWindow->Internals.Position.ChildrenPosition_X = 0;
		ParentWindow->Internals.Position.ChildrenPosition_Y = 0;
		//settings->Internals.H_ScrollBar->Value = 0;
		//settings->Internals.V_ScrollBar->Value = 0;
		full_screen_has_changed_state = true;
		ParentWindow->Internals.NeedEntireRefresh = true;
		//if(ParentWindow != NULL && settings != ParentWindow)
		//{
		//	ParentWindow->Internals.NeedEntireRefresh = true;
		//}
		//ChildrenWindowSize_t _ChildrenWindowSize;
		//window_set_children_settings(settings, true, true, control_comand, true, &_ChildrenWindowSize);
	}
	else if(settings->Internals.Header.MaxMin->Events.UnChecked)
	{
		settings->Internals.Header.MaxMin->Events.UnChecked = false;
		settings->Internals.NormalScreenPosition.X = settings->Position.X;
		settings->Internals.NormalScreenPosition.Y = settings->Position.Y;
		settings->Position.X = settings->Internals.WindowMoveLimits.sXMin;
		settings->Position.Y = settings->Internals.WindowMoveLimits.sYMin;
		settings->Internals.Position.ChildrenPosition_X = 0;
		settings->Internals.Position.ChildrenPosition_Y = 0;
		settings->Size.X = settings->WindowMoveLimits.sXMax - settings->Internals.WindowMoveLimits.sXMin;
		settings->Size.Y = settings->WindowMoveLimits.sYMax - settings->Internals.WindowMoveLimits.sYMin;
		settings->Internals.FullScreen = true;
		full_screen_has_changed_state = true;
	}

	scrollbar(settings->Internals.H_ScrollBar, control_comand);
	scrollbar(settings->Internals.V_ScrollBar, control_comand);
	ChildrenWindowSize_t ChildrenWindowSize;
	if(check_if_inside_box(X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down) settings->Internals.CursorDownInsideBox = true;
	bool _back_coordonate_used = control_comand->CursorCoordonateUsed;
	if(settings->Internals.FullScreen == false && control_comand->CursorCoordonateUsed == false)
	{
		int Resize_Position_X = (settings->Internals.Position.X + settings->Internals.Size.X) - (settings->Internals.Size.ScrollBarSize);
		int Resize_Position_Y = (settings->Internals.Position.Y + settings->Internals.Size.Y) - (settings->Internals.Size.ScrollBarSize);
		int Resize_Size_X = settings->Internals.Size.ScrollBarSize - 4;
		int Resize_Size_Y = settings->Internals.Size.ScrollBarSize - 4;

		if(check_if_inside_box(Resize_Position_X, Resize_Position_Y, Resize_Size_X, Resize_Size_Y, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down && control_comand->CursorCoordonateUsed == false) control_comand->CursorCoordonateUsed = true;
	}
	window_set_children_settings(settings, true, false, control_comand, false, &ChildrenWindowSize);
	control_comand->CursorCoordonateUsed = _back_coordonate_used;
	if(settings->Internals.ChildrenWindowSize.X != ChildrenWindowSize.X || settings->Internals.ChildrenWindowSize.Y != ChildrenWindowSize.Y)
	{
		signed int HeaderSize = 0;
		if(settings->Internals.HideHeader == false) HeaderSize = settings->Internals.Header.Size.Y;
		settings->Internals.ChildrenWindowSize.X = ChildrenWindowSize.X;
		settings->Internals.ChildrenWindowSize.Y = ChildrenWindowSize.Y;
		settings->Internals.H_ScrollBar->Maximum = ChildrenWindowSize.X - (settings->Size.X - 8 - settings->Internals.Size.ScrollBarSize);
		settings->Internals.V_ScrollBar->Maximum = ChildrenWindowSize.Y - (settings->Size.Y - 6 - settings->Internals.Size.ScrollBarSize - HeaderSize);
		if((settings->Internals.V_ScrollBar->Maximum > 0 && settings->HideVScroll == false/* || settings->ShowVScroll == true*/)/* && settings->AllowVScroll*/)
		{
			settings->Internals.V_ScrollBar->Size.X = settings->Internals.Size.ScrollBarSize;
		}
		else
		{
			settings->Internals.V_ScrollBar->Maximum = 0;
			settings->Internals.V_ScrollBar->Value = 0;
			settings->Internals.V_ScrollBar->Size.X = 0;
		}
		if((settings->Internals.H_ScrollBar->Maximum > 0 && settings->HideHScroll == false /*|| settings->ShowHScroll == true*/)/* && settings->AllowHScroll*/)
		{
			settings->Internals.H_ScrollBar->Size.Y = settings->Internals.Size.ScrollBarSize;
		}
		else
		{
			settings->Internals.H_ScrollBar->Maximum = 0;
			settings->Internals.H_ScrollBar->Value = 0;
			settings->Internals.H_ScrollBar->Size.Y = 0;
		}

		settings->Internals.V_ScrollBar->Size.Y = settings->Internals.Size.Y - 2 - settings->Internals.Size.ScrollBarSize - HeaderSize;
		settings->Internals.H_ScrollBar->Size.X = settings->Internals.Size.X - 5 - settings->Internals.Size.ScrollBarSize;


		settings->Internals.V_ScrollBar->Internals.NoPaintBackGround = false;
		scrollbar(settings->Internals.V_ScrollBar, control_comand);
		settings->Internals.V_ScrollBar->Internals.NoPaintBackGround = true;


		settings->Internals.H_ScrollBar->Internals.NoPaintBackGround = false;
		scrollbar(settings->Internals.H_ScrollBar, control_comand);
		settings->Internals.H_ScrollBar->Internals.NoPaintBackGround = true;
		if(settings->Internals.V_ScrollBar->Size.X == 0 && settings->Internals.V_ScrollBar->Value != 0)
		{
			settings->Internals.V_ScrollBar->Events.ValueChanged = true;
			settings->Internals.V_ScrollBar->Value = 0;
		}

		if(settings->Internals.H_ScrollBar->Size.Y == 0 && settings->Internals.H_ScrollBar->Value != 0)
		{
			settings->Internals.H_ScrollBar->Events.ValueChanged = true;
			settings->Internals.H_ScrollBar->Value = 0;
		}
	}
	if(settings->Internals.H_ScrollBar->Events.ValueChanged || settings->Internals.V_ScrollBar->Events.ValueChanged)
	{
		settings->Internals.H_ScrollBar->Events.ValueChanged = false;
		settings->Internals.V_ScrollBar->Events.ValueChanged = false;
		signed int HeaderSize = 0;
		if(settings->Internals.HideHeader == false) HeaderSize = settings->Internals.Header.Size.Y;
		put_rectangle(pDisplay, X_StartBox + 1, (Y_StartBox + HeaderSize) - 1, X_LenBox - 3 - settings->Internals.V_ScrollBar->Size.X, Y_LenBox - settings->Internals.H_ScrollBar->Size.Y - 1 - HeaderSize, true, /*controlls_change_color(*/controls_color.Scren/*, 1.2)*/);
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

	if(settings->Internals.FullScreen == false && control_comand->CursorCoordonateUsed == false && settings->Internals.OldTabGroupMode == false)
	{
		int Resize_Position_X = (settings->Internals.Position.X + settings->Internals.Size.X) - (settings->Internals.Size.ScrollBarSize);
		int Resize_Position_Y = (settings->Internals.Position.Y + settings->Internals.Size.Y) - (settings->Internals.Size.ScrollBarSize);
		int Resize_Size_X = settings->Internals.Size.ScrollBarSize - 4;
		int Resize_Size_Y = settings->Internals.Size.ScrollBarSize - 4;

		if(check_if_inside_box(Resize_Position_X, Resize_Position_Y, Resize_Size_X, Resize_Size_Y, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down && control_comand->CursorCoordonateUsed == false) settings->Internals.CursorDownOnResizeBtn = true;
		/* If is in sizeable window, from here begin to calculate the resize of window using touchscreen */
		if(settings->Internals.CursorDownOnResizeBtn == true)
		{
			if(control_comand->Cursor == Cursor_Down)
			{
				settings->Internals.WindowResizeTouchDownPointX = control_comand->X;
				settings->Internals.WindowResizeTouchDownPointY = control_comand->Y;
				settings->Internals.WindowResizeTouchDownSizeX = settings->Size.X;
				settings->Internals.WindowResizeTouchDownSizeY = settings->Size.Y;
				control_comand->CursorCoordonateUsed = true;
			}
			else if(control_comand->Cursor == Cursor_Move)
			{
				settings->Size.X = settings->Internals.WindowResizeTouchDownSizeX + (control_comand->X - settings->Internals.WindowResizeTouchDownPointX);
				settings->Size.Y = settings->Internals.WindowResizeTouchDownSizeY + (control_comand->Y - settings->Internals.WindowResizeTouchDownPointY);
				if(settings->Size.X < 160) settings->Size.X = 160;
				if(settings->Size.Y < 120) settings->Size.Y = 120;
				settings->SizeNormalScreen.X = settings->Size.X;
				settings->SizeNormalScreen.Y = settings->Size.Y;
				control_comand->CursorCoordonateUsed = true;
				if(ParentWindow != NULL && settings != ParentWindow)
				{
					ParentWindow->Internals.NeedEntireRefresh = true;
				}
			}
		}

		signed int HeaderSize = 0;
		if(settings->Internals.HideHeader == false) HeaderSize = settings->Internals.Header.Size.Y;
		if(check_if_inside_box(X_StartBox + 1, Y_StartBox + 1, X_LenBox - 2, HeaderSize - 2, control_comand->X, control_comand->Y) && control_comand->Cursor == Cursor_Down && control_comand->CursorCoordonateUsed == false) settings->Internals.CursorDownOnHeader = true;
		/* If is in sizeable window, from here begin to calculate the move of window using touchscreen */

		if(settings->Internals.CursorDownOnHeader == true)
		{
			if(control_comand->Cursor == Cursor_Down)
			{
				settings->Internals.CursorDownOnHeader = true;
				settings->Internals.HeaderTouchDownPointX = control_comand->X;
				settings->Internals.HeaderTouchDownPointY = control_comand->Y;
				settings->Internals.WindowTouchDownPointX = settings->Position.X;
				settings->Internals.WindowTouchDownPointY = settings->Position.Y;
				control_comand->CursorCoordonateUsed = true;
			}
			else if(control_comand->Cursor == Cursor_Move)
			{
				settings->Position.X = settings->Internals.WindowTouchDownPointX + (control_comand->X - settings->Internals.HeaderTouchDownPointX);
				settings->Position.Y = settings->Internals.WindowTouchDownPointY + (control_comand->Y - settings->Internals.HeaderTouchDownPointY);
				if(settings->Position.Y < settings->WindowMoveLimits.sYMin) settings->Position.Y = settings->WindowMoveLimits.sYMin;
				if(settings->Position.Y + HeaderSize >= settings->WindowMoveLimits.sYMax) settings->Position.Y = settings->WindowMoveLimits.sYMax - HeaderSize;
				control_comand->CursorCoordonateUsed = true;
				if(ParentWindow != NULL && settings != ParentWindow)
				{
					ParentWindow->Internals.NeedEntireRefresh = true;
				}
			}
		}
	}
	if(control_comand->Cursor == Cursor_Up || control_comand->Cursor == Cursor_NoAction)
	{
		settings->Internals.CursorDownOnHeader = false;
		settings->Internals.CursorDownOnResizeBtn = false;
		settings->Internals.CursorDownInsideBox = false;
	}
	if(control_comand->Cursor != Cursor_NoAction && settings->Internals.CursorDownInsideBox == true) control_comand->CursorCoordonateUsed |= true;
}
//#######################################################################################
tWindow *new_window_tab_group(void *ParentWindow, tDisplay *ScreenDisplay)
{
	tWindow *_ParentWindow = (tWindow *)ParentWindow;
	struct Window_s* settings = (struct Window_s*)calloc(1, sizeof(struct Window_s));
	tDisplay *_ScreenDisplay = ScreenDisplay;
	if(!settings || (!_ScreenDisplay && !ParentWindow)) return NULL;
	if(ParentWindow) _ScreenDisplay = _ParentWindow->Internals.pDisplay;

	/*settings->Internals.Childrens = (window_children_t **)calloc(1, sizeof(settings->Internals.Childrens));
	if(!settings->Internals.Childrens)
	{
		if(settings) free(settings);
		return NULL;
	}*/
	if(ParentWindow) settings->Internals.ParentWindow = ParentWindow;
	else settings->Internals.ParentWindow = settings;
	settings->Internals.pDisplay = _ScreenDisplay;
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
	settings->WindowColor.Enabled.WindowBorder = controls_color.Control_Color_Enabled_WindowBorder;
	settings->WindowColor.Enabled.WindowHeader = controls_color.Control_Color_Enabled_WindowHeader;

	settings->Color.Disabled.BackGround = controls_color.Control_Color_Disabled_BackGround;
	settings->Color.Disabled.Border = controls_color.Control_Color_Disabled_Border_Pull;
	settings->Color.Disabled.Buton = controls_color.Control_Color_Disabled_Buton_Pull;
	settings->Color.Disabled.Ink = controls_color.Control_Color_Disabled_Ink_Pull;
	settings->WindowColor.Disabled.WindowBorder = controls_color.Control_Color_Disabled_WindowBorder;
	settings->WindowColor.Disabled.WindowHeader = controls_color.Control_Color_Disabled_WindowHeader;

	//settings->Position.X = settings->PositionFullScreen.X;
	//settings->Position.Y = settings->PositionFullScreen.Y;
	settings->Size.X = _ScreenDisplay->raster_timings->X;
	settings->Size.Y = _ScreenDisplay->raster_timings->Y;
	settings->Size.ScrollBarSize = 20;
	//settings->SizeFullScreen.X = ScreenDisplay->Width;
	//settings->SizeFullScreen.Y = ScreenDisplay->Height;
	settings->SizeNormalScreen.X = 200;
	settings->SizeNormalScreen.Y = 150;
	settings->StateChangedOn = Cursor_Up;
	settings->Visible = true;
	settings->Enabled = true;

	if(ParentWindow)
	{
		settings->WindowMoveLimits = _ParentWindow->WindowMoveLimits;
	}
	else
	{
		settings->WindowMoveLimits.sXMin = 0;
		settings->WindowMoveLimits.sXMax = _ScreenDisplay->raster_timings->X;
		settings->WindowMoveLimits.sYMin = 0;
		settings->WindowMoveLimits.sYMax = _ScreenDisplay->raster_timings->Y;
	}
	settings->Internals.Header.Size.Y = 20;
	settings->Internals.FullScreen = true;

	settings->idle = window;
	settings->add_children = window_add_children;
	settings->get_children_address = window_get_children_address;
	settings->get_children_index = window_get_children_index;
	settings->set_list_of_childrens = window_set_list_of_childrens;

	settings->Internals.Header.Size.TabGroupSelectorSize = 70;
	return settings;
}
//#######################################################################################
tWindow *new_window(void *ParentWindow, tDisplay *ScreenDisplay)
{
	return new_window_tab_group(ParentWindow, ScreenDisplay);
}
//#######################################################################################
tWindow *new_tab_group(void *ParentWindow, tDisplay *ScreenDisplay)
{
	return new_window_tab_group(ParentWindow, ScreenDisplay);
}
//#######################################################################################
void* window_tabgroup_add_children(struct Window_s *settings, unsigned int children_type, char *children_name, unsigned int tab_nr)
{
	if(!settings) return NULL;
	if(tab_nr > settings->Internals.Header.TabGroupTabsListNr) return NULL;
	void *children_addr = NULL;
	switch(children_type)
	{
	case WindowButtonChildren:
		children_addr = (void *)new_button(settings);
		break;
	case WindowCheckboxChildren:
		children_addr = (void *)new_checkbox(settings);
		break;
	case WindowListboxChildren:
		children_addr = (void *)new_listbox(settings);
		break;
	case WindowProgressbarChildren:
		children_addr = (void *)new_progressbar(settings);
		break;
	case WindowScrollbarChildren:
		children_addr = (void *)new_scrollbar(settings);
		break;
	case WindowTextboxChildren:
		children_addr = (void *)new_textbox(settings);
		break;
	case WindowPictureboxChildren:
		children_addr = (void *)new_picturebox(settings);
		break;
	case WindowKeyboardChildren:
		children_addr = (void *)new_v_keyboard(settings);
		break;
	case WindowWindowChildren:
		children_addr = (void *)new_window(settings, NULL);
		break;
	case WindowTabGroupChildren:
		children_addr = (void *)new_tab_group(settings, NULL);
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
	settings->Internals.Childrens[added_children]->TabLocNr = tab_nr;
	switch(children_type)
	{
	case WindowButtonChildren:
		((tButton *)children_addr)->Position.X = /*settings->Position.X*/ 0;
		((tButton *)children_addr)->Position.Y = /*settings->Position.Y*/ 0;
		((tButton *)children_addr)->Size.X = 40;
		((tButton *)children_addr)->Size.Y = 20;
		((tButton *)children_addr)->StateChangedOn = settings->StateChangedOn;
		((tButton *)children_addr)->Caption.Font = settings->Caption.Font;
		((tButton *)children_addr)->Caption.TextAlign = settings->Caption.TextAlign;
		((tButton *)children_addr)->Caption.WordWrap = settings->Caption.WordWrap;
		((tButton *)children_addr)->Enabled = settings->Enabled;
		((tButton *)children_addr)->Visible = settings->Visible;
		((tButton *)children_addr)->Color = settings->Color;
		((tButton *)children_addr)->Caption.Text = malloc(strlen(children_name) + 1);
		strcpy(((tButton *)children_addr)->Caption.Text, children_name);
		((tButton *)children_addr)->Internals.ParentWindow = (void*)settings;
		break;
	case WindowCheckboxChildren:
		((tCheckBox *)children_addr)->Position.X = /*settings->Position.X*/ 0 + 45;
		((tCheckBox *)children_addr)->Position.Y = /*settings->Position.Y*/ 0;
		((tCheckBox *)children_addr)->Size.X = 60;
		((tCheckBox *)children_addr)->Size.Y = 20;
		((tCheckBox *)children_addr)->StateChangedOn = settings->StateChangedOn;
		((tCheckBox *)children_addr)->Caption.Font = settings->Caption.Font;
		((tCheckBox *)children_addr)->Caption.TextAlign = settings->Caption.TextAlign;
		((tCheckBox *)children_addr)->Caption.WordWrap = settings->Caption.WordWrap;
		((tCheckBox *)children_addr)->Enabled = settings->Enabled;
		((tCheckBox *)children_addr)->Visible = settings->Visible;
		((tCheckBox *)children_addr)->Color = settings->Color;
		((tCheckBox *)children_addr)->Caption.Text = malloc(strlen(children_name) + 1);
		strcpy(((tCheckBox *)children_addr)->Caption.Text, children_name);
		((tCheckBox *)children_addr)->Internals.ParentWindow = (void*)settings;
		break;
	case WindowListboxChildren:
		((tListBox *)children_addr)->Position.X = /*settings->Position.X*/ 0 + 110;
		((tListBox *)children_addr)->Position.Y = /*settings->Position.Y*/ 0;
		((tListBox *)children_addr)->Size.X = 200;
		((tListBox *)children_addr)->Size.Y = 70;
		((tListBox *)children_addr)->StateChangedOn = settings->StateChangedOn;
		((tListBox *)children_addr)->Caption.Font = settings->Caption.Font;
		((tListBox *)children_addr)->Caption.TextAlign = Align_Left;//settings->Caption.TextAlign;
		((tListBox *)children_addr)->Caption.WordWrap = settings->Caption.WordWrap;
		((tListBox *)children_addr)->Enabled = settings->Enabled;
		((tListBox *)children_addr)->Visible = settings->Visible;
		((tListBox *)children_addr)->Color = settings->Color;
		((tListBox *)children_addr)->Caption.Text = malloc(strlen(children_name) + 1);
		strcpy(((tListBox *)children_addr)->Caption.Text, children_name);
		((tListBox *)children_addr)->Internals.ParentWindow = (void*)settings;
		break;
	case WindowProgressbarChildren:
		((tProgressBar *)children_addr)->Position.X = /*settings->Position.X*/ 0;
		((tProgressBar *)children_addr)->Position.Y = /*settings->Position.Y*/ 0 + 25;
		((tProgressBar *)children_addr)->Size.X = 105;
		((tProgressBar *)children_addr)->Size.Y = 20;
		((tProgressBar *)children_addr)->StateChangedOn = settings->StateChangedOn;
		((tProgressBar *)children_addr)->Caption.Font = settings->Caption.Font;
		((tProgressBar *)children_addr)->Caption.TextAlign = settings->Caption.TextAlign;
		((tProgressBar *)children_addr)->Caption.WordWrap = settings->Caption.WordWrap;
		((tProgressBar *)children_addr)->Enabled = settings->Enabled;
		((tProgressBar *)children_addr)->Visible = settings->Visible;
		((tProgressBar *)children_addr)->Color = settings->Color;
		((tProgressBar *)children_addr)->Caption.Text = malloc(strlen(children_name) + 1);
		strcpy(((tProgressBar *)children_addr)->Caption.Text, children_name);
		((tProgressBar *)children_addr)->Internals.ParentWindow = (void*)settings;
		break;
	case WindowScrollbarChildren:
		((tScrollBar *)children_addr)->Position.X = /*settings->Position.X*/ 0;
		((tScrollBar *)children_addr)->Position.Y = /*settings->Position.Y*/ 0 + 50;
		((tScrollBar *)children_addr)->Size.X = 105;
		((tScrollBar *)children_addr)->Size.Y = 20;
		((tScrollBar *)children_addr)->StateChangedOn = Cursor_Move;
		((tScrollBar *)children_addr)->Enabled = settings->Enabled;
		((tScrollBar *)children_addr)->Visible = settings->Visible;
		((tScrollBar *)children_addr)->Color = settings->Color;
		((tScrollBar *)children_addr)->Internals.ParentWindow = (void*)settings;
		break;
	case WindowTextboxChildren:
		((tTextBox *)children_addr)->Position.X = /*settings->Position.X*/ 0;
		((tTextBox *)children_addr)->Position.Y = /*settings->Position.Y*/ 0 + 75;
		((tTextBox *)children_addr)->Size.X = 310;
		((tTextBox *)children_addr)->Size.Y = 160;
		((tTextBox *)children_addr)->StateChangedOn = settings->StateChangedOn;
		//((tTextBox *)children_addr)->Caption.Text = "Multiplatform SDK to create standalone applications\n\r1\n\r2\n\r3\n\r4\n\r5\n\r6\n\r7\n\r8\n\r9\n\r10\n\r11\n\r12\n\r13\n\r14\n\r15\n\r16\n\r17\n\r18";
		((tTextBox *)children_addr)->Font = settings->Caption.Font;
		((tTextBox *)children_addr)->TextAlign = settings->Caption.TextAlign;
		((tTextBox *)children_addr)->WordWrap = settings->Caption.WordWrap;
		((tTextBox *)children_addr)->Enabled = settings->Enabled;
		((tTextBox *)children_addr)->Visible = settings->Visible;
		((tTextBox *)children_addr)->Color = settings->Color;
		((tTextBox *)children_addr)->Text = malloc(strlen(children_name) + 1);
		strcpy(((tTextBox *)children_addr)->Text, children_name);
		((tTextBox *)children_addr)->Internals.ParentWindow = (void*)settings;
		break;
	case WindowPictureboxChildren:
		((tPictureBox *)children_addr)->Position.X = /*settings->Position.X*/ 0 + 320;
		((tPictureBox *)children_addr)->Position.Y = /*settings->Position.Y*/ 0;
		((tPictureBox *)children_addr)->Size.X = 128;
		((tPictureBox *)children_addr)->Size.Y = 96;
		((tPictureBox *)children_addr)->Enabled = settings->Enabled;
		((tPictureBox *)children_addr)->Visible = settings->Visible;
		((tPictureBox *)children_addr)->Color = settings->Color;
		break;
	case WindowKeyboardChildren:
		((tVKbd_Qwerty *)children_addr)->Position.X = 0;
		((tVKbd_Qwerty *)children_addr)->Position.Y = 0;
		((tVKbd_Qwerty *)children_addr)->Size.X = 320;
		((tVKbd_Qwerty *)children_addr)->Size.Y = 70;
		((tVKbd_Qwerty *)children_addr)->Enabled = settings->Enabled;
		((tVKbd_Qwerty *)children_addr)->Visible = settings->Visible;
		((tVKbd_Qwerty *)children_addr)->Color = settings->Color;
		break;
	case WindowWindowChildren:
		((tWindow *)children_addr)->StateChangedOn = settings->StateChangedOn;
		((tWindow *)children_addr)->Caption.Font = settings->Caption.Font;
		((tWindow *)children_addr)->Caption.TextAlign = settings->Caption.TextAlign;
		((tWindow *)children_addr)->Caption.WordWrap = settings->Caption.WordWrap;
		((tWindow *)children_addr)->Enabled = settings->Enabled;
		((tWindow *)children_addr)->Visible = settings->Visible;
		((tWindow *)children_addr)->Color = settings->Color;
		((tWindow *)children_addr)->Caption.Text = malloc(strlen(children_name) + 1);
		strcpy(((tWindow *)children_addr)->Caption.Text, children_name);
		((tWindow *)children_addr)->Internals.ParentWindow = (void*)settings;
		break;
	case WindowTabGroupChildren:
		((tWindow *)children_addr)->StateChangedOn = settings->StateChangedOn;
		((tWindow *)children_addr)->Caption.Font = settings->Caption.Font;
		((tWindow *)children_addr)->Caption.TextAlign = settings->Caption.TextAlign;
		((tWindow *)children_addr)->Caption.WordWrap = settings->Caption.WordWrap;
		((tWindow *)children_addr)->Enabled = settings->Enabled;
		((tWindow *)children_addr)->Visible = settings->Visible;
		((tWindow *)children_addr)->Color = settings->Color;
		((tWindow *)children_addr)->Caption.Text = malloc(strlen(children_name) + 1);
		strcpy(((tWindow *)children_addr)->Caption.Text, children_name);
		((tWindow *)children_addr)->Internals.ParentWindow = (void*)settings;
		((tWindow *)children_addr)->Internals.FullScreen = false;
		((tWindow *)children_addr)->Internals.TabGroupMode = true;
		((tWindow *)children_addr)->Internals.Header.Size.Y = 25;
		break;
	}
	settings->Internals.ChildrensNr++;
	ChildrenWindowSize_t ChildrenWindowSize;
	window_set_children_settings(settings, false, true, NULL, false, &ChildrenWindowSize);
	return children_addr;

window_can_not_add_children:
	switch(children_type)
	{
	case WindowButtonChildren:
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
	case WindowPictureboxChildren:
		free_picturebox(children_addr);
		break;
	case WindowWindowChildren:
		//free_window(children_addr);
		break;
	}
	return NULL;
}
//#######################################################################################
void* window_add_children(struct Window_s *settings, unsigned int children_type, char *children_name)
{
	return window_tabgroup_add_children(settings, children_type, children_name, 0);
}
//#######################################################################################
void* tab_group_add_children(struct Window_s *settings, unsigned int children_type, char *children_name, unsigned int tab_nr)
{
	return window_tabgroup_add_children(settings, children_type, children_name, tab_nr);
}
//#######################################################################################
signed int tab_group_new_tab(struct Window_s *settings, char *tab_name)
{
	if(settings->Internals.TabGroupMode)
	{
		if(settings->Internals.Header.TabGroupTabsList)
		{
			settings->Internals.Header.TabGroupTabsList = (tCheckBox **)realloc(settings->Internals.Header.TabGroupTabsList, (settings->Internals.Header.TabGroupTabsListNr + 1) * sizeof(tCheckBox *));
			if(!settings->Internals.Header.TabGroupTabsList) return -1;
		}
		else
		{
			settings->Internals.Header.TabGroupTabsList = (tCheckBox **)calloc(1, sizeof(tCheckBox *));
			if(!settings->Internals.Header.TabGroupTabsList) return - 1;
		}

		settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr] = new_checkbox(settings->Internals.ParentWindow);
		if(!settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]) return -1;
		if(!settings->Internals.Header.TabGroupTabsListNr) settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Cheched = true;
		settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Caption.Font = (tFont *)controls_color.DefaultFont;
		settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Caption.Text = malloc(strlen(tab_name) + 1);
		settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Caption.WordWrap = true;
		settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Caption.TextAlign = Align_Center;
		settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Internals.NoPaintBackGround = true;
		strcpy(settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Caption.Text, tab_name);
		//settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Color.Scren = controls_color.Control_Color_Enabled_Buton_Pull;
		//settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Color.Enabled.Buton.Push = controls_color.Control_Color_Enabled_Buton_Push;
		//settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Color.Enabled.Buton.Pull = controls_color.Control_Color_Enabled_Buton_Pull;
		settings->Internals.Header.TabGroupTabsList[settings->Internals.Header.TabGroupTabsListNr]->Style = checkbox_style_radio_btn;
		settings->Internals.Header.TabGroupTabsListNr++;
	}
	return settings->Internals.Header.TabGroupTabsListNr - 1;
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
