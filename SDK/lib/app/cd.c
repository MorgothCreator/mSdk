/*
 * cd.c
 *
 *  Created on: Oct 6, 2014
 *      Author: Morgoth
 */

#include <string.h>
#include <stdlib.h>
#include "cd.h"
#include "lib/fat_fs/inc/ff.h"
#include "lib/string_lib.h"
#include "app_result.h"
#include "app_util/path_util.h"


int _cd(int argc, char *argv[])
{
    FRESULT fresult;
    //FATFS *pFatFs;
    DIR g_sDirObject;
    //FILINFO g_sFileInfo;
	char *tmp_path;
	tmp_path = (char *)argv[0];
	char *tmp = calloc(1, strlen(tmp_path) + 1);
	strcpy(tmp, tmp_path);
	//tmp = str_copy(tmp_path[0]);
	tmp = path_append_parse(tmp, (char *)argv[1]);

	fresult = f_opendir(&g_sDirObject, tmp);
	/*
	** Check for error and return if there is a problem.
	*/
	if(fresult != FR_OK)
	{
		//free(tmp);
		switch(fresult)
		{
			case(FR_DISK_ERR):
				return (int)"ERROR :Disk ERR.\n\r";
			case(FR_INVALID_DRIVE):
				return (int)"ERROR :Invalid drive.\n\r";
			case(FR_NO_FILESYSTEM):
				return (int)"ERROR :No file system.\n\r";
			case(FR_TIMEOUT):
				return (int)"ERROR :Timeout.\n\r";
			case(FR_NO_PATH):
				return (int)"ERROR :No path.\n\r";
			default:
				return (int)"ERROR :Unknown ERR.\n\r";
		}
	}
	return (int)tmp;
}
