/*
 * ls.c
 *
 *  Created on: Oct 5, 2014
 *      Author: Morgoth
 */

#include <string.h>
#include <stdlib.h>
#include "ls.h"
#include "ffconf.h"		/* FatFs configuration options */
#include "lib/fat_fs/inc/ff.h"
#include "lib/string_lib.h"
#include "app_result.h"
#include "board_init.h"

//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char ls_help_output1[]  PROGMEM =
#else
static const unsigned char ls_help_output1[] =
#endif
{
	"-help: Display list of commands.",
};
//#######################################################################################
int _ls(int argc, char *argv[])
{
    FRESULT fresult;
    //FATFS *pFatFs;
    //DIR g_sDirObject;
    //FILINFO g_sFileInfo;
    LS_STRUCT *settings;
    settings = (LS_STRUCT *)argv[0];
	char *response = "";
    if(!argv[0])
    {
    	argv[0] = calloc(1, sizeof(LS_STRUCT));
    	if(!argv[0]) return (int)&"ERROR\n\r";
        settings = (LS_STRUCT *)argv[0];
        if(!strcmp(argv[1], "-help"));
        else
        {
#if _USE_LFN
        	settings->g_sFileInfo.lfname = settings->lfn_tmp;
			settings->g_sFileInfo.lfsize = 255;
#endif
			fresult = f_opendir(&settings->g_sDirObject, argv[1]);
			settings->fresult = fresult;
			/*
			** Check for error and return if there is a problem.
			*/
			if(fresult != FR_OK)
			{
				//free(tmp);
				switch(fresult)
				{
					case(FR_DISK_ERR):
							response = "ERROR :Disk ERR.\n\r";
					break;
					case(FR_INVALID_DRIVE):
							response = "ERROR :Invalid drive.\n\r";
							break;
					case(FR_NO_FILESYSTEM):
							response = "ERROR :No file system.\n\r";
							break;
					case(FR_TIMEOUT):
							response = "ERROR :Timeout.\n\r";
							break;
					case(FR_NO_PATH):
							response = "ERROR :No path.\n\r";
						break;
					default:
							response = "ERROR :Unknown ERR.\n\r";
							break;
				}
				return (int)response;
			}
        }
    }
    if(settings->fresult != FR_OK)
    {
		if(argv[0]) free(argv[0]);
		argv[0] = NULL;
		return (int)NULL;
    }
    if(!strcmp(argv[1], "-help"))
	{
		switch(settings->cnt)
		{
			case(0):
				settings->cnt++;
    	    	return (int)&ls_help_output1;
			case(1):
				if(argv[0]) free(argv[0]);
				argv[0] = NULL;
				return (int)NULL;
		}
	}
	/*char **tmp_path;
	tmp_path = (char **)argv[arg_cnt];
	char *tmp = calloc(1, strlen(tmp_path[0]) + 1);
	strcpy(tmp, tmp_path[0]);
	//tmp = str_copy(tmp_path[0]);
	tmp = path_append_parse(tmp, (char *)argv[arg_cnt + 1]);*/
	/*
	** Open the current directory for access.
	*/
//#if _USE_LFN
	//XCHAR lfn_tmp[258];
//	g_sFileInfo.lfname = lfn_tmp;
//	g_sFileInfo.lfsize = 256;
//#endif
	//free(tmp_path[0]);
	//tmp_path[0] = tmp;
	//arg_cnt += 2;
	//STR_RESULT str_status = STR_OK;
	//char **str_array = (char **)argv[1];
	//new_ (new_string_array);
	//char *strreturn = calloc(1,1);
	//char shortname[15];
	/*
	** Read an entry from the directory.
	*/
	fresult = f_readdir(&settings->g_sDirObject, &settings->g_sFileInfo);
	/*
	** Check for error and return if there is a problem.
	*/
	if(fresult != FR_OK)
	{
		response = "ERROR :Disk ERR.\n\r";
		settings->fresult = fresult;
		return (int)response;
	}
	/*
	** If the file name is blank, then this is the end of the listing.
	*/
	if(!settings->g_sFileInfo.fname[0])
	{
		if(argv[0]) free(argv[0]);
		argv[0] = NULL;
		return (int)NULL;
	}
	//int fname_len = 0;
#if _USE_LFN
	if(strlen(settings->g_sFileInfo.lfname))
	{
		strcat(settings->g_sFileInfo.lfname, "\n\r");
		response = settings->g_sFileInfo.lfname;
	}
	else
#endif
	{
		strcpy(settings->shr_name, settings->g_sFileInfo.fname);
		strcat(settings->shr_name, "\n\r");
		response = settings->shr_name;
	}
	//argv[1] = (char *)strreturn;
    return (int)response;
}
