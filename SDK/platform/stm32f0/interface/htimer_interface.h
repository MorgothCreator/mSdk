/*
 * htimer_interface.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#ifndef HTIMER_INTERFACE_H_
#define HTIMER_INTERFACE_H_
/*#####################################################*/
#include "stdbool.h"
#include "api/htimer_api_def.h"
/*#####################################################*/
bool htimer_init(Htimer_t *structure);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "htimer_interface.c"
#endif
/*#####################################################*/
#endif /* HTIMER_INTERFACE_H_ */
/*#####################################################*/
