/*
 * console.h
 *
 *  Created on: Oct 7, 2014
 *      Author: Morgoth
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "lib/string_lib.h"
//#######################################################
#define CONSOLE_ARGS	2
#define new_console(name) char *name[CONSOLE_ARGS] = {NULL, NULL}
#define console_arg(name) CONSOLE_ARGS, name
//#######################################################
#define CONSOLE_MAX_ARGS	16
//#######################################################
typedef enum{
	CONSOLE_CAT_NONE = 0,
	CONSOLE_CAT_PATH
}CONSOLE_CATEGORY;
//#######################################################
typedef enum{
	CONSOLE_NONE = 0,
	CONSOLE_LS,
	CONSOLE_CAT,
	CONSOLE_CD
}CONSOLE_ORDER;
//#######################################################
typedef struct{
	char name[13];
	int (*console)(int argc, char *argv[]);
	int arg_nr;
	CONSOLE_CATEGORY category;
	CONSOLE_ORDER order;
	int res_args;
	//char *help_few_args;
	//char *help_many_args;
}CONSOLE_APP_LIST;
//#######################################################
typedef struct{
	int cnt;
	int rx_cnt;
	int app_order;
	String_t *rx_data;
    char *path;
    int child_arg_nr;
    char **child_arg;
    int (*app)(int argc, char *argv[]);
}CONSOLE_STRUCT;
//#######################################################
typedef struct{
	char *start;
	char *end;
}CONSOLE_STERT_END_ARG;
//#######################################################
int console(int argc, char *argv[]);
//#######################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "console.c"
#endif
//#######################################################
#endif /* CONSOLE_H_ */
