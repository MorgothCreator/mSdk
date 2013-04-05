/*
 * sysdelay.h
 *
 *  Created on: Jul 21, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef SYSDELAY_H_
#define SYSDELAY_H_
/*#####################################################*/
//#include "Wdt.h"
/*#####################################################*/
void SysDelayTimerSetup(void);
void Sysdelay(unsigned int milliSec);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "sysdelay.c"
#endif
/*#####################################################*/
#endif /* SYSDELAY_H_ */
/*#####################################################*/
