/*
 * path_util.c
 *
 *  Created on: Oct 6, 2014
 *      Author: Morgoth
 */

#include <string.h>
#include <stdlib.h>
#include "path_util.h"
#include "../app_result.h"
#include "lib/string_lib.h"

String_t *path_append_parse(String_t *path, String_t *rel_path)
{
	if(!path || !rel_path)
		return NULL;
	String_t *a = path;
	String_t *b = rel_path;
	if(str_char_at(b, 0) == 0)
		return path;
	if(str_char_at(a, 0) == 0)
		return str_paste(a, "/");
	if(str_char_at(b, 0) == ':' && str_char_at(b, 1) == '/')
	{
		str_clear(a);
		str_paste(a, "/");
	}
	unsigned int ptr_a = 0;
	unsigned int ptr_b = 0;
	while(b->text[0] == '/' && b->len > 1)
		ptr_b++;
	if(str_char_at(a, a->len - 1) == '/' && b->text[ptr_b] == '/')
		ptr_b++;
	while(b->len - ptr_b)
	{
		if(b->text[0] != 0 && b->text[0] == '.' && b->text[1] == '.' && b->text[1] != 0 && b->text[2] != 0 && b->text[2] == '/')
		{
			int a_len = a->len - ptr_a;
			if(a_len > 1)
			{
				unsigned int _a = a_len - 1;
				while(a->len - _a != 0 && a->text[--_a] != '/');
				a->text[_a/* + 1*/] = 0;
				a_len = a->len - _a;
				if(!a_len)
					str_paste(a, "/");
				//a = realloc(a, a_len + 1);
			}
			//b = (char *)(b + 3);
			ptr_b += 3;
		}
		else
		{
			unsigned int _b = b->len - ptr_b;
			//int _b_len = strlen(_b);
			while(b->text[ptr_b] != '/' && b->len - ptr_b != 0)
				ptr_b++;
			//a = realloc(a, strlen(a) + (b - _b) + 3);
			if(b->text[_b] != '/' && a->text[a->len - ptr_a] != '/')
				str_paste(a, "/");
			char* _tmp_a_substring = str_substring(a, _b, (ptr_b - _b) + 1);
			str_paste(a, _tmp_a_substring);
			if(_tmp_a_substring)
				free(_tmp_a_substring);
			//strncat(a, _b, (b - _b) + 1);
			if(b->text[ptr_b] == '/')
				ptr_b++;
		}
	}
	return STR_OK;
}
