/*
 *  lib/gfx/keyboard.c
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
//#######################################################################################
#include <stdbool.h>
#include <stdlib.h>
#include "button.h"
#include "keyboard.h"
#include "keyboard_def.h"
#include "window_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/timer_api.h"
#include "graphic_string.h"
#include "controls_definition.h"
//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char kbd_qwerty_keys_little_return[38]  PROGMEM =
#else
static const unsigned int kbd_qwerty_keys_little_return[38] =
#endif
{
		kbd_qwerty_ctrl,'q','w','e','r','t','y','u','i' ,'o','p'            ,','          ,8               ,
		'#'           ,'a','s','d','f','g','h','j','k' ,'l','.'            ,kbd_qwerty_up,13              ,
		0            ,'z','x','c','v','b','n','m',32      ,kbd_qwerty_left,kbd_qwerty_dn,kbd_qwerty_right
};
//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char kbd_qwerty_keys_big_return[38]  PROGMEM =
#else
static const unsigned int kbd_qwerty_keys_big_return[38] =
#endif
{
		kbd_qwerty_ctrl,'Q','W','E','R','T','Y','U','I','O','P'            ,';'          ,8               ,
		'#'           ,'A','S','D','F','G','H','J','K','L',':'            ,kbd_qwerty_up,13              ,
		0              ,'Z','X','C','V','B','N','M',32     ,kbd_qwerty_left,kbd_qwerty_dn,kbd_qwerty_right
};
//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char kbd_qwerty_keys_numeric_return[38]  PROGMEM =
#else
static const unsigned int kbd_qwerty_keys_numeric_return[38] =
#endif
{
		kbd_qwerty_ctrl,'1','2','3','4','5','6','7','8','9','0'            ,'='          ,8               ,
		'#'           ,'*','+','#','-','_','(',')','&','!','?'            ,kbd_qwerty_up,13              ,
		0              ,0  ,'&',0  ,47 ,92 ,34 ,96 ,32     ,kbd_qwerty_left,kbd_qwerty_dn,kbd_qwerty_right
};
//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char kbd_qwerty_keys_little[][6]  PROGMEM =
#else
static const unsigned char kbd_qwerty_keys_little[][6] =
#endif
{
		"Ctrl","q","w","e","r","t","y","u","i" ,"o","p"  ,"," ,"BkS",
		"Nr"  ,"a","s","d","f","g","h","j","k" ,"l","."  ,"Up","Ent",
		"^"   ,"z","x","c","v","b","n","m","space" ,"Lef","Dn","Rig"
};
//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char kbd_qwerty_keys_big[][6]  PROGMEM =
#else
static const unsigned char kbd_qwerty_keys_big[][6] =
#endif
{
		"Ctrl","Q","W","E","R","T","Y","U","I" ,"O","P"  ,";" ,"BkS",
		"Nr"  ,"A","S","D","F","G","H","J","K" ,"L",":"  ,"Up","Ent",
		"^"   ,"Z","X","C","V","B","N","M","space" ,"Lef","Dn","Rig"
};
//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char kbd_qwerty_keys_numeric[][6]  PROGMEM =
#else
static const unsigned char kbd_qwerty_keys_numeric[][6] =
#endif
{
		"Ctrl","1","2","3","4","5","6","7","8" ,"9"     ,"0"  ,"=" ,"BkS",
		"Nr"  ,"*","+","#","-","_","(",")","&" ,"!"     ,"?"  ,"Up","Ent",
		"^"   ," ","&"," ","/","sl","quot","'" ,"space" ,"Lef","Dn","Rig"
};
//#######################################################################################
static void paint_v_keyboard(tVKbd_Qwerty* settings, tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand, bool refrash)
{
	unsigned int color = 0;
	tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	tRectangle back_up_clip = pDisplay->sClipRegion;
	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + x_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);

	if(refrash)
	{
		color = controls_color.Control_Color_Enabled_Border_Pull;
		put_rectangle(pDisplay, x_start, y_start, x_len, y_len, false, controlls_change_color(color, -3));
		put_rectangle(pDisplay, x_start + 1, y_start + 1, x_len - 2, y_len - 2, false, controlls_change_color(color, -2));
		color = controls_color.Control_Color_Enabled_Buton_Pull;
		put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, color);
		signed int key_size_x = ((settings->Size.X - 4 - (settings->kbd_border_size << 1))/13) - settings->key_space_size;
		signed int key_size_y = ((settings->Size.Y - 4 - (settings->kbd_border_size << 1))/3) - settings->key_space_size;

		signed int KeyboardLocationX = ((/*settings->Internals.Position.X + */settings->kbd_border_size) + ((settings->Internals.Size.X>>1) - settings->kbd_border_size)) - (((key_size_x + settings->key_space_size) * 13)>>1);
		signed int KeyLocationY =      ((/*settings->Internals.Position.Y + */settings->kbd_border_size) + ((settings->Internals.Size.Y>>1) - settings->kbd_border_size)) - (((key_size_y + settings->key_space_size) *  3)>>1);
		signed int KeyLocationX = KeyboardLocationX + 2;

		signed int CntInitKeys = 0;
		for(; CntInitKeys < sizeof(settings->Internals.Keys)/sizeof(settings->Internals.Keys[0]); CntInitKeys++)
		{
			tButton* KeyTmpPtr =  settings->Internals.Keys[CntInitKeys];
			if(KeyTmpPtr == 0) break;
			switch (CntInitKeys)
			{
				case 0:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX = KeyboardLocationX;
				//KeyLocationY += key_size_y + settings->key_space_size;
				break;
				case 13:
				case 26:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX = KeyboardLocationX;
				KeyLocationY += key_size_y + settings->key_space_size;
				break;
				//case 25:
				//KeyTmpPtr->Size.X = (key_size_x << 1) + settings->key_space_size;
				//KeyTmpPtr->Size.Y = key_size_y;
				//KeyLocationX += key_size_x + settings->key_space_size;
				//break;
				case 34:
				KeyLocationX += key_size_x + settings->key_space_size;
				KeyTmpPtr->Size.X = (key_size_x << 1) + settings->key_space_size;
				KeyTmpPtr->Size.Y = key_size_y;
				break;
				case 35:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX += ((key_size_x << 1) + (settings->key_space_size << 1));
				break;
				default:
				KeyTmpPtr->Size.X = key_size_x;
				KeyTmpPtr->Size.Y = key_size_y;
				KeyLocationX += key_size_x + settings->key_space_size;
			}
			KeyTmpPtr->Internals.PositionOffset.X = x_start - ParentWindow->Internals.Position.X;
			KeyTmpPtr->Internals.PositionOffset.Y = y_start - ParentWindow->Internals.Position.Y;
			KeyTmpPtr->Position.X = KeyLocationX;
			KeyTmpPtr->Position.Y = KeyLocationY;
			KeyTmpPtr->Caption.Font = settings->Caption.Font;
			KeyTmpPtr->Internals.NoPaintBackGround = true;
			KeyTmpPtr->Visible = settings->Visible;
			button(KeyTmpPtr, control_comand);
		}
		control_comand->WindowRefresh |= true;
	}
	else
	{
		signed int CntInitKeys = 0;
		for(; CntInitKeys < sizeof(settings->Internals.Keys)/sizeof(settings->Internals.Keys[0]); CntInitKeys++)
		{
			tButton* KeyTmpPtr =  settings->Internals.Keys[CntInitKeys];
			KeyTmpPtr->Visible = settings->Visible;
			button(KeyTmpPtr, control_comand);
			if(CntInitKeys == 13 && KeyTmpPtr->Events.CursorUp == true)
			{
				KeyTmpPtr->Events.CursorUp = false;
				settings->Internals.keboard_type = Kbd_Qwerty_Numeric;
			}
			else if(CntInitKeys == 26 && KeyTmpPtr->Events.CursorUp == true)
			{
				KeyTmpPtr->Events.CursorUp = false;
				if(settings->Internals.keboard_type == Kbd_Qwerty_Numeric) settings->Internals.keboard_type = Kbd_Qwerty_LittleLetters;
				else if(settings->Internals.keboard_type == Kbd_Qwerty_BigLetters) settings->Internals.keboard_type = Kbd_Qwerty_LittleLetters;
				else if(settings->Internals.keboard_type == Kbd_Qwerty_LittleLetters) settings->Internals.keboard_type = Kbd_Qwerty_BigLetters;
			}
			else if(KeyTmpPtr->Events.CursorUp == true)
			{

				KeyTmpPtr->Events.CursorUp = false;
				if(settings->Internals.keboard_type == Kbd_Qwerty_Numeric) settings->Return = (unsigned int)kbd_qwerty_keys_numeric_return[CntInitKeys];
				else if(settings->Internals.keboard_type == Kbd_Qwerty_BigLetters) settings->Return = (unsigned int)kbd_qwerty_keys_big_return[CntInitKeys];
				else if(settings->Internals.keboard_type == Kbd_Qwerty_LittleLetters) settings->Return = (unsigned int)kbd_qwerty_keys_little_return[CntInitKeys];
				settings->Events.CursorUp = true;
				if(settings->Events.OnUp.CallBack) settings->Events.OnUp.CallbackReturnData = settings->Events.OnUp.CallBack(settings->Events.OnUp.CallbackData, settings->Return);
			}
			else if(KeyTmpPtr->Events.CursorDown == true)
			{
				KeyTmpPtr->Events.CursorDown = false;
				if(settings->Internals.keboard_type == Kbd_Qwerty_Numeric) settings->Return = (unsigned int)kbd_qwerty_keys_numeric_return[CntInitKeys];
				else if(settings->Internals.keboard_type == Kbd_Qwerty_BigLetters) settings->Return = (unsigned int)kbd_qwerty_keys_big_return[CntInitKeys];
				else if(settings->Internals.keboard_type == Kbd_Qwerty_LittleLetters) settings->Return = (unsigned int)kbd_qwerty_keys_little_return[CntInitKeys];
				settings->Events.CursorDown = true;
				if(settings->Events.OnDown.CallBack) settings->Events.OnDown.CallbackReturnData = settings->Events.OnDown.CallBack(settings->Events.OnDown.CallbackData, settings->Return);
			}
			else if(KeyTmpPtr->Events.CursorMove == true)
			{
				KeyTmpPtr->Events.CursorMove = false;
				if(settings->Internals.keboard_type == Kbd_Qwerty_Numeric) settings->Return = (unsigned int)kbd_qwerty_keys_numeric_return[CntInitKeys];
				else if(settings->Internals.keboard_type == Kbd_Qwerty_BigLetters) settings->Return = (unsigned int)kbd_qwerty_keys_big_return[CntInitKeys];
				else if(settings->Internals.keboard_type == Kbd_Qwerty_LittleLetters) settings->Return = (unsigned int)kbd_qwerty_keys_little_return[CntInitKeys];
				settings->Events.CursorMove = true;
				if(settings->Events.OnMove.CallBack) settings->Events.OnMove.CallbackReturnData = settings->Events.OnMove.CallBack(settings->Events.OnMove.CallbackData, settings->Return);
			}
		}
	}

	pDisplay->sClipRegion.sXMin = x_start;
	pDisplay->sClipRegion.sYMin = y_start;
	pDisplay->sClipRegion.sXMax = x_start + x_len;
	pDisplay->sClipRegion.sYMax = y_start + y_len;
	clip_limit(&pDisplay->sClipRegion, &back_up_clip);
	box_cache_clean(pDisplay, x_start, y_start, x_len, y_len);
	pDisplay->sClipRegion = back_up_clip;
}
//#######################################################################################
void v_keyboard(tVKbd_Qwerty *settings, tControlCommandData* control_comand)
{
	if(settings == NULL) return;
	if(control_comand->Comand != Control_Nop)
	{
		/* Parse commands */
		switch((unsigned char)control_comand->Comand)
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
		settings->Internals.Position.X = settings->Position.X;
		settings->Internals.Position.Y = settings->Position.Y;
		settings->Internals.Size.X = settings->Size.X;
		settings->Internals.Size.Y = settings->Size.Y;
	}
	/* Verify if position on size has been modified */
	if(ParentWindow) {
		if(settings->Position.X + ParentWindow->Internals.Position.X + settings->Internals.PositionOffset.X != settings->Internals.Position.X ||
				settings->Position.Y + ParentWindow->Internals.Position.Y + settings->Internals.PositionOffset.Y != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.keboard_type != settings->Internals.old_keboard_type)
			settings->Internals.NeedEntireRefresh = true;
	}
	else {
		if(settings->Position.X != settings->Internals.Position.X ||
				settings->Position.Y != settings->Internals.Position.Y ||
					settings->Size.X != settings->Internals.Size.X ||
						settings->Size.Y != settings->Internals.Size.Y ||
							settings->Internals.keboard_type != settings->Internals.old_keboard_type)
			settings->Internals.NeedEntireRefresh = true;
	}

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
		settings->Internals.old_keboard_type = settings->Internals.keboard_type;
		X_StartBox = settings->Internals.Position.X;
		Y_StartBox = settings->Internals.Position.Y;
		X_LenBox = settings->Internals.Size.X;
		Y_LenBox = settings->Internals.Size.Y;

		unsigned char CntInitKeys = 0;
		for(; CntInitKeys < sizeof(settings->Internals.Keys)/sizeof(settings->Internals.Keys[0]); CntInitKeys++)
		{
			if(settings->Internals.keboard_type == Kbd_Qwerty_LittleLetters) settings->Internals.Keys[CntInitKeys]->Caption.Text = (char *)kbd_qwerty_keys_little[CntInitKeys];
			else if(settings->Internals.keboard_type == Kbd_Qwerty_BigLetters) settings->Internals.Keys[CntInitKeys]->Caption.Text = (char *)kbd_qwerty_keys_big[CntInitKeys];
			else if(settings->Internals.keboard_type == Kbd_Qwerty_Numeric) settings->Internals.Keys[CntInitKeys]->Caption.Text = (char *)kbd_qwerty_keys_numeric[CntInitKeys];
			settings->Internals.Keys[CntInitKeys]->Internals.NeedEntireRefresh = true;
		}
		//CursorState back = control_comand->Cursor;
		//control_comand->Cursor = Cursor_Up;
		paint_v_keyboard(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand, true);
		//control_comand->Cursor = back;
		settings->Internals.OldStateVisible = settings->Visible;
		settings->Internals.Control.Initiated = true;
		settings->Internals.NeedEntireRefresh = false;
		settings->Internals.NeedEntireRepaint = false;
		//control_comand->Cursor = cursor;
		//control_comand->WindowRefresh |= true;
		return;
	}
	paint_v_keyboard(settings, pDisplay, X_StartBox, Y_StartBox, X_LenBox, Y_LenBox, control_comand, false);
	if(control_comand->Cursor && settings->Internals.CursorDownInsideBox) control_comand->CursorCoordonateUsed |= true;
	//control_comand->CursorCoordonateUsed |= true;
	//control_comand->WindowRefresh |= true;
	return;
}
//#######################################################################################
tVKbd_Qwerty *new_v_keyboard(void *ParentWindow)
{
	tVKbd_Qwerty* settings = (tVKbd_Qwerty*)calloc(1, sizeof(tVKbd_Qwerty));

	if(!settings || !ParentWindow) return NULL;
	settings->Internals.ParentWindow = ParentWindow;

	tWindow *_ParentWindow = (tWindow *)ParentWindow;
	settings->Internals.pDisplay = _ParentWindow->Internals.pDisplay;

	settings->Caption.TextAlign = Align_Center;
	settings->Caption.WordWrap = true;
	settings->Caption.Font = controls_color.DefaultFont;

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
	settings->Visible = true;
	settings->key_space_size = 0;

	unsigned char CntInitKeys = 0;
	for(; CntInitKeys < sizeof(settings->Internals.Keys)/sizeof(settings->Internals.Keys[0]); CntInitKeys++)
	{
		settings->Internals.Keys[CntInitKeys] = new_button(settings->Internals.ParentWindow);
		settings->Internals.Keys[CntInitKeys]->Caption.WordWrap = false;
		if(settings->Internals.Keys[CntInitKeys]->Caption.Text) free(settings->Internals.Keys[CntInitKeys]->Caption.Text);
		settings->Internals.Keys[CntInitKeys]->Caption.Text = (char *)kbd_qwerty_keys_little[CntInitKeys];
	}
	return settings;
}
//#######################################################################################
bool free_v_keyboard(tVKbd_Qwerty* settings)
{
	if(!settings) return false;
	tControlCommandData comand;
	memset(&comand, 0x00, sizeof(tControlCommandData));

	settings->Visible = false;
	v_keyboard(settings, &comand);
	if(settings) free(settings);
	return true;
}
//#######################################################################################
