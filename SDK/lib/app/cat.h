/*
 * cat.h
 *
 *  Created on: Oct 31, 2014
 *      Author: Morgoth
 */

#ifndef CAT_H_
#define CAT_H_
//#######################################################
#include "lib/fat_fs/inc/ff.h"
//#######################################################
typedef struct{
	int cnt;
    FIL g_sFilObject;
    FILINFO g_sFileInfo;
    FRESULT fresult;
    char *file;
}CAT_STRUCT;
//#######################################################
int _cat(int argc, char *argv[]);
//#######################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "cat.c"
#endif
//#######################################################
#endif /* CAT_H_ */
