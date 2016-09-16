/*
 * crc7.h
 *
 *  Created on: Sep 4, 2016
 *      Author: John Smith
 */

#ifndef LIB_UTIL_CRC7_H_
#define LIB_UTIL_CRC7_H_

typedef struct crc7_s
{
	unsigned char CRCPoly;
	unsigned char *CRCTable;
}crc7_t;
//#####################################################
#define new_crc_7 crc7_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_crc_7(address) free(address);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "crc7.c"
#endif

#endif /* LIB_UTIL_CRC7_H_ */
