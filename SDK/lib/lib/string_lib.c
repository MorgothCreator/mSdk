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

char* str_to_upercase(char* string)
{
	unsigned int TmpCnt = strlen(string);
	char* Str = string;
	do{
		*Str = toupper(*Str);
		Str++;
	}while(--TmpCnt);
	return string;
}

char* str_to_lowercase(char* string)
{
	unsigned int TmpCnt = strlen(string);
	char* Str = string;
	do{
		*Str = tolower(*Str);
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

char *str_copy(char* str)
{
	if(!str) return NULL;
	unsigned int LenSrc = strlen(str);
	char *Return = (char *)realloc(str, LenSrc + 1);
	strcpy(Return, str);
	return Return;
}
