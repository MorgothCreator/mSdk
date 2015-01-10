/*
 * htimer_api.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#ifndef HTIMER_API_H_
#define HTIMER_API_H_
#include "stdbool.h"
#include "api/htimer_api_def.h"
/*#####################################################*/
bool htimer_init(Htimer_t *structure);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "htimer_api.c"
#endif
/*#####################################################*/
#endif /* HTIMER_API_H_ */
/*#####################################################*/
