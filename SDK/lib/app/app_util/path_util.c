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

char *path_append_parse(char *path, char *rel_path)
{
	if(!path || !rel_path) return path;
	char *a = path;
	char *b = rel_path;
	if(*b == 0) return a;
	if(*a == 0) return str_append(a, "/");
	if(*b == ':' && b[1] == '/')
	{
		a = str_clear(a);
		a = str_append(a, "/");
	}
	while(*b == '/' && strlen(b) > 1) b++;
	if(a[strlen(a)- 1] == '/' && *b == '/') b++;
	while(strlen(b))
	{
		if(b[0] != 0 && b[0] == '.' && b[1] == '.' && b[1] != 0 && b[2] != 0 && b[2] == '/')
		{
			int a_len = strlen(a);
			if(a_len > 1)
			{
				char *_a = (char *)(a + a_len - 1);
				while(_a != a && *--_a != '/');
				*(_a/* + 1*/) = 0;
				a_len = strlen(a);
				if(!a_len)
					strcat(a, "/");
				a = realloc(a, a_len + 1);
			}
			b = (char *)(b + 3);
		}
		else
		{
			char *_b = b;
			//int _b_len = strlen(_b);
			while(*b != '/' && *b != 0) b++;
			a = realloc(a, strlen(a) + (b - _b) + 3);
			if(*_b != '/' && a[strlen(a) - 1] != '/')
				strcat(a, "/");
			strncat(a, _b, (b - _b) + 1);
			if(*b == '/') b++;
		}
	}
	return a;
}
