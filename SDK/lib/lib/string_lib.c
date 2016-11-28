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
#include <stdio.h>
#include "string_lib.h"


//static int string_printf(
//		String_t* fp,			/* Pointer to the file object */
//		const char* str,	/* Pointer to the format string */
//		...					/* Optional arguments... */
//);

StringFunc_t String = {
	.Append = str_append,
	.AppendChar = str_append_char,
	.AppendF = str_appendf,
	.ArrayAddItem = str_array_item_add,
	.ArrayFree = str_array_free,
	.ArrayGetItem = str_array_item_get,
	.ArrayGetLen = str_array_items_nr,
	.CharAt = str_char_at,
	.Clear = str_clear,
	.Clone = str_clone,
	.Copy = str_copy,
	.Cut = str_cut,
	.Free = str_free,
	.Insert = str_insert,
	.NewArray = str_array_new,
	.Paste = str_paste,
	.RemoveNewLine = str_remove_new_line,
	.Set = str_set,
	.Substring = str_substring,
	.ToLowerCase = str_to_lowercase,
	.ToUperCase = str_to_upercase,
	.SetF = str_setf,
};

void str_remove_new_line(String_t *string)
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
	string->text = realloc(tmp_str2, (tmp_str1 - tmp_str2) + 1);
	string->error = STR_OK;
	string->modifyed = true;
}

void str_to_upercase(String_t* string)
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

void str_to_lowercase(String_t* string)
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

