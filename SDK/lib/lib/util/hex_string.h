/*
 * hex_string.h
 *
 *  Created on: Sep 11, 2016
 *      Author: John Smith
 */

#ifndef LIB_LIB_UTIL_HEX_STRING_H_
#define LIB_LIB_UTIL_HEX_STRING_H_

void GetHexChar(char *hex_str, unsigned char data);
void GetHexBuff(char *hex_str, unsigned char *data, unsigned int data_len);
bool GetBinFromHexChar(unsigned char *dest, char src);
unsigned int GetBinFromHexBuff(unsigned char *bin_buff, char *data, unsigned int dest_buff_len);


#endif /* LIB_LIB_UTIL_HEX_STRING_H_ */
