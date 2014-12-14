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

//#define string (char)

typedef enum{
	STR_OK = 0,
	STR_UNKNOWN_ERR,
	STR_OUT_OF_MEMORY,
	STR_OUT_OF_RANGE,
	STR_NOT_ALLOCATED
}STR_RESULT;

#define string(name, text)\
	char *name = malloc(sizeof(text) + 1);\
	strcpy(name, text);

#define string_free(name)\
		if(name) free(name);

char* str_remove_new_line(char *string);
char* str_to_upercase(char* string);
char* str_to_lowercase(char* string);
char *str_append(char* dest, char* src);
char *str_insert(char* dest, char* src, unsigned int location);
char *str_clear(char* dest);
char *str_copy(char* str);

char ** str_array_new();
STR_RESULT str_array_free(char **array);
STR_RESULT str_array_items_nr(char **array, unsigned int *items_nr);
char **str_array_item_add(char **array, char *item);
STR_RESULT str_array_item_get(char **item, char **array, unsigned int item_nr);


//#######################################################
#define new_string char*
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_string(address) free(address);
//#######################################################


#ifdef HEADER_INCLUDE_C_FILES
#include "string_lib.c"
#endif

#endif /* STRING_LIB_H_ */
