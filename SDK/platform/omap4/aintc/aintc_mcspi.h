/*
 * aintc_mcspi.h
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef AINTC_MCSPI_H_
#define AINTC_MCSPI_H_
/*#####################################################*/
#include "api/mcspi_def.h"
//#include "include/mcspi.h"
/*#####################################################*/
typedef struct
{
	Mcspi_t* Mcspi0;
	Mcspi_t* Mcspi1;
	Mcspi_t* Mcspi2;
	Mcspi_t* Mcspi3;
}Mcspi_Int_Service_t;
/*#####################################################*/
void McSPIAintcConfigure(unsigned char instance);
void McSPIAintcUnConfigure(unsigned char instance);
/*#####################################################*/
extern Mcspi_Int_Service_t Mcspi_Int_Service;
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "aintc_mcspi.c"
#endif
/*#####################################################*/
#endif /* AINTC_MCSPI_H_ */
/*#####################################################*/
