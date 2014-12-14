/*
 *  lib/string_lib.c
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

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "string_lib.h"

char* str_remove_new_line(char *string)
{
	char *_item = string;
	char *tmp_str1 = string;
	char *tmp_str2 = string;
	while(*_item != 0)
	{
		if(*_item == '\n' || *_item == '\r') _item++;
		else *tmp_str1++ = *_item++;
	}
	*tmp_str1 = 0;
	return realloc(tmp_str2, (tmp_str1 - tmp_str2) + 1);
}

char* str_to_upercase(char* string)
{
	unsigned int TmpCnt = strlen(string);
	char* Str = string;
	do{
		*Str = (char)toupper((int)*Str);
		Str++;
	}while(--TmpCnt);
	return string;
}

char* str_to_lowercase(char* string)
{
	unsigned int TmpCnt = strlen(string);
	char* Str = string;
	do{
		*Str = (char)tolower((int)*Str);
		Str++;
	}while(--TmpCnt);
	return string;
}

char *str_append(char* dest, char* src)
{
	if(!dest || !src) return NULL;
	unsigned int LenSrc = strlen(src);
	unsigned int LenDest = strlen(dest);
	char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if(!Return) return NULL;
	strcat(Return, src);
	return Return;
}

char *str_insert(char* dest, char* src, unsigned int location)
{
	if(!dest || !src) return NULL;
	unsigned int LenSrc = strlen(src);
	if(location > LenSrc) return NULL;
	unsigned int LenDest = strlen(dest);
	char *Tmp = (char *)malloc(LenSrc + 1);
	if(!Tmp) return NULL;
	strcpy(Tmp, dest + location);
	char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if(!Return)
	{
		free(Tmp);
		return NULL;
	}
	strcpy(Return + location, src);
	strcat(Return, Tmp);
	free(Tmp);
	return Return;
}

char *str_clear(char* dest)
{
	char * a = realloc(dest, 1);
	*a = 0;
	return a;
}

char *str_copy(char* str)
{
	if(!str) return NULL;
	char *Return = (char *)malloc(strlen(str) + 1);
	strcpy(Return, str);
	return Return;
}

char ** str_array_new()
{
	return calloc(1, sizeof(char *));
}

STR_RESULT str_array_free(char **array)
{
	if(!array) return STR_NOT_ALLOCATED;
	unsigned int free_string_cnt = 0;
	while(array[free_string_cnt] != 0)
	{
		if(array[free_string_cnt]) free(array[free_string_cnt]);
		array[free_string_cnt] = NULL;
	}
	if(array) free(array);
	return STR_OK;
}

STR_RESULT str_array_items_nr(char **array, unsigned int *items_nr)
{
	if(!array) return STR_NOT_ALLOCATED;
	unsigned int free_string_cnt = 0;
	while(array[free_string_cnt]) free_string_cnt++;
	*items_nr = free_string_cnt;
	return STR_OK;
}

char **str_array_item_add(char **array, char *item)
{
	if(!array) return NULL;
	unsigned int items_nr = 0;
	STR_RESULT status = str_array_items_nr(array, &items_nr);
	if(status != STR_OK)
	{
		return NULL;
	}
	char **new_str_addr = realloc(array, (items_nr + 2) * sizeof(char *));
	if(!new_str_addr)
	{
		return NULL;
	}
	//array = new_str_addr;
	//char **_array = &**array;
	new_str_addr[items_nr + 1] = 0;
	char *tmp_str = malloc(strlen(item) + 1);
	strcpy(tmp_str, item);
	new_str_addr[items_nr] = str_remove_new_line(tmp_str);
	if(!new_str_addr[items_nr])
	{
		return NULL;
	}
	//strcpy(new_str_addr[items_nr], tmp_str2);
	return new_str_addr;
}

STR_RESULT str_array_item_get(char **item, char **array, unsigned int item_nr)
{
	unsigned int items_nr = 0;
	STR_RESULT status =	str_array_items_nr(array, &items_nr);
	if(status != STR_OK)
	{
		return STR_NOT_ALLOCATED;
	}
	if(item_nr >= items_nr) return STR_OUT_OF_RANGE;
	*item = array[item_nr];
	return STR_OK;
}

