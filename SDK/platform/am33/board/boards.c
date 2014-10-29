/*
 * boards.c
 *
 *  Created on: Oct 28, 2014
 *      Author: Morgoth
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "board/boards.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "device/24c.h"


/********************************************************************/
extern new_uart* Uart[];
extern new_twi* TWI[];
extern new_uart* DebugCom;
/*****************************************************************************
**                   GLOBAL VARIABLE DEFINITIONS
*****************************************************************************/
unsigned char *OnBoardConfigData = NULL;
unsigned char *Board1ConfigData = NULL;
unsigned char *Board2ConfigData = NULL;
unsigned char *Board3ConfigData = NULL;
unsigned char *Board4ConfigData = NULL;
/* Reads EEPROM and validates the board information */
/********************************************************************/
unsigned int BoardInfoCheck(unsigned char *boardId, unsigned char *boardVer);
/********************************************************************/
unsigned int RtcVersionGet(void)
{
	return 2;
}

unsigned int EDMAVersionGet(void)
{
	return 2;
}
#ifdef beaglebone
/********************************************************************/
static bool verify_if_is_beaglebone_board_extension(unsigned char*BoardInfo)
{
	if((unsigned int)(BoardInfo[0]<<24 | BoardInfo[1]<<16 | BoardInfo[2]<<8 | BoardInfo[3]<<0) == 0xAA5533EE) return true;
	else return false;
}
/********************************************************************/
static bool beaglebone_decode_board_info(unsigned char *BoardInfo)
{
	if(!memcmp(BoardInfo + 4, "A0BeagleBone BATTERY CAPE", 25))
	{
		UARTPuts(DebugCom, "BeagleBone battery cape detected.\n\r", -1);
		return true;
	}
	if(!memcmp(BoardInfo + 4, "A0BeagleBone LCD3 CAPE", 22))
	{
		UARTPuts(DebugCom, "BeagleBone LCD3 cape detected.\n\r", -1);
		return true;
	}
	return false;
}
/********************************************************************/
void beaglebone_detect_extension_boards()
{
	unsigned char TwiBuff[49];
	TwiBuff[48] = 0;

	if(E2promRead(E2PROM_BOARD1, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:1 unidentified.\n\r" , -1);
		}
		else
		{
			Board1ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board1ConfigData, TwiBuff, 48);
		}
	}else Board1ConfigData = NULL;

	if(E2promRead(E2PROM_BOARD2, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:2 unidentified.\n\r" , -1);
		}
		else
		{
			Board2ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board2ConfigData, TwiBuff, 48);
		}
	}else Board2ConfigData = NULL;

	if(E2promRead(E2PROM_BOARD3, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:3 unidentified.\n\r" , -1);
		}
		else
		{
			Board3ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board3ConfigData, TwiBuff, 48);
		}
	}else Board3ConfigData = NULL;

	if(E2promRead(E2PROM_BOARD4, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:4 unidentified.\n\r" , -1);
		}
		else
		{
			Board4ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board4ConfigData, TwiBuff, 48);
		}
	}else Board4ConfigData = NULL;
}
/********************************************************************/
bool is_beaglebone()
{
	UARTPuts(DebugCom, "Read BeagleBone data.\n\r" , -1);
	unsigned char TwiBuff[49];
	TwiBuff[48] = 0;
	if(E2promRead(E2PROM_ONBOARD, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		OnBoardConfigData = (unsigned char*)calloc(1, 49);
		memcpy(OnBoardConfigData, TwiBuff, 48);
		if(!memcmp(OnBoardConfigData + 4, "A335BONE", 8))
		{
			unsigned char tmpstr[3];
			memcpy(tmpstr, OnBoardConfigData + 14, 2);
			tmpstr[2] = (int)NULL;
			UARTprintf(DebugCom, "Board detected: BeagleBone rev %s.\n\r", tmpstr);
			return true;
		}
		else if(!memcmp(OnBoardConfigData + 4, "A335BNLT", 8))
		{
			unsigned char tmpstr[3];
			memcpy(tmpstr, OnBoardConfigData + 14, 2);
			tmpstr[2] = (int)NULL;
			UARTprintf(DebugCom, "Board detected: BeagleBoneBlack rev %s.\n\r", tmpstr);
			return true;
		}
		else
		{
			OnBoardConfigData = NULL;
		}
	}
	return false;
}
/********************************************************************/
#endif
