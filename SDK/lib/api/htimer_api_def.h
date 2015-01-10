/*
 * htimer_api_def.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Iulian Gheorghiu
 */
/*#####################################################*/
#ifndef HTIMER_API_DEF_H_
#define HTIMER_API_DEF_H_
/*#####################################################*/
#include "interface/htimer_interface_def.h"
/*#####################################################*/
typedef struct Htimer_s {

}Htimer_t;
/*#####################################################*/
#define new_htimer Htimer_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_htimer(address) free(address);
/*#####################################################*/
#endif /* HTIMER_API_DEF_H_ */
/*#####################################################*/
