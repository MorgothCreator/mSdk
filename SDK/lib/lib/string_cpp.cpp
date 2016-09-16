/*
 * string_cpp.cpp
 *
 *  Created on: Aug 20, 2016
 *      Author: John Smith
 */


#include "string_cpp.h"

String::String(char *text)
{

}

String::~String()
{

}

void String::RemoveNewLine(String_t *string)
{
	char *_item = string->text;
	char *tmp_str1 = string->text;
	char *tmp_str2 = string->text;
	while(*_item != 0)
	{
		if(*_item == '\n' || *_item == '\r')
			_item++;
		else
			*tmp_str1++ = *_item++;
	}
	*tmp_str1 = 0;
	string->len = tmp_str1 - tmp_str2;
	string->text = (char *)realloc(tmp_str2, (tmp_str1 - tmp_str2) + 1);
	string->error = STR_OK;
	string->modifyed = true;
}

void String::ToUperCase(String_t* string)
{
	unsigned int TmpCnt = strlen(string->text);
	char* Str = string->text;
	do{
		*Str = (char)toupper((int)*Str);
		Str++;
	}while(--TmpCnt);
	string->error = STR_OK;
	string->modifyed = true;
}

void String::ToLowerCase(String_t* string)
{
	unsigned int TmpCnt = strlen(string->text);
	char* Str = string->text;
	do{
		*Str = (char)tolower((int)*Str);
		Str++;
	}while(--TmpCnt);
	string->error = STR_OK;
	string->modifyed = true;
}

String_t *String::Append(String_t* dest, String_t* src)
{
	if(!src)
		return NULL;
	if(!dest)
	{
		dest = (String_t *)calloc(1, sizeof(String_t));
		if(!dest)
			return NULL;
		//StringInit(dest);
	}
	unsigned int LenSrc = strlen(src->text);
	unsigned int LenDest = strlen(dest->text);
	char *Return;
	if(dest->initialized)
		Return = (char *)realloc(dest->text, LenDest + LenSrc + 1);
	else
	{
		Return = (char *)malloc(LenDest + LenSrc + 1);
		*Return = 0;
		dest->initialized = true;
	}
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if(!Return)
		return NULL;
	strcat(Return, src->text);
	dest->text = Return;
	dest->len = LenDest + LenSrc;
	dest->modifyed = true;
	dest->error = STR_OK;
	return dest;
}

String_t *String::Insert(String_t* dest, String_t* src, unsigned int location)
{
	if(!src)
		return NULL;
	unsigned int LenSrc = strlen(src->text);
	if(location > LenSrc)
		return NULL;
	if(!dest)
	{
		dest = (String_t *)calloc(1, sizeof(String_t));
		if(!dest)
			return NULL;
		//StringInit(dest);
	}

	char *Return;
	if(dest->initialized)
	{
		Return = (char *)realloc(dest->text, strlen(dest->text) + LenSrc + 1);
		if(!Return)
			return NULL;
	}
	else
	{
		Return = (char *)malloc(LenSrc + 1);
		if(!Return)
			return NULL;
		*Return = 0;
		dest->initialized = true;
	}
	unsigned int LenDest = strlen(dest->text + location);
	char *Tmp = (char *)malloc(LenDest + 1);
	if(!Tmp)
		return NULL;
	strcpy(Tmp, dest->text + location);
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	strcpy(Return + location, src->text);
	strcat(Return, Tmp);
	free(Tmp);
	dest->text = Return;
	dest->len = LenDest + LenSrc;
	dest->error = STR_OK;
	dest->modifyed = true;
	return dest;
}

String_t *String::Clear(String_t* dest)
{
	if(!dest)
	{
		dest = (String_t *)calloc(1, sizeof(String_t));
		if(!dest)
			return NULL;
		//StringInit(dest);
	}
	char *Return;
	if(dest->initialized)
	{
		Return = (char *)realloc(dest->text, 1);
	}
	else
	{
		Return = (char *)malloc(1);
		dest->initialized = true;
	}
	*Return = 0;
	dest->len = 0;
	dest->text = Return;
	dest->error = STR_OK;
	dest->modifyed = true;
	return dest;
}

char* String::Copy(String_t* src)
{
	if(!src)
		return NULL;
	unsigned int src_len = strlen(src->text);
	char *Return = (char *)malloc(src_len + 1);
	if(!Return)
		return NULL;
	strcpy(Return, src->text);
	return Return;
}

char* String::Cut(String_t* src)
{
	if(!src)
		return NULL;
	unsigned int src_len = strlen(src->text);
	char *Return = (char *)malloc(src_len + 1);
	if(!Return)
		return NULL;
	strcpy(Return, src->text);
	char *src_raloc = (char *)realloc(src->text, 1);
	if(!src_raloc)
		return NULL;
	src->text[0] = 0;
	src->len = 0;
	src->error = STR_OUT_OF_MEMORY;
	src->modifyed = true;
	return Return;
}

