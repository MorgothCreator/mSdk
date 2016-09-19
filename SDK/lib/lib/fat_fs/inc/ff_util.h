/*
 * ff_util.h
 *
 *  Created on: Sep 4, 2016
 *      Author: John Smith
 */

#ifndef LIB_LIB_FAT_FS_INC_FF_UTIL_H_
#define LIB_LIB_FAT_FS_INC_FF_UTIL_H_

#include "../inc/ff.h"

void ff_util_reset_chk();
unsigned long ff_util_get_chk();

int ff_util_gets (
	TCHAR* buff,	/* Pointer to the string buffer to read */
	int len,		/* Size of string buffer (characters) */
	FIL* fp			/* Pointer to the file object */
);

int ff_util_putc (
	TCHAR c,	/* A character to be output */
	FIL* fp		/* Pointer to the file object */
);

int ff_util_puts (
	const TCHAR* str,	/* Pointer to the string to be output */
	FIL* fp				/* Pointer to the file object */
);

int ff_util_appendhexs (
	const unsigned char* buff,	/* Pointer to the buffer to be output */
	const unsigned int buff_len,/* Number of bytes to be output */
	FIL* fp				/* Pointer to the file object */
);

int ff_util_put_nextline (
	FIL* fp				/* Pointer to the file object */
);

int ff_util_printf(
		FIL* fp,			/* Pointer to the file object */
		const TCHAR* str,	/* Pointer to the format string */
		...					/* Optional arguments... */
);

FRESULT ff_util_seek (
	FIL* fp,				/* Pointer to the file object */
	unsigned int location
);

FRESULT ff_util_seek_eof (
	FIL* fp				/* Pointer to the file object */
);

FRESULT ff_util_file_exist (
		const TCHAR* str	/* Pointer to the string to be output */
);

#endif /* LIB_LIB_FAT_FS_INC_FF_UTIL_H_ */