String_t *str_append(String_t* dest, String_t* src)
{
	if(!src)
		return NULL;
	if(!dest)
	{
		dest = calloc(1, sizeof(String_t));
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

String_t *str_append_char(String_t* dest, char Char)
{
	if(!dest)
	{
		dest = calloc(1, sizeof(String_t));
		if(!dest)
			return NULL;
		//StringInit(dest);
	}
	unsigned int LenSrc = 1;
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
	char buff_char[2];
	buff_char[0] = Char;
	buff_char[1] = '\0';
	strcat(Return, buff_char);
	dest->text = Return;
	dest->len = LenDest + LenSrc;
	dest->modifyed = true;
	dest->error = STR_OK;
	return dest;
}

String_t *str_insert(String_t* dest, String_t* src, unsigned int location)
{
	if(!src)
		return NULL;
	unsigned int LenSrc = strlen(src->text);
	if(location > LenSrc)
		return NULL;
	if(!dest)
	{
		dest = calloc(1, sizeof(String_t));
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

String_t *str_clear(String_t* dest)
{
	if(!dest)
	{
		dest = calloc(1, sizeof(String_t));
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

char* str_copy(String_t* src)
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

char* str_cut(String_t* src)
{
	if(!src)
		return NULL;
	unsigned int src_len = strlen(src->text);
	char *Return = (char *)malloc(src_len + 1);
	if(!Return)
		return NULL;
	strcpy(Return, src->text);
	char *src_raloc = realloc(src->text, 1);
	if(!src_raloc)
		return NULL;
	src->text[0] = 0;
	src->len = 0;
	src->error = STR_OUT_OF_MEMORY;
	src->modifyed = true;
	return Return;
}

String_t *str_paste(String_t* dest, char* text)
{
	if(!text)
		return NULL;
	if(!dest)
	{
		dest = calloc(1, sizeof(String_t));
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

String_t *str_set(String_t* dest, char* text)
{
	if(!text)
		return NULL;
	if(!dest)
	{
		dest = calloc(1, sizeof(String_t));
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

int str_char_at(String_t* dest, unsigned int position)
{
	if(!dest || !dest->text)
		return STR_NOT_ALLOCATED;
	if(position >= dest->len - 1)
		return STR_OUT_OF_RANGE;
	return dest->text[position];
}

char *str_substring(String_t* src, unsigned int position, unsigned int len)
{
	if(!src || !src->text)
		return NULL;
	if(position + len >= src->len)
		return NULL;
	char *Return = (char *)calloc(1, len + 1);
	if(!Return)
		return NULL;
	*Return = 0;
	strncpy(Return, src->text + position, len);
	return Return;
}

String_t *str_clone(String_t* dest, String_t* src)
{
	if(!src)
		return NULL;
	if(!dest)
	{
		dest = calloc(1, sizeof(String_t));
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

String_t *str_free(String_t* str)
{
	if(str->text)
		free(str->text);
	if(str)
		free(str);
	return NULL;
}

String_t ** str_array_new()
{
	return calloc(1, sizeof(String_t *));
}

STR_RESULT str_array_free(String_t **array)
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

STR_RESULT str_array_items_nr(String_t **array, unsigned int *items_nr)
{
	if(!array) return STR_NOT_ALLOCATED;
	unsigned int free_string_cnt = 0;
	while(array[free_string_cnt]) free_string_cnt++;
	*items_nr = free_string_cnt;
	return STR_OK;
}

String_t **str_array_item_add(String_t **array, String_t *item)
{
	if(!array) return NULL;
	unsigned int items_nr = 0;
	STR_RESULT status = str_array_items_nr(array, &items_nr);
	if(status != STR_OK)
	{
		return NULL;
	}
	String_t **new_str_addr = realloc(array, (items_nr + 2) * sizeof(String_t *));
	if(!new_str_addr)
	{
		return NULL;
	}
	//array = new_str_addr;
	//char **_array = &**array;
	new_str_addr[items_nr + 1] = 0;
	//new_string *tmp_str = new_(new_string);
	//memcpy(tmp_str, item, strlen);
	str_remove_new_line(item);
	new_str_addr[items_nr] = item;
	if(!new_str_addr[items_nr])
	{
		return NULL;
	}
	//strcpy(new_str_addr[items_nr], tmp_str2);
	return new_str_addr;
}

STR_RESULT str_array_item_get(String_t **item, String_t **array, unsigned int item_nr)
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

/* Version number of package */
#undef VERSION//#include "config.h"

#if !defined(HAVE_SNPRINTF) || !defined(HAVE_VSNPRINTF)

#include <string.h>
#include <ctype.h>
#ifndef USE_AVR_STUDIO
#include <sys/types.h>
#endif


/* Define this as a fall through, HAVE_STDARG_H is probably already set */

#define HAVE_VARARGS_H


/* varargs declarations: */
#define HAVE_STDARG_H

#if defined(HAVE_STDARG_H)
# include "stdarg.h"
# define HAVE_STDARGS    /* let's hope that works everywhere (mj) */
# define VA_LOCAL_DECL   va_list ap
# define VA_START(f)     va_start(ap, f)
# define VA_SHIFT(v,t)  ;   /* no-op for ANSI */
# define VA_END          va_end(ap)
#else
# if defined(HAVE_VARARGS_H)
#  include "varargs.h"
#  undef HAVE_STDARGS
#  define VA_LOCAL_DECL   va_list ap
#  define VA_START(f)     va_start(ap)      /* f is ignored! */
#  define VA_SHIFT(v,t) v = va_arg(ap,t)
#  define VA_END        va_end(ap)
# else
/*XX ** NO VARARGS ** XX*/
# endif
#endif

#ifdef HAVE_LONG_DOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif

//int snprintf (Uart_t* UartSettings, char *str, size_t count, const char *fmt, ...);
int str_vsnprintf_ (String_t* fp, char *str, size_t count, const char *fmt, va_list arg);// Renamed to avoid conflict with builtin 'vsnprintf' function.

static void dopr (String_t* fp, char *buffer, size_t maxlen, const char *format,
                  va_list args);
static void fmtstr (String_t* fp, char *buffer, size_t *currlen, size_t maxlen,
		    char *value, int flags, int min, int max);
static void fmtint (String_t* fp, char *buffer, size_t *currlen, size_t maxlen,
		    long value, int base, int min, int max, int flags);
static void fmtfp (String_t* fp, char *buffer, size_t *currlen, size_t maxlen,
		   LDOUBLE fvalue, int min, int max, int flags);
static void dopr_outch (String_t* fp, char *buffer, size_t *currlen, size_t maxlen, char c );

/*
 * dopr(): poor man's version of doprintf
 */

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS   1
#define DP_S_MIN     2
#define DP_S_DOT     3
#define DP_S_MAX     4
#define DP_S_MOD     5
#define DP_S_CONV    6
#define DP_S_DONE    7

/* format flags - Bits */
#define DP_F_MINUS 	(1 << 0)
#define DP_F_PLUS  	(1 << 1)
#define DP_F_SPACE 	(1 << 2)
#define DP_F_NUM   	(1 << 3)
#define DP_F_ZERO  	(1 << 4)
#define DP_F_UP    	(1 << 5)
#define DP_F_UNSIGNED 	(1 << 6)

/* Conversion Flags */
#define DP_C_SHORT   1
#define DP_C_LONG    2
#define DP_C_LDOUBLE 3

#define char_to_int(p) (p - '0')
#define MAX(p,q) ((p >= q) ? p : q)

static void dopr (String_t* fp, char *buffer, size_t maxlen, const char *format, va_list args)
{
  int ch;
  long value;
  LDOUBLE fvalue;
  char *strvalue;
  int min;
  int max;
  int state;
  int flags;
  int cflags;
  size_t currlen;

  state = DP_S_DEFAULT;
  currlen = flags = cflags = min = 0;
  max = -1;
  ch = *format++;

  while (state != DP_S_DONE)
  {
    if ((ch == '\0') || (currlen >= maxlen))
      state = DP_S_DONE;

    switch(state)
    {
    case DP_S_DEFAULT:
      if (ch == '%')
	state = DP_S_FLAGS;
      else
	dopr_outch (fp, buffer, &currlen, maxlen, ch);
      ch = *format++;
      break;
    case DP_S_FLAGS:
      switch (ch)
      {
      case '-':
	flags |= DP_F_MINUS;
        ch = *format++;
	break;
      case '+':
	flags |= DP_F_PLUS;
        ch = *format++;
	break;
      case ' ':
	flags |= DP_F_SPACE;
        ch = *format++;
	break;
      case '#':
	flags |= DP_F_NUM;
        ch = *format++;
	break;
      case '0':
	flags |= DP_F_ZERO;
        ch = *format++;
	break;
      default:
	state = DP_S_MIN;
	break;
      }
      break;
    case DP_S_MIN:
      if (isdigit(ch))
      {
	min = 10*min + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	min = va_arg (args, int);
	ch = *format++;
	state = DP_S_DOT;
      }
      else
	state = DP_S_DOT;
      break;
    case DP_S_DOT:
      if (ch == '.')
      {
	state = DP_S_MAX;
	ch = *format++;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MAX:
      if (isdigit(ch))
      {
	if (max < 0)
	  max = 0;
	max = 10*max + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	max = va_arg (args, int);
	ch = *format++;
	state = DP_S_MOD;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MOD:
      /* Currently, we don't support Long Long, bummer */
      switch (ch)
      {
      case 'h':
	cflags = DP_C_SHORT;
	ch = *format++;
	break;
      case 'l':
	cflags = DP_C_LONG;
	ch = *format++;
	break;
      case 'L':
	cflags = DP_C_LDOUBLE;
	ch = *format++;
	break;
      default:
	break;
      }
      state = DP_S_CONV;
      break;
    case DP_S_CONV:
      switch (ch)
      {
      case 'd':
      case 'i':
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, long);
	else
	  value = va_arg (args, int);
	fmtint (fp, buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'o':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, unsigned int);
	fmtint (fp, buffer, &currlen, maxlen, value, 8, min, max, flags);
	break;
      case 'u':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, unsigned int);
	fmtint (fp, buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'X':
	flags |= DP_F_UP;
      case 'x':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, unsigned int);
	fmtint (fp, buffer, &currlen, maxlen, value, 16, min, max, flags);
	break;
      case 'f':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	/* um, floating point? */
	fmtfp (fp, buffer, &currlen, maxlen, fvalue, min, max, flags);
	break;
      case 'E':
	flags |= DP_F_UP;
      case 'e':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'G':
	flags |= DP_F_UP;
      case 'g':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'c':
	dopr_outch (fp, buffer, &currlen, maxlen, va_arg (args, int));
	break;
      case 's':
	strvalue = va_arg (args, char *);
	if (max < 0)
	  max = maxlen; /* ie, no max */
	fmtstr (fp, buffer, &currlen, maxlen, strvalue, flags, min, max);
	break;
      case 'p':
	strvalue = va_arg (args, void *);
	fmtint (fp, buffer, &currlen, maxlen, strvalue, 16, min, max, flags);
	break;
      case 'n':
	if (cflags == DP_C_SHORT)
	{
	  short int *num;
	  num = va_arg (args, short *);
	  *num = currlen;
        }
	else if (cflags == DP_C_LONG)
	{
	  long int *num;
	  num = va_arg (args, long *);
	  *num = currlen;
        }
	else
	{
	  int *num;
	  num = va_arg (args, int *);
	  *num = currlen;
        }
	break;
      case '%':
	dopr_outch (fp, buffer, &currlen, maxlen, ch);
	break;
      case 'w':
	/* not supported yet, treat as next char */
	ch = *format++;
	break;
      default:
	/* Unknown, skip */
	break;
      }
      ch = *format++;
      state = DP_S_DEFAULT;
      flags = cflags = min = 0;
      max = -1;
      break;
    case DP_S_DONE:
      break;
    default:
      /* hmm? */
      break; /* some picky compilers need this */
    }
  }
  if (currlen < maxlen - 1)
    buffer[currlen] = '\0';
  else
    buffer[maxlen - 1] = '\0';
}

static void fmtstr (String_t* fp, char *buffer, size_t *currlen, size_t maxlen,
		    char *value, int flags, int min, int max)
{
  int padlen, strln;     /* amount to pad */
  int cnt = 0;

  if (value == 0)
  {
    value = "<NULL>";
  }

  for (strln = 0; value[strln]; ++strln); /* strlen */
  padlen = min - strln;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justify */

  while ((padlen > 0) && (cnt < max))
  {
    dopr_outch (fp, buffer, currlen, maxlen, ' ');
    --padlen;
    ++cnt;
  }
  while (*value && (cnt < max))
  {
    dopr_outch (fp, buffer, currlen, maxlen, *value++);
    ++cnt;
  }
  while ((padlen < 0) && (cnt < max))
  {
    dopr_outch (fp, buffer, currlen, maxlen, ' ');
    ++padlen;
    ++cnt;
  }
}

/* Have to handle DP_F_NUM (ie 0x and 0 alternates) */

static void fmtint (String_t* fp, char *buffer, size_t *currlen, size_t maxlen,
		    long value, int base, int min, int max, int flags)
{
  int signvalue = 0;
  unsigned long uvalue;
  char convert[20];
  int place = 0;
  int spadlen = 0; /* amount to space pad */
  int zpadlen = 0; /* amount to zero pad */
  int caps = 0;

  if (max < 0)
    max = 0;

  uvalue = value;

  if(!(flags & DP_F_UNSIGNED))
  {
    if( value < 0 ) {
      signvalue = '-';
      uvalue = -value;
    }
    else
      if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
	signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';
  }

  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */

  do {
    convert[place++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")
      [uvalue % (unsigned)base  ];
    uvalue = (uvalue / (unsigned)base );
  } while(uvalue && (place < 20));
  if (place == 20) place--;
  convert[place] = 0;

  zpadlen = max - place;
  spadlen = min - MAX (max, place) - (signvalue ? 1 : 0);
  if (zpadlen < 0) zpadlen = 0;
  if (spadlen < 0) spadlen = 0;
  if (flags & DP_F_ZERO)
  {
    zpadlen = MAX(zpadlen, spadlen);
    spadlen = 0;
  }
  if (flags & DP_F_MINUS)
    spadlen = -spadlen; /* Left Justifty */

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
      zpadlen, spadlen, min, max, place));
#endif

  /* Spaces */
  while (spadlen > 0)
  {
    dopr_outch (fp, buffer, currlen, maxlen, ' ');
    --spadlen;
  }

  /* Sign */
  if (signvalue)
    dopr_outch (fp, buffer, currlen, maxlen, signvalue);

  /* Zeros */
  if (zpadlen > 0)
  {
    while (zpadlen > 0)
    {
      dopr_outch (fp, buffer, currlen, maxlen, '0');
      --zpadlen;
    }
  }

  /* Digits */
  while (place > 0)
    dopr_outch (fp, buffer, currlen, maxlen, convert[--place]);

  /* Left Justified spaces */
  while (spadlen < 0) {
    dopr_outch (fp, buffer, currlen, maxlen, ' ');
    ++spadlen;
  }
}

static LDOUBLE abs_val (LDOUBLE value)
{
  LDOUBLE result = value;

  if (value < 0)
    result = -value;

  return result;
}

static LDOUBLE _pow10_ (int exp)
{
  LDOUBLE result = 1;

  while (exp)
  {
    result *= 10;
    exp--;
  }

  return result;
}

static long _round_ (LDOUBLE value)
{
  long intpart;

  intpart = value;
  value = value - intpart;
  if (value >= 0.5)
    intpart++;

  return intpart;
}

static void fmtfp (String_t* fp, char *buffer, size_t *currlen, size_t maxlen,
		   LDOUBLE fvalue, int min, int max, int flags)
{
  int signvalue = 0;
  LDOUBLE ufvalue;
  char iconvert[20];
  char fconvert[20];
  int iplace = 0;
  int fplace = 0;
  int padlen = 0; /* amount to pad */
  int zpadlen = 0;
  int caps = 0;
  long intpart;
  long fracpart;

  /*
   * AIX manpage says the default is 0, but Solaris says the default
   * is 6, and sprintf on AIX defaults to 6
   */
  if (max < 0)
    max = 6;

  ufvalue = abs_val (fvalue);

  if (fvalue < 0)
    signvalue = '-';
  else
    if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
      signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';

#if 0
  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */
#endif

  intpart = ufvalue;

  /*
   * Sorry, we only support 9 digits past the decimal because of our
   * conversion method
   */
  if (max > 9)
    max = 9;

  /* We "cheat" by converting the fractional part to integer by
   * multiplying by a factor of 10
   */
  fracpart = _round_ ((_pow10_ (max)) * (ufvalue - intpart));

  if (fracpart >= _pow10_ (max))
  {
    intpart++;
    fracpart -= _pow10_ (max);
  }

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "fmtfp: %f =? %d.%d\n", fvalue, intpart, fracpart));
#endif

  /* Convert integer part */
  do {
    iconvert[iplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[intpart % 10];
    intpart = (intpart / 10);
  } while(intpart && (iplace < 20));
  if (iplace == 20) iplace--;
  iconvert[iplace] = 0;

  /* Convert fractional part */
  do {
    fconvert[fplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[fracpart % 10];
    fracpart = (fracpart / 10);
  } while(fracpart && (fplace < 20));
  if (fplace == 20) fplace--;
  fconvert[fplace] = 0;

  /* -1 for decimal point, another -1 if we are printing a sign */
  padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0);
  zpadlen = max - fplace;
  if (zpadlen < 0)
    zpadlen = 0;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justifty */

  if ((flags & DP_F_ZERO) && (padlen > 0))
  {
    if (signvalue)
    {
      dopr_outch (fp, buffer, currlen, maxlen, signvalue);
      --padlen;
      signvalue = 0;
    }
    while (padlen > 0)
    {
      dopr_outch (fp, buffer, currlen, maxlen, '0');
      --padlen;
    }
  }
  while (padlen > 0)
  {
    dopr_outch (fp, buffer, currlen, maxlen, ' ');
    --padlen;
  }
  if (signvalue)
    dopr_outch (fp, buffer, currlen, maxlen, signvalue);

  while (iplace > 0)
    dopr_outch (fp, buffer, currlen, maxlen, iconvert[--iplace]);

  /*
   * Decimal point.  This should probably use locale to find the correct
   * char to print out.
   */
  if (max > 0)
  {
    dopr_outch (fp, buffer, currlen, maxlen, '.');

    while (fplace > 0)
      dopr_outch (fp, buffer, currlen, maxlen, fconvert[--fplace]);
  }

  while (zpadlen > 0)
  {
    dopr_outch (fp, buffer, currlen, maxlen, '0');
    --zpadlen;
  }

  while (padlen < 0)
  {
    dopr_outch (fp, buffer, currlen, maxlen, ' ');
    ++padlen;
  }
}

static void dopr_outch (String_t* fp, char *buffer, size_t *currlen, size_t maxlen, char c)
{
  /*if (*currlen < maxlen)
    buffer[(*currlen)++] = c;*/
	char buff[2];
	buff[0] = c;
	buff[1] = 0;
	str_paste(fp, buff);
}

#ifndef HAVE_VSNPRINTF
int str_vsnprintf_ (String_t* fp, char *str, size_t count, const char *fmt, va_list args)
{
  str[0] = 0;
  dopr(fp, str, count, fmt, args);
  return(strlen(str));
}
#endif /* !HAVE_VSNPRINTF */



#if 0
#ifndef HAVE_SNPRINTF
/* VARARGS3 */
#ifdef HAVE_STDARGS
/*
 * Source of this description is : http://www.cplusplus.com/reference/cstdio/printf/
 * specifier	output
 *
 * d or i		Signed decimal integer																392
 * u			Unsigned decimal integer															7235
 * o			Unsigned octal																		610
 * x			Unsigned hexadecimal integer														7fa
 * X			Unsigned hexadecimal integer (uppercase)											7FA
 * f			Decimal floating point, lowercase													392.65
 * F			Decimal floating point, uppercase													392.65
 * e			Scientific notation (mantissa/exponent), lowercase									3.9265e+2
 * E			Scientific notation (mantissa/exponent), uppercase									3.9265E+2
 * g			Use the shortest representation: %e or %f											392.65
 * G			Use the shortest representation: %E or %F											392.65
 * a			Hexadecimal floating point, lowercase												-0xc.90fep-2
 * A			Hexadecimal floating point, uppercase												-0XC.90FEP-2
 * c			Character																			a
 * s			String of characters																sample
 * p			Pointer address																		b8000000
 * n			Nothing printed.
 * 					The corresponding argument must be a pointer to a signed int.
 * 					The number of characters written so far is stored in the pointed location.
 * %			A % followed by another % character will write a single % to the stream.			%
 *
 *
 *
 *
 *The format specifier can also contain sub-specifiers: flags, width, .precision and modifiers (in that order), which are optional and follow these specifications:

 * flags		description
 * -			Left-justify within the given field width; Right justification is the default (see width sub-specifier).
 * +			Forces to preceed the result with a plus or minus sign (+ or -) even for positive numbers. By default, only negative numbers are preceded with a - sign.
 * (space)		If no sign is going to be written, a blank space is inserted before the value.
 * #			Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X respectively for values different than zero.
 * 					Used with a, A, e, E, f, F, g or G it forces the written output to contain a decimal point even if no more digits follow. By default, if no digits follow, no decimal point is written.
 * 0			Left-pads the number with zeroes (0) instead of spaces when padding is specified (see width sub-specifier).
 *
 * width		description
 * (number)		Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger.
 * *			The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.
 *
 * .precision	description
 * .number		For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.
 * 					For a, A, e, E, f and F specifiers: this is the number of digits to be printed after the decimal point (by default, this is 6).
 * 					For g and G specifiers: This is the maximum number of significant digits to be printed.
 * 					For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.
 * 					If the period is specified without an explicit value for precision, 0 is assumed.
 * .*			The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.
 *
 *
 *
 *
 * The length sub-specifier modifies the length of the data type. This is a chart showing the types used to interpret the corresponding arguments with and without length specifier (if a different type is used, the proper type promotion or conversion is performed, if allowed):
 * 											specifiers
 * length		d i				u o x X						f F e E g G a A				c			s			p			n
 * (none)		int				unsigned int				double						int			char*		void*		int*
 * hh			signed char		unsigned char																				signed char*
 * h			short int		unsigned short int																			short int*
 * l			long int		unsigned long int										wint_t		wchar_t*				long int*
 * ll			long long int	unsigned long long int																		long long int*
 * j			intmax_t		uintmax_t																					intmax_t*
 * z			size_t			size_t																						size_t*
 * t			ptrdiff_t		ptrdiff_t																					ptrdiff_t*
 * L														long double
 *
 *
 * Note regarding the c specifier: it takes an int (or wint_t) as argument, but performs the proper conversion to a char value (or a wchar_t) before formatting it for output.
 *
 * Example
 *
 *  UARTprintf example
 *    UARTprintf ("Characters: %c %c \n", 'a', 65);
 *    UARTprintf ("Decimals: %d %ld\n", 1977, 650000L);
 *    UARTprintf ("Preceding with blanks: %10d \n", 1977);
 *    UARTprintf ("Preceding with zeros: %010d \n", 1977);
 *    UARTprintf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
 *    UARTprintf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
 *    UARTprintf ("Width trick: %*d \n", 5, 10);
 *    UARTprintf ("%s \n", "A string");
 *
 * Output:
 *
 *
 * Characters: a A
 * Decimals: 1977 650000
 * Preceding with blanks:       1977
 * Preceding with zeros: 0000001977
 * Some different radices: 100 64 144 0x64 0144
 * floats: 3.14 +3e+000 3.141600E+000
 * Width trick:    10
 * A string
 *
 */
static int string_printf(
		String_t* fp,			/* Pointer to the file object */
		const char* str,	/* Pointer to the format string */
		...					/* Optional arguments... */
)
//int snprintf (Uart_t* UartSettings, char *str,size_t count,const char *fmt,...)
#else
	int snprintf (va_alist) //va_dcl
#endif
{
#ifndef HAVE_STDARGS
	char *str;
	size_t count;
	char *fmt;
#endif
	VA_LOCAL_DECL;

	VA_START (str);
	VA_SHIFT (str, char *);
	VA_SHIFT (count, size_t );
	VA_SHIFT (fmt, char *);
	char buff[1];
	long string_len = str_vsnprintf_(fp, buff, 65535, str, ap);
	VA_END;
	return string_len;//(strlen(str));
}
#endif /* !HAVE_SNPRINTF */

#endif /* !HAVE_SNPRINTF */
#endif

String_t *str_setf(String_t* dest, char* str, ...)
{
	VA_LOCAL_DECL;

	VA_START (str);
	char buff[1];
	str_clear(dest);
	str_vsnprintf_(dest, buff, 65535, str, ap);
	VA_END;
	return dest;
}

String_t *str_appendf(String_t* dest, char* str, ...)
{
	VA_LOCAL_DECL;

	VA_START (str);
	char buff[1];
	//str_clear(dest);
	str_vsnprintf_(dest, buff, 65535, str, ap);
	VA_END;
	return dest;
}


