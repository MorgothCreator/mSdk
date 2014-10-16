/*
 * console.c
 *
 *  Created on: Oct 7, 2014
 *      Author: Morgoth
 */

#include <string.h>
#include <stdlib.h>
#include "console.h"
#include "cd.h"
#include "ls.h"
#include "lib/string_lib.h"
#include "api/uart_api.h"
#include "app_result.h"
#include "app_util/path_util.h"
#include "board_init.h"

//extern new_uart* ConsoleCom;
//static char *console_rx_data;
//#######################################################################################
#ifdef FLASH_DEVICE
const CONSOLE_APP_LIST console_app_list[]  PROGMEM =
#else
static const CONSOLE_APP_LIST console_app_list[] =
#endif
{
		{"ls", _ls, 2, CONSOLE_CAT_PATH},
		{"cd", _cd, 2, CONSOLE_CAT_NONE}
};
//#######################################################################################
static void console_clear_child_data(CONSOLE_STRUCT *settings)
{
	if(settings->child_arg[1]) free(settings->child_arg[1]);
	if(settings->child_arg) free(settings->child_arg);
	settings->child_arg = NULL;
	settings->child_arg_nr = 0;
	settings->app = NULL;
}
//#######################################################################################
static void console_start_end_arg(CONSOLE_STERT_END_ARG * start_end_arg, char* str)
{
	char *a = str;
	while(*a == ' ' && *a != 0) a++;
	//if(*a == 0) break;
	start_end_arg->start = a;
	while(*a != ' ' && *a != 0) a++;
	start_end_arg->end = a;
}
/*
 * arg[0] = input uart settings
 * arg[1] = rx buff
 * arg[2] = path buff
 */
int console(int argc, char *argv[])
{
	//if(argc != 3 || !argv[0]) return;
	CONSOLE_STRUCT *settings;
 	if(!argv[0])
	{
		argv[0] = calloc(1, sizeof(CONSOLE_STRUCT));
		settings = (CONSOLE_STRUCT *)argv[0];
		settings->path = malloc(2);
		settings->rx_data = calloc(1, 1);
		strcpy(settings->path, "/");
		//argv[2] = malloc(sizeof(char *));
	}
	else
	{
		settings = (CONSOLE_STRUCT *)argv[0];
		int rx_c = (int)argv[1];
		argv[1] = NULL;
	    if(rx_c >= 0)
	    {
	    	char _a_[2];
	    	_a_[0] = rx_c;
	    	_a_[1] = 0;
	    	if(rx_c != '\n' && rx_c != '\r') settings->rx_data = str_append(settings->rx_data, _a_);
	    	else
	    	{
	    		char *a = settings->rx_data;
	    		int a_len = 0;
	    		while((a_len = strlen(a)) != 0)
	    		{
		    		int app_list_cnt = sizeof(console_app_list) / sizeof(CONSOLE_APP_LIST);
	    			while(*a == ' ' && *a != 0) a++;
	    			if(*a == 0) break;
	    			while(--app_list_cnt >= 0 && memcmp(a, console_app_list[app_list_cnt].name, strlen(console_app_list[app_list_cnt].name)) != 0);
	    			if(app_list_cnt >= 0)
	    			{
						a += strlen(console_app_list[app_list_cnt].name);
						CONSOLE_STERT_END_ARG erg_start_end;
						console_start_end_arg(&erg_start_end, a);
						int arg_len = (int)(erg_start_end.end - erg_start_end.start);
						char *argument = calloc(1, arg_len + 1);
						strncat(argument, erg_start_end.start, arg_len);
						a = erg_start_end.end;
	    				if(console_app_list[app_list_cnt].console == _cd)
	    				{
	    				    char *cd_arg[2];
	    				    cd_arg[0] = settings->path;
	    				    cd_arg[1] = argument;
	    				    char *cd_result = (char *)_cd(2, cd_arg);
	    				    if(!memcmp(cd_result, "ERROR", 5)) UARTprintf(DebugCom, "%s" , cd_result);
	    				    else
	    				    {
	    				    	if(settings->path) free(settings->path);
	    				    	settings->path = cd_result;
	    				    }
	    				}
	    				else
	    				{
							if(console_app_list[app_list_cnt].category == CONSOLE_CAT_PATH)
							{
								char *tmp_path;
								tmp_path = settings->path;
								char *tmp = calloc(1, strlen(tmp_path) + 1);
								strcpy(tmp, tmp_path);
								//tmp = str_copy(tmp_path[0]);
								tmp = path_append_parse(tmp, argument);

								settings->child_arg = malloc(sizeof(char **) * console_app_list[app_list_cnt].arg_nr);
								settings->child_arg[0] = NULL;
								settings->child_arg[1] = tmp;
								settings->child_arg_nr = console_app_list[app_list_cnt].arg_nr;
								settings->app = console_app_list[app_list_cnt].console;
							}
	    				}
	    			} else a++;
	    		}
	    		settings->rx_data = str_clear(settings->rx_data);
	    	}
	    }
		if(settings->app)
		{
			char* result;
			if((result = (char *)settings->app(settings->child_arg_nr, settings->child_arg)))
			{
				UARTprintf(DebugCom, "%s" , result);
			}
			else
			{
				console_clear_child_data(settings);
			}
		}
	}
 	return 0;
}

