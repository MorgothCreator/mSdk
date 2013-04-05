/*
 * int_twi.h
 *
 * Created: 2/8/2013 5:13:36 PM
 *  Author: XxXx
 */ 
//#####################################################
#ifndef INT_TWI_H_
#define INT_TWI_H_
//#####################################################
#include <avr/io.h>
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
typedef struct
{
#ifdef use_twi0
	Twi_t* Twi0;
#endif
#ifdef use_twi1
	Twi_t* Twi1;
#endif
#ifdef use_twi2
	Twi_t* Twi2;
#endif
#ifdef use_twi3
	Twi_t* Twi3;
#endif
}Twi_Int_Service_t;
//#####################################################
extern Twi_Int_Service_t TWI_ServiceAddresses;
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "int_twi.c"
#endif
//#####################################################
#endif /* INT_TWI_H_ */
//#####################################################
