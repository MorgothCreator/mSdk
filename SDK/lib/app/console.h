/*
 * console.h
 *
 *  Created on: Oct 7, 2014
 *      Author: Morgoth
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_
//#######################################################
#define CONSOLE_ARGS	2
#define new_console(name) char *name[CONSOLE_ARGS] = {NULL, NULL}
#define console_arg(name) CONSOLE_ARGS, name
//#######################################################
typedef enum{
	CONSOLE_CAT_NONE = 0,
	CONSOLE_CAT_PATH
}CONSOLE_CATEGORY;
//#######################################################
typedef struct{
	char name[13];
	int (*console)(int argc, char *argv[]);
	int arg_nr;
	int category;
}CONSOLE_APP_LIST;
//#######################################################
typedef struct{
	int cnt;
	int rx_cnt;
	char *rx_data;
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
