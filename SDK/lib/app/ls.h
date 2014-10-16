/*
 * ls.h
 *
 *  Created on: Oct 5, 2014
 *      Author: Morgoth
 */

#ifndef LS_H_
#define LS_H_
//#######################################################
#include "lib/fat_fs/inc/ff.h"
//#######################################################
typedef enum{
	LS_OUT_NOT_DEFINED = 0,
	LS_OUT_TO_CONSOLE,
	LS_OUT_TO_LISTBOX,
	LS_OUT_TO_DEVICE
}LS_OUT_TYPE;
//#######################################################
typedef struct{
	int cnt;
    DIR g_sDirObject;
    FILINFO g_sFileInfo;
    FRESULT fresult;
    char shr_name[15];
#if _USE_LFN
	XCHAR lfn_tmp[258];
#endif
}LS_STRUCT;
//#######################################################
int _ls(int argc, char *argv[]);
//#######################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ls.c"
#endif
//#######################################################
#endif /* LS_H_ */