String_t *String::Paste(String_t* dest, char* text)
{
	if(!text)
		return NULL;
	if(!dest)
	{
		dest = (String_t *)calloc(1, sizeof(String_t));
		if(!dest)
			return NULL;
		//StringInit(dest);
	}
	unsigned int LenSrc = strlen(text);
	unsigned int LenDest = strlen(dest->text);
	char *Return;
	if(dest->initialized)
		Return = (char *)realloc(dest->text, LenDest + LenSrc + 1);
	else
	{
		Return = (char *)malloc(LenDest + LenSrc + 1);
		*Return = 0;
		dest->initialized = true;
	}
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if(!Return)
		return NULL;
	strcat(Return, text);
	dest->text = Return;
	dest->len = LenDest + LenSrc;
	dest->modifyed = true;
	dest->error = STR_OK;
	return dest;
}

String_t *String::Set(String_t* dest, char* text)
{
	if(!text)
		return NULL;
	if(!dest)
	{
		dest = (String_t *)calloc(1, sizeof(String_t));
		if(!dest)
			return NULL;
		//StringInit(dest);
	}
	unsigned int LenSrc = strlen(text);
	char *Return;
	if(dest->initialized)
	{
		Return = (char *)realloc(dest->text, LenSrc + 1);
		*Return = 0;
	}
	else
	{
		Return = (char *)malloc(LenSrc + 1);
		*Return = 0;
		dest->initialized = true;
	}
	if(!Return)
		return NULL;
	strcat(Return, text);
	dest->text = Return;
	dest->len = LenSrc;
	dest->modifyed = true;
	dest->error = STR_OK;
	return dest;
}

int String::CharAt(String_t* dest, unsigned int position)
{
	if(!dest || !dest->text)
		return STR_NOT_ALLOCATED;
	if(position >= dest->len - 1)
		return STR_OUT_OF_RANGE;
	return dest->text[position];
}

char *String::Substring(String_t* src, unsigned int position, unsigned int len)
{
	if(!src || !src->text)
		return NULL;
	if(position + len >= src->len - 1)
		return NULL;
	char *Return;
	if(src->initialized)
	{
		Return = (char *)realloc(src->text, len + 1);
		*Return = 0;
	}
	else
	{
		Return = (char *)malloc(len + 1);
		*Return = 0;
		src->initialized = true;
	}
	//char *Return = (char *)calloc(1, len + 1);
	if(!Return)
		return NULL;
	strncpy(Return, src->text + position, len);
	return Return;
}

String_t *String::Clone(String_t* dest, String_t* src)
{
	if(!src)
		return NULL;
	if(!dest)
	{
		dest = (String_t *)calloc(1, sizeof(String_t));
		if(!dest)
			return NULL;
		//StringInit(dest);
	}
	unsigned int LenSrc = strlen(src->text);
	char *Return;
	if(dest->initialized)
		Return = (char *)realloc(dest->text, LenSrc + 1);
	else
	{
		Return = (char *)malloc(LenSrc + 1);
		*Return = 0;
		dest->initialized = true;
	}
	//char *Return = (char *)realloc(dest, LenDest + LenSrc + 1);
	if(!Return)
		return NULL;
	strcpy(Return, src->text);
	dest->text = Return;
	dest->len = LenSrc;
	dest->modifyed = true;
	dest->error = STR_OK;
	return dest;
}

String_t *String::Free(String_t* str)
{
	if(str->text)
		free(str->text);
	if(str)
		free(str);
	return NULL;
}

String_t ** str_array_new()
{
	return (String_t **)calloc(1, sizeof(String_t *));
}

STR_RESULT String::ArrayFree(String_t **array)
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

STR_RESULT String::ArrayGetLen(String_t **array, unsigned int *items_nr)
{
	if(!array) return STR_NOT_ALLOCATED;
	unsigned int free_string_cnt = 0;
	while(array[free_string_cnt]) free_string_cnt++;
	*items_nr = free_string_cnt;
	return STR_OK;
}

String_t **String::ArrayAddItem(String_t **array, String_t *item)
{
	if(!array) return NULL;
	unsigned int items_nr = 0;
	STR_RESULT status = ArrayGetLen(array, &items_nr);
	if(status != STR_OK)
	{
		return NULL;
	}
	String_t **new_str_addr = (String_t **)realloc(array, (items_nr + 2) * sizeof(String_t *));
	if(!new_str_addr)
	{
		return NULL;
	}
	//array = new_str_addr;
	//char **_array = &**array;
	new_str_addr[items_nr + 1] = 0;
	//new_string *tmp_str = new_(new_string);
	//memcpy(tmp_str, item, strlen);
	RemoveNewLine(item);
	new_str_addr[items_nr] = item;
	if(!new_str_addr[items_nr])
	{
		return NULL;
	}
	//strcpy(new_str_addr[items_nr], tmp_str2);
	return new_str_addr;
}

STR_RESULT String::ArrayGetItem(String_t **item, String_t **array, unsigned int item_nr)
{
	unsigned int items_nr = 0;
	STR_RESULT status =	ArrayGetLen(array, &items_nr);
	if(status != STR_OK)
	{
		return STR_NOT_ALLOCATED;
	}
	if(item_nr >= items_nr) return STR_OUT_OF_RANGE;
	*item = array[item_nr];
	return STR_OK;
}
