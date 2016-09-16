/*
 * hex_string.c
 *
 *  Created on: Sep 11, 2016
 *      Author: John Smith
 */

#include "hex_string.h"

void GetHexChar(char *hex_str, unsigned char data)
{
	char char1 = (data >> 4) & 0x0F;
	char char2 = data & 0x0F;
	if(char1 <= 9)
		hex_str[0] = (char1 + '0');
	else
		hex_str[0] = ((char1 - 10) + 'A');
	if(char2 <= 9)
		hex_str[1] = (char2 + '0');
	else
		hex_str[1] = ((char2 - 10) + 'A');
}

void GetHexBuff(char *hex_str, unsigned char *data, unsigned int data_len)
{
	unsigned int cnt = 0;
	char tmp_str[2];
	for(; cnt < data_len; cnt++)
	{
		GetHexChar(tmp_str, data[cnt]);
		hex_str[cnt * 2] = tmp_str[0];
		hex_str[(cnt * 2) + 1] = tmp_str[1];
	}
	hex_str[cnt * 2] = 0;
}




