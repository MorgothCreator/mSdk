/*
 *  lib/device/sx1509.h
 *
 *  Copyright (C) 2015  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*#####################################################*/
#ifndef SX1509_H_
#define SX1509_H_
/*#####################################################*/
#include "stdbool.h"
#include "api/twi_def.h"
#include "api/timer_api.h"
/*#####################################################*/
#define SX1509_ADDR_0								(0x3E)
#define SX1509_ADDR_1								(0x3F)
#define SX1509_ADDR_2								(0x70)
#define SX1509_ADDR_3								(0x71)
/*
 * Device and IO Banks
 */
#define SX1509_RegInputDisableB						0x00
#define SX1509_RegInputDisableA						0x01
#define SX1509_RegLongSlewB							0x02
#define SX1509_RegLongSlewA							0x03
#define SX1509_RegLowDriveB							0x04
#define SX1509_RegLowDriveA							0x05
#define SX1509_RegPullUpB							0x06
#define SX1509_RegPullUpA							0x07
#define SX1509_RegPullDownB							0x08
#define SX1509_RegPullDownA							0x09
#define SX1509_RegOpenDrainB						0x0A
#define SX1509_RegOpenDrainA						0x0B
#define SX1509_RegPolarityB							0x0C
#define SX1509_RegPolarityA							0x0D
#define SX1509_RegDirB								0x0E
#define SX1509_RegDirA								0x0F
#define SX1509_RegDataB								0x10
#define SX1509_RegDataA								0x11
#define SX1509_RegInterruptMaskB					0x12
#define SX1509_RegInterruptMaskA					0x13
#define SX1509_RegSenseHighB						0x14
#define SX1509_RegSenseLowB							0x15
#define SX1509_RegSenseHighA						0x16
#define SX1509_RegSenseLowA							0x17
#define SX1509_RegInterruptSourceB					0x18
#define SX1509_RegInterruptSourceA					0x19
#define SX1509_RegEventStatusB						0x1A
#define SX1509_RegEventStatusA						0x1B
#define SX1509_RegLevelShifter1						0x1C
#define SX1509_RegLevelShifter2						0x1D
#define SX1509_RegClock								0x1E
#define SX1509_RegMisc								0x1F
#define SX1509_RegLEDDriverEnableB					0x20
#define SX1509_RegLEDDriverEnableA					0x21
/*
 * Debounce and Keypad Engine
 */
#define SX1509_RegDebounceConfig					0x22
#define SX1509_RegDebounceEnableB					0x23
#define SX1509_RegDebounceEnableA					0x24
#define SX1509_RegKeyConfig1						0x25
#define SX1509_RegKeyConfig2						0x26
#define SX1509_RegKeyData1							0x27
#define SX1509_RegKeyData2							0x28
/*
 * LED Driver (PWM, blinking, breathing)
 */
#define SX1509_RegTOn0								0x29
#define SX1509_RegIOn0								0x2A
#define SX1509_RegOff0								0x2B
#define SX1509_RegTOn1								0x2C
#define SX1509_RegIOn1								0x2D
#define SX1509_RegOff1								0x2E
#define SX1509_RegTOn2								0x2F
#define SX1509_RegIOn2								0x30
#define SX1509_RegOff2								0x31
#define SX1509_RegTOn3								0x32
#define SX1509_RegIOn3								0x33
#define SX1509_RegOff3								0x34
#define SX1509_RegTOn4								0x35
#define SX1509_RegIOn4								0x36
#define SX1509_RegOff4								0x37
#define SX1509_RegTRise4							0x38
#define SX1509_RegTFall4							0x39
#define SX1509_RegTOn5								0x3A
#define SX1509_RegIOn5								0x3B
#define SX1509_RegOff5								0x3C
#define SX1509_RegTRise5							0x3D
#define SX1509_RegTFall5							0x3E
#define SX1509_RegTOn6								0x3F
#define SX1509_RegIOn6								0x40
#define SX1509_RegOff6								0x41
#define SX1509_RegTRise6							0x42
#define SX1509_RegTFall6							0x43
#define SX1509_RegTOn7								0x44
#define SX1509_RegIOn7								0x45
#define SX1509_RegOff7								0x46
#define SX1509_RegTRise7							0x47
#define SX1509_RegTFall7							0x48
#define SX1509_RegTOn8								0x49
#define SX1509_RegIOn8								0x4A
#define SX1509_RegOff8								0x4B
#define SX1509_RegTOn9								0x4C
#define SX1509_RegIOn9								0x4D
#define SX1509_RegOff9								0x4E
#define SX1509_RegTOn10								0x4F
#define SX1509_RegIOn10								0x50
#define SX1509_RegOff10								0x51
#define SX1509_RegTOn11								0x52
#define SX1509_RegIOn11								0x53
#define SX1509_RegOff11								0x54
#define SX1509_RegTOn12								0x55
#define SX1509_RegIOn12								0x56
#define SX1509_RegOff12								0x57
#define SX1509_RegTRise12							0x58
#define SX1509_RegTFall12							0x59
#define SX1509_RegTOn13								0x5A
#define SX1509_RegIOn13								0x5B
#define SX1509_RegOff13								0x5C
#define SX1509_RegTRise13							0x5D
#define SX1509_RegTFall13							0x5E
#define SX1509_RegTOn14								0x5F
#define SX1509_RegIOn14								0x60
#define SX1509_RegOff14								0x61
#define SX1509_RegTRise14							0x62
#define SX1509_RegTFall14							0x63
#define SX1509_RegTOn15								0x64
#define SX1509_RegIOn15								0x65
#define SX1509_RegOff15								0x66
#define SX1509_RegTRise15							0x67
#define SX1509_RegTFall15							0x68
/*
 * Miscellaneous
 */
#define SX1509_RegHighInputB						0x69
#define SX1509_RegHighInputA						0x6A
/*
 * Software Reset
 */
#define SX1509_RegReset								0x7D
/*
 * Test (not to be written)
 */
#define SX1509_RegTest1								0x7E
#define SX1509_RegTest2								0x7F
/*#####################################################*/
typedef struct SX1509_s{
	STimer_t Timeout_Timer;
	bool busy;
	unsigned char Addr;
	Twi_t* TWI;
}SX1509_t;
/*#####################################################*/
bool sx1509_init(SX1509_t *structure);
/*#####################################################*/
#define new_sx1509 SX1509_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_sx1509(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "sx1509.c"
#endif
/*#####################################################*/
#endif /* SX1509_H_ */
/*#####################################################*/
