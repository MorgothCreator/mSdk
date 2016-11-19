/*
 * ff_util.c
 *
 *  Created on: Sep 4, 2016
 *      Author: John Smith
 */

#include "../inc/ff_util.h"
//#include "../inc/ff.h"			/* FatFs configurations and declarations */
#include "lib/util/hex_string.h"

unsigned long checksum_send;


void ff_util_reset_chk()
{
	checksum_send = (unsigned long) -1;
}
unsigned long ff_util_get_chk()
{
	return checksum_send;
}

/*-----------------------------------------------------------------------*/
/* Write stream to the file the file                                            */
/*-----------------------------------------------------------------------*/
FRESULT ff_util_write (
	FIL* fp,			/* Pointer to the file object */
	const void* buff,	/* Pointer to the data to be written */
	UINT btw,			/* Number of bytes to write */
	UINT* bw			/* Pointer to number of bytes written */
)
{
	unsigned int cnt = 0;
	unsigned char *_buff = (unsigned char *)buff;
	for(; cnt < btw; cnt++)
		checksum_send -= _buff[cnt];
	return f_write(fp, buff, btw, bw);
}
/*-----------------------------------------------------------------------*/
/* Get a string from the file                                            */
/*-----------------------------------------------------------------------*/
int ff_util_gets (
	TCHAR* buff,	/* Pointer to the string buffer to read */
	int len,		/* Size of string buffer (characters) */
	FIL* fp			/* Pointer to the file object */
)
{
	TCHAR* res = f_gets(buff, len, fp);
	if(res)
		return *res;
	else
		return -1;
}
/*-----------------------------------------------------------------------*/
/* Append a character to the file                                           */
/*-----------------------------------------------------------------------*/
int ff_util_appendc (
	TCHAR c,	/* A character to be output */
	FIL* fp		/* Pointer to the file object */
)
{
	checksum_send -= c;
	if(f_size(fp) != 0)
		ff_util_seek_eof(fp);
	return f_putc(c, fp);
}
/*-----------------------------------------------------------------------*/
/* Put a character to the file                                           */
/*-----------------------------------------------------------------------*/
int ff_util_putc (
	TCHAR c,	/* A character to be output */
	FIL* fp		/* Pointer to the file object */
)
{
	checksum_send -= c;
	return f_putc(c, fp);
}
/*-----------------------------------------------------------------------*/
/* Put a string to file                                              */
/*-----------------------------------------------------------------------*/
int ff_util_appends (
	const TCHAR* str,	/* Pointer to the string to be output */
	FIL* fp				/* Pointer to the file object */
)
{
	if(f_size(fp) != 0)
		ff_util_seek_eof(fp);
	return ff_util_puts(str, fp);
}
/*-----------------------------------------------------------------------*/
/* Put a string to file                                              */
/*-----------------------------------------------------------------------*/
int ff_util_puts (
	const TCHAR* str,	/* Pointer to the string to be output */
	FIL* fp				/* Pointer to the file object */
)
{
	int n;


	for (n = 0; *str; str++, n++) {
		if (ff_util_putc(*str, fp) == EOF)
			return EOF;
	}
	if(f_sync(fp) != FR_OK)
		return 0;
	return n;
}

