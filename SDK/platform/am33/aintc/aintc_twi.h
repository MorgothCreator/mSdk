/*
 * aintc_twi.h
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef AINTC_TWI_H_
#define AINTC_TWI_H_
/*#####################################################*/
#include "stdbool.h"
#include "api/twi_def.h"
/*#####################################################*/
typedef struct
{
	Twi_t* Twi0;
	Twi_t* Twi1;
	Twi_t* Twi2;
}Twi_Int_Service_t;
/*#####################################################*/
void I2CAINTCConfigure(new_twi* TwiStruct);
void I2CAINTCUnConfigure(new_twi* TwiStruct);
void CleanUpInterrupts(new_twi* TwiStruct);
/*#####################################################*/
extern Twi_Int_Service_t Twi_Int_Service;
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "aintc_twi.c"
#endif
/*#####################################################*/
#endif /* AINTC_TWI_H_ */
/*#####################################################*/
