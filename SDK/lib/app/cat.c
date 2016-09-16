/*
 * cat.c
 *
 *  Created on: Oct 31, 2014
 *      Author: Morgoth
 */

#include <string.h>
#include <stdlib.h>
#include "cat.h"
#include "ffconf.h"		/* FatFs configuration options */
#include "lib/fat_fs/inc/ff.h"
#include "lib/string_lib.h"
#include "app_result.h"
#include "board_init.h"
#include "app_util/path_util.h"
//#######################################################################################
#ifdef FLASH_DEVICE
const unsigned char cat_help_output1[]  PROGMEM =
#else
static const unsigned char cat_help_output1[] =
#endif
{
	"-help: Display list of commands.",
};
//#######################################################################################
int _cat(int argc, char *argv[])
{
    FRESULT fresult;
    //FATFS *pFatFs;
    //DIR g_sDirObject;
    //FILINFO g_sFileInfo;
    CAT_STRUCT *settings;
    settings = (CAT_STRUCT *)argv[0];
	char *response = "";
    if(!argv[0])
    {
    	argv[0] = calloc(1, sizeof(CAT_STRUCT));
    	if(!argv[0]) return (int)&"ERROR\n\r";
        settings = (CAT_STRUCT *)argv[0];
        if(strcmp(argv[2], "-help"));
        {
			char open_mode = FA_OPEN_EXISTING || FA_READ;
			if(argv[3])
			{
				if(!strcmp(argv[3], "r") || !strcmp(argv[3], "R")) open_mode = FA_READ;
				else if(!strcmp(argv[3], "w") || !strcmp(argv[3], "W"))  open_mode = FA_WRITE | FA_CREATE_ALWAYS;
				else if(!strcmp(argv[3], "a") || !strcmp(argv[3], "A"))  open_mode = FA_WRITE;
				else if(!strcmp(argv[3], "r+") || !strcmp(argv[3], "R+"))  open_mode = FA_READ | FA_WRITE | FA_CREATE_NEW;
				else if(!strcmp(argv[3], "w+") || !strcmp(argv[3], "W+"))  open_mode = FA_READ | FA_WRITE | FA_CREATE_ALWAYS;
				else if(!strcmp(argv[3], "a+") || !strcmp(argv[3], "A+"))  open_mode = FA_READ | FA_WRITE | FA_CREATE_ALWAYS;
			}
			string(tmp_arg1, argv[1]);
			string(tmp_arg2, argv[2]);
			path_append_parse(&tmp_arg1, &tmp_arg2);
			fresult = f_open(&settings->g_sFilObject, tmp_arg1.text, open_mode);
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
			else
			{
				settings->file = malloc(settings->g_sFilObject.fsize);
				if(!settings->file)
				{
					settings->fresult = FR_INT_ERR;
					return (int)"ERROR :Out of memory.\n\r";
				}
			}
        }
    }
    if(settings->fresult != FR_OK || settings->cnt == 1)
    {
		if(settings->file) free(settings->file);
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
    	    	return (int)&cat_help_output1;
			case(1):
				if(settings->file) free(settings->file);
				if(argv[0]) free(argv[0]);
				argv[0] = NULL;
				return (int)NULL;
		}
	}
	/*
	** Read an entry from the directory.
	*/
    unsigned long bytes_read = 0;
	fresult = f_read(&settings->g_sFilObject, settings->file, settings->g_sFilObject.fsize, &bytes_read);
	/*
	** Check for error and return if there is a problem.
	*/
	if(fresult != FR_OK)
	{
		if(settings->file) free(settings->file);
		response = "ERROR :Disk ERR.\n\r";
		settings->fresult = fresult;
		return (int)response;
	}
	else if(bytes_read != settings->g_sFilObject.fsize)
	{
		if(settings->file) free(settings->file);
		response = "ERROR :Disk ERR.\n\r";
		settings->fresult = FR_DISK_ERR;
		return (int)response;
	}
	settings->cnt ++;
	//int fname_len = 0;
	//argv[1] = (char *)strreturn;
	argv[4] = (char *)settings->g_sFilObject.fsize;
    return (int)settings->file;
}
