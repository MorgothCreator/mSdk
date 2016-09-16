/*
 *  lib/gfx/graphic_string.c
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
#include <string.h>
#include "graphic_string.h"
#include "../../api/lcd_def.h"
#include "../../api/lcd_api.h"
#include "../../api/timer_api.h"
#include "controls_definition.h"
#include "lib/string_lib.h"
//#######################################################################################
char *gfx_change_str(String_t *dest, String_t *src)
{
	/*if(src && (dest != src || strcmp(dest, src)))
	{
		if(dest) free(dest);
		char *dertination = (char*)malloc(strlen(src) + 1);
		strcpy(dertination, src);
		return dertination;
	}*/
	memcpy(dest, src, sizeof(String_t));
	return dest->text;
}
//#######################################################################################
bool _put_roll_string(tDisplay *pDisplay, graphic_strings_t *StringsStruct)
{
	if(!StringsStruct->TimerInitialized)
	{
		StringsStruct->TimerInitialized = true;
		if(!StringsStruct->TimerScrollInterval) StringsStruct->TimerScrollInterval = 1000;
		timer_interval(&StringsStruct->TimerScroll, StringsStruct->TimerScrollInterval);
	}
	bool Return = true;
	tRectangle sClipRegion;
	sClipRegion = pDisplay->sClipRegion;
	pDisplay->sClipRegion = StringsStruct->sClipRegion;
	if(!StringsStruct->WordWrap && timer_tick(&StringsStruct->TimerScroll))
	{
		if(StringsStruct->X_Location + StringsStruct->string_width_get_function(pDisplay, StringsStruct->pFont, StringsStruct->Str->text, -1) < pDisplay->sClipRegion.sXMin)
		{
			StringsStruct->X_Location =  StringsStruct->sClipRegion.sXMin;
			Return = false;
		}
		if(Return)
		{
			
			pDisplay->lcd_func.put_rectangle(pDisplay, pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax, pDisplay->sClipRegion.sYMax, true, StringsStruct->Background_Color);
			print_string_properties properties;
			memset(&properties, 0, sizeof(print_string_properties));
			properties.pDisplay = pDisplay;
			properties.pFont = StringsStruct->pFont;
			properties.pcString = str_clone(properties.pcString, StringsStruct->Str);
			//properties.pcString = StringsStruct->Str;
			properties.lLength = -1;
			properties.foreground_color = StringsStruct->Foreground_Color;
			properties.background_color = StringsStruct->Background_Color;
			properties.ulOpaque = StringsStruct->ulOpaque;
			properties.ulVisible = StringsStruct->ulVisible;
			properties.WordWrap = StringsStruct->WordWrap;
			properties.lX = StringsStruct->X_Location;
			properties.lY = StringsStruct->Y_Location;
			properties._SelStart = 0;
			properties._SelLen = 0;
			pDisplay->lcd_func.put_string(&properties);
			pDisplay->lcd_func.box_cache_clean(pDisplay, pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax, pDisplay->sClipRegion.sYMax);
			StringsStruct->X_Location--;
		}
	}
	//else
	//{
	//	put_rectangle(pDisplay, pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax, pDisplay->sClipRegion.sYMax, true, StringsStruct->Background_Color);
	//	StringsStruct->print_function(pDisplay, StringsStruct->pFont, Text, -1, StringsStruct->Foreground_Color, StringsStruct->Background_Color, StringsStruct->ulOpaque, StringsStruct->ulVisible, StringsStruct->WordWrap, StringsStruct->X_Location, StringsStruct->Y_Location, 0, 0);
	//	box_cache_clean(pDisplay, pDisplay->sClipRegion.sXMin, pDisplay->sClipRegion.sYMin, pDisplay->sClipRegion.sXMax, pDisplay->sClipRegion.sYMax);
	//}
	pDisplay->sClipRegion = sClipRegion;
	return Return;
}
//#######################################################################################
#ifdef FLASH_DEVICE
bool put_roll_string(tDisplay *pDisplay, graphic_strings_t *StringsStruct)
{
	bool Return = false;
	if(StringsStruct->Pstr)
	{
		char* Str = (char*)malloc(strlen_P(StringsStruct->Pstr) + 1);
		if(!Str) return false;
		char *StrBack = StringsStruct->Str;
		StringsStruct->Str = Str;
		strcpy_P(Str, StringsStruct->Pstr);
		Return = _put_roll_string(pDisplay, StringsStruct);
		StringsStruct->Str = StrBack;
		free(Str);
	}
	if(StringsStruct->Str)
	{
		Return = _put_roll_string(pDisplay, StringsStruct);
	}
	return Return;
}
#endif
//#######################################################################################
