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
#include "cat.h"
#include "lib/string_lib.h"
#include "api/uart_api.h"
#include "app_result.h"
#include "app_util/path_util.h"
#include "board_init.h"

extern new_uart* DebugCom;
//extern new_uart* ConsoleCom;
//static char *console_rx_data;
//#######################################################################################
#ifdef FLASH_DEVICE
const CONSOLE_APP_LIST console_app_list[]  PROGMEM =
#else
static const CONSOLE_APP_LIST console_app_list[] =
#endif
{
		{"ls"	, _ls	, 1, CONSOLE_CAT_PATH,	CONSOLE_LS,		0/*,	(char *)(&"Usage : ls<path>, ls"),				(char *)(&"Usage : ls <path>, ls")*/},
		{"cat"	, _cat	, 3, CONSOLE_CAT_PATH,	CONSOLE_CAT,	1/*,	(char *)(&"Usage : cat<file> <open mode>"),		(char *)(&"Usage : cat<file> <open mode>")*/},
		{"cd"	, _cd	, 1, CONSOLE_CAT_NONE, CONSOLE_CD,		0/*,	(char *)(&"Usage : cd<path>"),					(char *)(&"Usage : cd<path>")*/}
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
static bool console_start_end_arg(CONSOLE_STERT_END_ARG * start_end_arg, char* str)
{
	char *a = str;
	while(*a == ' ' && *a != 0) a++;
	//if(*a == 0) break;
	start_end_arg->start = a;
	while(*a != ' ' && *a != 0) a++;
	start_end_arg->end = a;
	if(start_end_arg->end != start_end_arg->start) return true;
	else return false;
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
		settings->rx_data = new_(new_string);
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
	    	if(rx_c != '\n' && rx_c != '\r')
	    		str_paste(settings->rx_data, _a_);
	    	else
	    	{
	    		char *a = settings->rx_data->text;
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
	    				if(console_app_list[app_list_cnt].console == _cd)
	    				{
							CONSOLE_STERT_END_ARG arg_start_end;
							console_start_end_arg(&arg_start_end, a);
							int arg_len = (int)(arg_start_end.end - arg_start_end.start);
							char *argument = calloc(1, arg_len + 1);
							strncat(argument, arg_start_end.start, arg_len);
							a = arg_start_end.end;
	    				    char *cd_arg[2];
	    				    cd_arg[0] = settings->path;
	    				    cd_arg[1] = argument;
	    				    char *cd_result = (char *)_cd(2, cd_arg);
	    				    if(!memcmp(cd_result, "ERROR", 5)) uart.printf(DebugCom, "%s" , cd_result);
	    				    else
	    				    {
	    				    	if(settings->path) free(settings->path);
	    				    	settings->path = cd_result;
	    				    }
	    				    uart.puts(DebugCom, settings->path, -1);
	    				    uart.puts(DebugCom, "\n\r", -1);
	    				}
	    				else
	    				{
							CONSOLE_STERT_END_ARG arg_start_end;
							switch((int)console_app_list[app_list_cnt].order)
							{
								case(CONSOLE_LS):
								case(CONSOLE_CAT):
									settings->child_arg = calloc(1, sizeof(char **) * (console_app_list[app_list_cnt].arg_nr + 2));
									settings->child_arg[0] = NULL;
									settings->child_arg[1] = settings->path;
									console_start_end_arg(&arg_start_end, a);
									int arg_nr_cnt = console_app_list[app_list_cnt].arg_nr;
									if(arg_nr_cnt)
									{
										/*char *argument = calloc(1, arg_len + 1);
										strncat(argument, arg_start_end.start, arg_len);
										a = arg_start_end.end;
										char *tmp_path;
										tmp_path = settings->path;
										char *tmp = calloc(1, strlen(tmp_path) + 1);
										strcpy(tmp, tmp_path);
										//tmp = str_copy(tmp_path[0]);
										tmp = path_append_parse(tmp, argument);
										settings->child_arg[1] = tmp;*/
										while(console_start_end_arg(&arg_start_end, a) && arg_nr_cnt != 0)
										{
											int arg_len = (int)(arg_start_end.end - arg_start_end.start);
											settings->child_arg[console_app_list[app_list_cnt].arg_nr - arg_nr_cnt] = malloc(arg_len + 1);
											strncpy(settings->child_arg[console_app_list[app_list_cnt].arg_nr - arg_nr_cnt], arg_start_end.start ,arg_len);
											settings->child_arg[console_app_list[app_list_cnt].arg_nr - arg_nr_cnt][arg_len] = 0;
											arg_nr_cnt--;
											a = arg_start_end.end;
										}
									}
									/*if(console_app_list[app_list_cnt].arg_nr - console_app_list[app_list_cnt].res_args == console_app_list[app_list_cnt].arg_nr - arg_nr_cnt)
									{*/
										settings->child_arg[0] = NULL;
										settings->child_arg_nr = console_app_list[app_list_cnt].arg_nr - arg_nr_cnt/*console_app_list[app_list_cnt].arg_nr*/;
										settings->app = console_app_list[app_list_cnt].console;
										settings->app_order = console_app_list[app_list_cnt].order;
				    				    if(console_app_list[app_list_cnt].order == CONSOLE_CAT)
				    				    {
											uart.puts(DebugCom, settings->path, -1);
											uart.puts(DebugCom, "\n\r", -1);
				    				    }
									/*} else
									{
										if(console_app_list[app_list_cnt].arg_nr - console_app_list[app_list_cnt].res_args > console_app_list[app_list_cnt].arg_nr - arg_nr_cnt)
										{
											uart.puts(DebugCom, console_app_list[app_list_cnt].help_many_args, -1);
											uart.puts(DebugCom, "\n\r", -1);
										}
										else
										{
											uart.puts(DebugCom, console_app_list[app_list_cnt].help_few_args, -1);
											uart.puts(DebugCom, "\n\r", -1);
										}
										console_clear_child_data(settings);
									}*/
									break;
							}
	    				}
	    			} else a++;
	    		}
	    		str_clear(settings->rx_data);
	    	}
	    }
		if(settings->app)
		{
			char* result;
			if((result = (char *)settings->app(settings->child_arg_nr, settings->child_arg)))
			{
				switch((int)settings->app_order)
				{
					case(CONSOLE_LS):
					case(CONSOLE_CD):
						uart.printf(DebugCom, "%s" , result);
						break;
					case(CONSOLE_CAT):
						if(!memcmp(result, "ERROR", 5)) uart.puts(DebugCom, result, -1);
						else	uart.puts(DebugCom, result, (int)settings->child_arg[4]);
						break;
				}
			}
			else
			{
			    if(settings->app == _ls)
			    {
			    	uart.puts(DebugCom, settings->path, -1);
			    	uart.puts(DebugCom, "\n\r", -1);
			    }
				console_clear_child_data(settings);
			}
		}
	}
 	return 0;
}

