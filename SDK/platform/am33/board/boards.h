/*
 * boards.h
 *
 *  Created on: Oct 28, 2014
 *      Author: Morgoth
 */

#ifndef BOARDS_H_
#define BOARDS_H_

#include "stdbool.h"
#define BBB_BOARD_NAME                      ("A335BON")
#define BBB_A1_VERSION                      ("00A1")
#define BBB_A2_VERSION                      ("00A2")
#define BBB_A3_VERSION                      ("00A3")
#define BBB_A5_VERSION                      ("00A5")
#define BBB_A6_VERSION                      ("00A6")
#define BOARD_ID_BBB_A1                     (0x1)
#define BOARD_ID_BBB_A2                     (0x2)
#define BOARD_ID_BBB_A3                     (0x3)
#define BOARD_ID_BBB_A5                     (0x4)
#define BOARD_ID_BBB_A6                     (0x5)


/***************************************************************************/
#define E2PROM_ONBOARD TWI[0], 0x50
#define E2PROM_BOARD1 TWI[2],0x54
#define E2PROM_BOARD2 TWI[2],0x55
#define E2PROM_BOARD3 TWI[2],0x56
#define E2PROM_BOARD4 TWI[2],0x57
/****************************************************************************
**                    MACRO DEFINITIONS
****************************************************************************/

#define EEPROM_SIZE_HEADER         (4)
#define EEPROM_SIZE_BOARD_NAME     (8)
#define EEPROM_SIZE_VERSION        (5)
#define EEPROM_SIZE_SERIAL_NUMBER  (12)
#define MAX_DATA                   (EEPROM_SIZE_HEADER + \
		                    		EEPROM_SIZE_BOARD_NAME + \
		                    		EEPROM_SIZE_VERSION + \
		                    		EEPROM_SIZE_SERIAL_NUMBER)

#define BOARD_UNKNOWN              (0xFF)
#define BOARD_VER_UNKNOWN          (0xFE)

/****************************************************************************
**                  STRUCTURE DECLARATION
****************************************************************************/

struct EEPROMData {
                 unsigned char header[EEPROM_SIZE_HEADER];
                 unsigned char boardName[EEPROM_SIZE_BOARD_NAME];
                 unsigned char version[EEPROM_SIZE_VERSION];
                 unsigned char serialNumber[EEPROM_SIZE_SERIAL_NUMBER];
            };

typedef struct EEPROMData BOARDINFO;
/***************************************************************************/
extern unsigned char *OnBoardConfigData;
extern unsigned char *Board1ConfigData;
extern unsigned char *Board2ConfigData;
extern unsigned char *Board3ConfigData;
extern unsigned char *Board4ConfigData;
/***************************************************************************/
void beaglebone_detect_extension_boards();
bool is_beaglebone();

#endif /* BOARDS_H_ */
