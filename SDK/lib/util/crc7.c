/*
 * crc7.c
 *
 *  Created on: Sep 4, 2016
 *      Author: John Smith
 */

#include <stdlib.h>
#include "crc7.h"

void GenerateCRC7Table(crc7_t *param, int length)
{
    int i, j;

    // generate a table value for all 256 possible byte values
    for (i = 0; i < 256; i++)
    {
    	param->CRCTable[i] = (i & 0x80) ? i ^ param->CRCPoly : i;
        for (j = 1; j < 8; j++)
        {
        	param->CRCTable[i] <<= 1;
            if (param->CRCTable[i] & 0x80)
            	param->CRCTable[i] ^= param->CRCPoly;
        }
    }
}


// adds a message byte to the current CRC-7 to get a the new CRC-7
unsigned char CRC7Add(crc7_t *param, unsigned char CRC, unsigned char message_byte)
{
    return param->CRCTable[(CRC << 1) ^ message_byte];
}


// returns the CRC-7 for a message of "length" bytes
unsigned char getCRC7(crc7_t *param, unsigned char message[], int length)
{
	param->CRCTable = (unsigned char *)malloc(length);
	if(!param->CRCTable)
		return 0;
	GenerateCRC7Table(param, length);
    int i;
    unsigned char CRC = 0;
    param->CRCPoly = 0x89;// the value of our CRC-7 polynomial

    for (i = 0; i < length; i++)
        CRC = CRC7Add(param, CRC, message[i]);
	if(param->CRCTable)
		free(param->CRCTable);
    return CRC;
}
