/*
 *  lib/string_lib.h
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

#ifndef STRING_LIB_H_
#define STRING_LIB_H_

#include <stdbool.h>

//#define string (char)

typedef enum{
	STR_OK = 0,
	STR_UNKNOWN_ERR = -1,
	STR_OUT_OF_MEMORY = -2,
	STR_OUT_OF_RANGE = -3,
	STR_NOT_ALLOCATED = -4
}STR_RESULT;


typedef struct String_s {
	char *text;
	unsigned int len;
	bool modifyed;
	bool initialized;
	STR_RESULT error;
}String_t;

typedef struct {
	void (*RemoveNewLine)(String_t *string);
	void (*ToUperCase)(String_t* string);
	void (*ToLowerCase)(String_t* string);
	String_t *(*Append)(String_t* Dest, String_t* Src);
	String_t *(*AppendChar)(String_t* dest, char Char);
	String_t *(*AppendF)(String_t* dest, char* str, ...);
	String_t *(*Insert)(String_t* Dest, String_t* Src, unsigned int Location);
	String_t *(*Clear)(String_t* dest);
	char* (*Cut)(String_t* Str);
	char* (*Copy)(String_t* dest);
	String_t *(*Paste)(String_t* Str, char *Text);
	String_t *(*Set)(String_t* dest, char* text);
	int (*CharAt)(String_t* dest, unsigned int position);
	char *(*Substring)(String_t* src, unsigned int position, unsigned int len);
	String_t *(*Clone)(String_t* dest, String_t* src);
	String_t *(*Free)(String_t* str);

	String_t ** (*NewArray)();
	STR_RESULT (*ArrayFree)(String_t **array);
	STR_RESULT (*ArrayGetLen)(String_t **array, unsigned int *items_nr);
	struct String_s **(*ArrayAddItem)(String_t **array, String_t *item);
	STR_RESULT (*ArrayGetItem)(String_t **item, String_t **array, unsigned int item_nr);

	String_t *(*SetF)(String_t* dest, char* str, ...);
}StringFunc_t;

extern StringFunc_t String;


#define string(ptr, _text)\
		String_t ptr;\
		ptr.text = (char *)malloc(strlen(_text) + 1);\
		if(ptr.text)\
		{\
			strcpy(ptr.text, _text);\
			ptr.len = strlen(_text);\
			ptr.modifyed = true;\
			ptr.error = STR_OK;\
		}


#define string_free(ptr)\
		if(ptr->text)\
			free(ptr->text)

void str_remove_new_line(String_t *string);
void str_to_upercase(String_t* string);
void str_to_lowercase(String_t* string);
String_t *str_append(String_t* Dest, String_t* Src);
String_t *str_append_char(String_t* dest, char Char);
String_t *str_insert(String_t* Dest, String_t* Src, unsigned int Location);
String_t *str_clear(String_t* dest);
char* str_cut(String_t* Str);
char* str_copy(String_t* dest);
String_t *str_paste(String_t* Str, char *Text);
String_t *str_set(String_t* dest, char* text);
int str_char_at(String_t* dest, unsigned int position);
char *str_substring(String_t* src, unsigned int position, unsigned int len);
String_t *str_clone(String_t* dest, String_t* src);
String_t *str_free(String_t* str);

String_t ** str_array_new();
STR_RESULT str_array_free(String_t **array);
STR_RESULT str_array_items_nr(String_t **array, unsigned int *items_nr);
String_t **str_array_item_add(String_t **array, String_t *item);
STR_RESULT str_array_item_get(String_t **item, String_t **array, unsigned int item_nr);

String_t *str_setf(String_t* dest, char* str, ...);
String_t *str_appendf(String_t* dest, char* str, ...);

//#######################################################
#define new_string String_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure))
#endif
#define free_string(address) free(address);
//#######################################################


#ifdef HEADER_INCLUDE_C_FILES
#include "string_lib.c"
#endif

#endif /* STRING_LIB_H_ */
