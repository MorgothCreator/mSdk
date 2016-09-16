/*
 * string_cpp.h
 *
 *  Created on: Aug 20, 2016
 *      Author: John Smith
 */

#ifndef LIB_LIB_STRING_CPP_H_
#define LIB_LIB_STRING_CPP_H_

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "string_lib.h"

class String
{
public:
	String(char *text);
	~String();
	static void RemoveNewLine(String_t *string);
	static void ToUperCase(String_t* string);
	static void ToLowerCase(String_t* string);
	static String_t *Append(String_t* Dest, String_t* Src);
	static String_t *Insert(String_t* Dest, String_t* Src, unsigned int Location);
	static String_t *Clear(String_t* dest);
	static char* Cut(String_t* src);
	static char* Copy(String_t* dest);
	static String_t *Paste(String_t* Str, char *Text);
	static String_t *Set(String_t* dest, char* text);
	static int CharAt(String_t* dest, unsigned int position);
	static char *Substring(String_t* src, unsigned int position, unsigned int len);
	static String_t *Clone(String_t* dest, String_t* src);
	static String_t *Free(String_t* str);

	static String_t ** NewArray();
	static STR_RESULT ArrayFree(String_t **array);
	static STR_RESULT ArrayGetLen(String_t **array, unsigned int *items_nr);
	static String_t **ArrayAddItem(String_t **array, String_t *item);
	static STR_RESULT ArrayGetItem(String_t **item, String_t **array, unsigned int item_nr);

	char *text;
	unsigned int len;
	bool modifyed;
	bool initialized;
	STR_RESULT error;

protected:
private:
};

#endif /* LIB_LIB_STRING_CPP_H_ */