/*-----------------------------------------------------------------------*/
/* Put a hex buff to file                                              */
/*-----------------------------------------------------------------------*/
int ff_util_appendhexs (
	const unsigned char* buff,	/* Pointer to the buffer to be output */
	const unsigned int buff_len,/* Number of bytes to be output */
	FIL* fp				/* Pointer to the file object */
)
{
	unsigned int cnt = 0;
	char tmp_str_buff[65];
	for(; cnt + 32 <= buff_len; cnt += 32)
	{
		GetHexBuff(tmp_str_buff, (unsigned char *)buff + cnt, 32);
		if(ff_util_puts(tmp_str_buff, fp) != 64)
			return cnt;
	}
	if((signed int)cnt < (signed int)buff_len)
	{
		GetHexBuff(tmp_str_buff, (unsigned char *)buff + cnt, buff_len - cnt);
		if(ff_util_puts(tmp_str_buff, fp) != (buff_len - cnt) * 2)
			return cnt;
	}
	return cnt;//f_puts(str, fp);
}
/*-----------------------------------------------------------------------*/
/* Put a next line                                                       */
/*-----------------------------------------------------------------------*/
int ff_util_put_nextline (
	FIL* fp				/* Pointer to the file object */
)
{
	char str[3];
	str[0] = '\r';
	str[1] = '\n';
	str[2] = '\0';
	return ff_util_puts(str, fp);
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
int ff_vsnprintf_ (FIL* fp, char *str, size_t count, const char *fmt, va_list arg);// Renamed to avoid conflict with builtin 'vsnprintf' function.

static void dopr (FIL* fp, char *buffer, size_t maxlen, const char *format,
                  va_list args);
static void fmtstr (FIL* fp, char *buffer, size_t *currlen, size_t maxlen,
		    char *value, int flags, int min, int max);
static void fmtint (FIL* fp, char *buffer, size_t *currlen, size_t maxlen,
		    long value, int base, int min, int max, int flags);
static void fmtfp (FIL* fp, char *buffer, size_t *currlen, size_t maxlen,
		   LDOUBLE fvalue, int min, int max, int flags);
static void dopr_outch (FIL* fp, char *buffer, size_t *currlen, size_t maxlen, char c );

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

static void dopr (FIL* fp, char *buffer, size_t maxlen, const char *format, va_list args)
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

static void fmtstr (FIL* fp, char *buffer, size_t *currlen, size_t maxlen,
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

static void fmtint (FIL* fp, char *buffer, size_t *currlen, size_t maxlen,
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

static void fmtfp (FIL* fp, char *buffer, size_t *currlen, size_t maxlen,
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

static void dopr_outch (FIL* fp, char *buffer, size_t *currlen, size_t maxlen, char c)
{
  /*if (*currlen < maxlen)
    buffer[(*currlen)++] = c;*/
	ff_util_appendc(c, fp);
}

#ifndef HAVE_VSNPRINTF
int ff_vsnprintf_ (FIL* fp, char *str, size_t count, const char *fmt, va_list args)
{
  str[0] = 0;
  dopr(fp, str, count, fmt, args);
  return(strlen(str));
}
#endif /* !HAVE_VSNPRINTF */




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
int ff_util_printf(
		FIL* fp,			/* Pointer to the file object */
		const TCHAR* str,	/* Pointer to the format string */
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
	long string_len = ff_vsnprintf_(fp, buff, 65535, str, ap);
	VA_END;
	if(f_sync(fp) != FR_OK)
		return 0;
	return string_len;//(strlen(str));
}
#endif /* !HAVE_SNPRINTF */

#endif /* !HAVE_SNPRINTF */

/*-----------------------------------------------------------------------*/
/* Seek                                                       */
/*-----------------------------------------------------------------------*/
FRESULT ff_util_seek (
	FIL* fp,				/* Pointer to the file object */
	unsigned int location
)
{
	return f_lseek(fp, location);
}
/*-----------------------------------------------------------------------*/
/* Seek to end of file                                                       */
/*-----------------------------------------------------------------------*/
FRESULT ff_util_seek_eof (
	FIL* fp				/* Pointer to the file object */
)
{
	return f_lseek(fp, f_size(fp));
}
/*-----------------------------------------------------------------------*/
/* Check if file exist                                                   */
/*-----------------------------------------------------------------------*/
FRESULT ff_util_file_exist (
		const TCHAR* str	/* Pointer to the string to be output */
)
{
	FIL fp;				/* Pointer to the file object */
	memset(&fp, 0, sizeof(FIL));
	FRESULT res = f_open(&fp, str, FA_READ);
	if(res == FR_OK)
	{
	    f_close(&fp);
	}
	return res;
}
