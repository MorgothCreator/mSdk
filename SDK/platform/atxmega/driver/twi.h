/*
 * twi.h
 *
 * Created: 2/8/2013 5:10:16 PM
 *  Author: XxXx
 */ 
//#####################################################
#ifndef TWI_H_
#define TWI_H_
//#####################################################
#include <avr/io.h>
#include "api/twi_def.h"
//#####################################################
#ifndef TWI_Int_Disabled
#define TWI_Int_Disabled			(0)
#endif
#ifndef TWI_Int_Low_Level
#define TWI_Int_Low_Level			(1)
#endif
#ifndef TWI_Int_Med_Level
#define TWI_Int_Med_Level			(2)
#endif
#ifndef TWI_Int_High_Level
#define TWI_Int_High_Level			(3)
#endif
//#####################################################
bool TWI_open(Twi_t* TwiStruct);
bool TWI_reset(Twi_t* TwiStruct);
void TWI_close(Twi_t *twi);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "twi.c"
#endif
//#####################################################
#endif /* TWI_H_ */
//#####################################################
