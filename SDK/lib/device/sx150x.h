/*
 * sx150x.h
 *
 *  Created on: Nov 12, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_SX150X_H_
#define LIB_DEVICE_SX150X_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/twi_def.h"
#include "api/timer_api.h"
/*#####################################################*/
#define SX1507_ADDR1	0x3E
#define SX1507_ADDR2	0x3F
#define SX1507_ADDR3	0x70
#define SX1507_ADDR4	0x71

#define SX1508_ADDR1	0x20
#define SX1508_ADDR2	0x21
#define SX1508_ADDR3	0x22
#define SX1508_ADDR4	0x23

#define SX1509_ADDR1	0x3E
#define SX1509_ADDR2	0x3F
#define SX1509_ADDR3	0x70
#define SX1509_ADDR4	0x71
/*#####################################################*/
#define SX1507_RegInputDisable				0x00//  Input buffer disable register XXXX 0000
#define SX1507_RegLongSlew					0x01//  Output buffer long slew register XXXX 0000
#define SX1507_RegLowDrive					0x02//  Output buffer low drive register XXXX 0000
#define SX1507_RegPullUp					0x03//  Pull-up register XXXX 0000
#define SX1507_RegPullDown					0x04//  Pull-down register XXXX 0000
#define SX1507_RegOpenDrain					0x05//  Open drain register XXXX 0000
#define SX1507_RegPolarity					0x06//  Polarity register XXXX 0000
#define SX1507_RegDir						0x07//  Direction register XXXX 1111
#define SX1507_RegData						0x08//  Data register XXXX 1111*
#define SX1507_RegInterruptMask				0x09//  Interrupt mask register XXXX 1111
#define SX1507_RegSense						0x0A//  Sense register 0000 0000
#define SX1507_RegInterruptSource			0x0B//  Interrupt source register XXXX 0000
#define SX1507_RegEventStatus				0x0C//  Event status register XXXX 0000
#define SX1507_RegClock						0x0D//  Clock management register 0000 0000
#define SX1507_RegMisc						0x0E//  Miscellaneous device settings register 0000 0000
#define SX1507_RegLEDDriverEnable			0x0F//  LED driver enable register XXXX 0000
/*Debounce*/
#define SX1507_RegDebounceConfig			0x10//  Debounce configuration register 0000 0000
#define SX1507_RegDebounceEnable			0x11//  Debounce enable register XXXX 0000
/*LED Driver (PWM, blinking, breathing)*/
#define SX1507_RegTOn0						0x12//  ON time register for I/O[0] 0000 0000
#define SX1507_RegIOn0						0x13//  ON intensity register for I/O[0] 1111 1111
#define SX1507_RegOff0						0x14//  OFF time/intensity register for I/O[0] 0000 0000
#define SX1507_RegTOn1						0x15//  ON time register for I/O[1] 0000 0000
#define SX1507_RegIOn1						0x16//  ON intensity register for I/O[1] 1111 1111
#define SX1507_RegOff1						0x17//  OFF time/intensity register for I/O[1] 0000 0000
#define SX1507_RegTRise1					0x18//  Fade in register for I/O[1] 0000 0000
#define SX1507_RegTFall1					0x19//  Fade out register for I/O[1] 0000 0000
#define SX1507_RegTOn2						0x1A//  ON time register for I/O[2] 0000 0000
#define SX1507_RegIOn2						0x1B//  ON intensity register for I/O[2] 1111 1111
#define SX1507_RegOff2						0x1C//  OFF time/intensity register for I/O[2] 0000 0000
#define SX1507_RegTRise2					0x1D//  Fade in register for I/O[2] 0000 0000
#define SX1507_RegTFall2					0x1E//  Fade out register for I/O[2] 0000 0000
#define SX1507_RegTOn3						0x1F//  ON time register for I/O[3] 0000 0000
#define SX1507_RegIOn3						0x20//  ON intensity register for I/O[3] 1111 1111
#define SX1507_RegOff3						0x21//  OFF time/intensity register for I/O[3] 0000 0000
#define SX1507_RegTRise3					0x22//  Fade in register for I/O[3] 0000 0000
#define SX1507_RegTFall3					0x23//  Fade out register for I/O[3] 0000 0000
/*Software Reset*/
#define SX1507_RegReset						0x7D//  Software reset register 0000 0000
/*Test (not to be written)*/
#define SX1507_RegTest1						0x7E//  Test register 0000 0000
#define SX1507_RegTest2						0x7F//  Test register 0000 0000
/*#####################################################*/
#define SX1508_RegInputDisable				0x00//  Input buffer disable register 0000 0000
#define SX1508_RegLongSlew					0x01//  Output buffer long slew register 0000 0000
#define SX1508_RegLowDrive					0x02//  Output buffer low drive register 0000 0000
#define SX1508_RegPullUp					0x03//  Pull-up register 0000 0000
#define SX1508_RegPullDown					0x04//  Pull-down register 0000 0000
#define SX1508_RegOpenDrain					0x05//  Open drain register 0000 0000
#define SX1508_RegPolarity					0x06//  Polarity register 0000 0000
#define SX1508_RegDir						0x07//  Direction register 1111 1111
#define SX1508_RegData						0x08//  Data register 1111 1111*
#define SX1508_RegInterruptMask				0x09//  Interrupt mask register 1111 1111
#define SX1508_RegSenseHigh					0x0A//  Sense register for I/O[7:4] 0000 0000
#define SX1508_RegSenseLow					0x0B//  Sense register for I/O[3:0] 0000 0000
#define SX1508_RegInterruptSource			0x0C//  Interrupt source register 0000 0000
#define SX1508_RegEventStatus				0x0D//  Event status register 0000 0000
#define SX1508_RegLevelShifter				0x0E//  Level shifter register 0000 0000
#define SX1508_RegClock						0x0F//  Clock management register 0000 0000
#define SX1508_RegMisc						0x10//  Miscellaneous device settings register 0000 0000
#define SX1508_RegLEDDriverEnable			0x11//  LED driver enable register 0000 0000
/*Debounce and Keypad Engine*/
#define SX1508_RegDebounceConfig			0x12//  Debounce configuration register 0000 0000
#define SX1508_RegDebounceEnable			0x13//  Debounce enable register 0000 0000
#define SX1508_RegKeyConfig					0x14//  Key scan configuration register 0000 0000
#define SX1508_RegKeyData					0x15//  Key value 1111 1111
/*LED Driver (PWM, blinking, breathing)*/
#define SX1508_RegIOn0						0x16//  ON intensity register for I/O[0] 1111 1111
#define SX1508_RegIOn1						0x17//  ON intensity register for I/O[1] 1111 1111
#define SX1508_RegTOn2						0x18//  ON time register for I/O[2] 0000 0000
#define SX1508_RegIOn2						0x19//  ON intensity register for I/O[2] 1111 1111
#define SX1508_RegOff2						0x1A//  OFF time/intensity register for I/O[2] 0000 0000
#define SX1508_RegTOn3						0x1B//  ON time register for I/O[3] 0000 0000
#define SX1508_RegIOn3						0x1C//  ON intensity register for I/O[3] 1111 1111
#define SX1508_RegOff3						0x1D//  OFF time/intensity register for I/O[3] 0000 0000
#define SX1508_RegTRise3					0x1E//  Fade in register for I/O[3] 0000 0000
#define SX1508_RegTFall3					0x1F//  Fade out register for I/O[3] 0000 0000
#define SX1508_RegIOn4						0x20//  ON intensity register for I/O[4] 1111 1111
#define SX1508_RegIOn5						0x21//  ON intensity register for I/O[5] 1111 1111
#define SX1508_RegTOn6						0x22//  ON time register for I/O[6] 0000 0000
#define SX1508_RegIOn6						0x23//  ON intensity register for I/O[6] 1111 1111
#define SX1508_RegOff6						0x24//  OFF time/intensity register for I/O[6] 0000 0000
#define SX1508_RegTOn7						0x25//  ON time register for I/O[7] 0000 0000
#define SX1508_RegIOn7						0x26//  ON intensity register for I/O[7] 1111 1111
#define SX1508_RegOff7						0x27//  OFF time/intensity register for I/O[7] 0000 0000
#define SX1508_RegTRise7					0x28//  Fade in register for I/O[7] 0000 0000
#define SX1508_RegTFall7					0x29// Fade out register for I/O[7] 0000 0000
/*Miscellaneous*/
#define SX1508_RegHighInput					0x2A// High input enable register 0000 0000
/*Software Reset*/
#define SX1508_RegReset						0x7D// Software reset register 0000 0000
/*Test (not to be written)*/
#define SX1508_RegTest1						0x7E// Test register 0000 0000
#define SX1508_RegTest2						0x7F// Test register 0000 0000
/*#####################################################*/
#define SX1509_RegInputDisableB				0x00// Input buffer disable register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegInputDisableA				0x01//  Input buffer disable register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegLongSlewB					0x02//  Output buffer long slew register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegLongSlewA					0x03//  Output buffer long slew register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegLowDriveB					0x04//  Output buffer low drive register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegLowDriveA					0x05//  Output buffer low drive register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegPullUpB					0x06//  Pull-up register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegPullUpA					0x07//  Pull-up register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegPullDownB					0x08//  Pull-down register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegPullDownA					0x09//  Pull-down register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegOpenDrainB				0x0A//  Open drain register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegOpenDrainA				0x0B//  Open drain register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegPolarityB					0x0C//  Polarity register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegPolarityA					0x0D//  Polarity register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegDirB						0x0E//  Direction register - I/O[15-8] (Bank B) 1111 1111
#define SX1509_RegDirA						0x0F//  Direction register - I/O[7-0] (Bank A) 1111 1111
#define SX1509_RegDataB						0x10//  Data register - I/O[15-8] (Bank B) 1111 1111*
#define SX1509_RegDataA						0x11//  Data register - I/O[7-0] (Bank A) 1111 1111*
#define SX1509_RegInterruptMaskB			0x12//  Interrupt mask register - I/O[15-8] (Bank B) 1111 1111
#define SX1509_RegInterruptMaskA			0x13//  Interrupt mask register - I/O[7-0] (Bank A) 1111 1111
#define SX1509_RegSenseHighB				0x14//  Sense register for I/O[15:12] 0000 0000
#define SX1509_RegSenseLowB					0x15//  Sense register for I/O[11:8] 0000 0000
#define SX1509_RegSenseHighA				0x16//  Sense register for I/O[7:4] 0000 0000
#define SX1509_RegSenseLowA					0x17//  Sense register for I/O[3:0] 0000 0000
#define SX1509_RegInterruptSourceB			0x18//  Interrupt source register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegInterruptSourceA			0x19//  Interrupt source register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegEventStatusB				0x1A//  Event status register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegEventStatusA				0x1B//  Event status register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegLevelShifter1				0x1C//  Level shifter register 0000 0000
#define SX1509_RegLevelShifter2				0x1D//  Level shifter register 0000 0000
#define SX1509_RegClock						0x1E//  Clock management register 0000 0000
#define SX1509_RegMisc						0x1F//  Miscellaneous device settings register 0000 0000
#define SX1509_RegLEDDriverEnableB			0x20//  LED driver enable register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegLEDDriverEnableA			0x21//  LED driver enable register - I/O[7-0] (Bank A) 0000 0000
/*Debounce and Keypad Engine*/
#define SX1509_RegDebounceConfig			0x22//  Debounce configuration register 0000 0000
#define SX1509_RegDebounceEnableB			0x23//  Debounce enable register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegDebounceEnableA			0x24//  Debounce enable register - I/O[7-0] (Bank A) 0000 0000
#define SX1509_RegKeyConfig1				0x25//  Key scan configuration register 0000 0000
#define SX1509_RegKeyConfig2				0x26//  Key scan configuration register 0000 0000
#define SX1509_RegKeyData1					0x27//  Key value (column) 1111 1111
#define SX1509_RegKeyData2					0x28//  Key value (row) 1111 1111
/*LED Driver (PWM, blinking, breathing)*/
#define SX1509_RegTOn0						0x29//  ON time register for I/O[0] 0000 0000
#define SX1509_RegIOn0						0x2A//  ON intensity register for I/O[0] 1111 1111
#define SX1509_RegOff0						0x2B//  OFF time/intensity register for I/O[0] 0000 0000
#define SX1509_RegTOn1						0x2C//  ON time register for I/O[1] 0000 0000
#define SX1509_RegIOn1						0x2D//  ON intensity register for I/O[1] 1111 1111
#define SX1509_RegOff1						0x2E//  OFF time/intensity register for I/O[1] 0000 0000
#define SX1509_RegTOn2						0x2F//  ON time register for I/O[2] 0000 0000
#define SX1509_RegIOn2						0x30//  ON intensity register for I/O[2] 1111 1111
#define SX1509_RegOff2						0x31//  OFF time/intensity register for I/O[2] 0000 0000
#define SX1509_RegTOn3						0x32//  ON time register for I/O[3] 0000 0000
#define SX1509_RegIOn3						0x33//  ON intensity register for I/O[3] 1111 1111
#define SX1509_RegOff3						0x34//  OFF time/intensity register for I/O[3] 0000 0000
#define SX1509_RegTOn4						0x35//  ON time register for I/O[4] 0000 0000
#define SX1509_RegIOn4						0x36//  ON intensity register for I/O[4] 1111 1111
#define SX1509_RegOff4						0x37//  OFF time/intensity register for I/O[4] 0000 0000
#define SX1509_RegTRise4					0x38//  Fade in register for I/O[4] 0000 0000
/*ADVANCED COMMUNICATIONS & SENSING*/
#define SX1509_RegTFall4					0x39//  Fade out register for I/O[4] 0000 0000
#define SX1509_RegTOn5						0x3A//  ON time register for I/O[5] 0000 0000
#define SX1509_RegIOn5						0x3B//  ON intensity register for I/O[5] 1111 1111
#define SX1509_RegOff5						0x3C//  OFF time/intensity register for I/O[5] 0000 0000
#define SX1509_RegTRise5					0x3D//  Fade in register for I/O[5] 0000 0000
#define SX1509_RegTFall5					0x3E//  Fade out register for I/O[5] 0000 0000
#define SX1509_RegTOn6						0x3F//  ON time register for I/O[6] 0000 0000
#define SX1509_RegIOn6						0x40//  ON intensity register for I/O[6] 1111 1111
#define SX1509_RegOff6						0x41//  OFF time/intensity register for I/O[6] 0000 0000
#define SX1509_RegTRise6					0x42//  Fade in register for I/O[6] 0000 0000
#define SX1509_RegTFall6					0x43//  Fade out register for I/O[6] 0000 0000
#define SX1509_RegTOn7						0x44//  ON time register for I/O[7] 0000 0000
#define SX1509_RegIOn7						0x45//  ON intensity register for I/O[7] 1111 1111
#define SX1509_RegOff7						0x46//  OFF time/intensity register for I/O[7] 0000 0000
#define SX1509_RegTRise7					0x47//  Fade in register for I/O[7] 0000 0000
#define SX1509_RegTFall7					0x48//  Fade out register for I/O[7] 0000 0000
#define SX1509_RegTOn8						0x49//  ON time register for I/O[8] 0000 0000
#define SX1509_RegIOn8						0x4A//  ON intensity register for I/O[8] 1111 1111
#define SX1509_RegOff8						0x4B//  OFF time/intensity register for I/O[8] 0000 0000
#define SX1509_RegTOn9						0x4C//  ON time register for I/O[9] 0000 0000
#define SX1509_RegIOn9						0x4D//  ON intensity register for I/O[9] 1111 1111
#define SX1509_RegOff9						0x4E//  OFF time/intensity register for I/O[9] 0000 0000
#define SX1509_RegTOn10						0x4F//  ON time register for I/O[10] 0000 0000
#define SX1509_RegIOn10						0x50//  ON intensity register for I/O[10] 1111 1111
#define SX1509_RegOff10						0x51//  OFF time/intensity register for I/O[10] 0000 0000
#define SX1509_RegTOn11						0x52//  ON time register for I/O[11] 0000 0000
#define SX1509_RegIOn11						0x53//  ON intensity register for I/O[11] 1111 1111
#define SX1509_RegOff11						0x54//  OFF time/intensity register for I/O[11] 0000 0000
#define SX1509_RegTOn12						0x55//  ON time register for I/O[12] 0000 0000
#define SX1509_RegIOn12						0x56//  ON intensity register for I/O[12] 1111 1111
#define SX1509_RegOff12						0x57//  OFF time/intensity register for I/O[12] 0000 0000
#define SX1509_RegTRise12					0x58//  Fade in register for I/O[12] 0000 0000
#define SX1509_RegTFall12					0x59//  Fade out register for I/O[12] 0000 0000
#define SX1509_RegTOn13						0x5A//  ON time register for I/O[13] 0000 0000
#define SX1509_RegIOn13						0x5B//  ON intensity register for I/O[13] 1111 1111
#define SX1509_RegOff13						0x5C//  OFF time/intensity register for I/O[13] 0000 0000
#define SX1509_RegTRise13					0x5D//  Fade in register for I/O[13] 0000 0000
#define SX1509_RegTFall13					0x5E//  Fade out register for I/O[13] 0000 0000
#define SX1509_RegTOn14						0x5F//  ON time register for I/O[14] 0000 0000
#define SX1509_RegIOn14						0x60//  ON intensity register for I/O[14] 1111 1111
#define SX1509_RegOff14						0x61//  OFF time/intensity register for I/O[14] 0000 0000
#define SX1509_RegTRise14					0x62//  Fade in register for I/O[14] 0000 0000
#define SX1509_RegTFall14					0x63//  Fade out register for I/O[14] 0000 0000
#define SX1509_RegTOn15						0x64//  ON time register for I/O[15] 0000 0000
#define SX1509_RegIOn15						0x65//  ON intensity register for I/O[15] 1111 1111
#define SX1509_RegOff15						0x66//  OFF time/intensity register for I/O[15] 0000 0000
#define SX1509_RegTRise15					0x67//  Fade in register for I/O[15] 0000 0000
#define SX1509_RegTFall15					0x68//  Fade out register for I/O[15] 0000 0000
/*Miscellaneous*/
#define SX1509_RegHighInputB				0x69//  High input enable register - I/O[15-8] (Bank B) 0000 0000
#define SX1509_RegHighInputA				0x6A//  High input enable register - I/O[7-0] (Bank A) 0000 0000
/*Software Reset*/
#define SX1509_RegReset						0x7D//  Software reset register 0000 0000
/*Test (not to be written)*/
#define SX1509_RegTest1						0x7E//  Test register 0000 0000
#define SX1509_RegTest2						0x7F//  Test register 0000 0000
/*#####################################################*/
typedef struct SX150x_s{
	bool (*reset)(struct SX150x_s *param);
	bool (*probe)(struct SX150x_s *param);
	bool (*read_reg)(struct SX150x_s *param, unsigned char addr, unsigned char *data);
	bool (*write_reg)(struct SX150x_s *param, unsigned char addr, unsigned char data);
	//STimer_t Timeout_Timer;
	Twi_t* TWI;
	unsigned char Twi_Addr;
}SX150x_t;
/*#####################################################*/
bool sx150x_read_reg(SX150x_t *param, unsigned char addr, unsigned char *data);
bool sx150x_write_reg(SX150x_t *param, unsigned char addr, unsigned char data);
bool sx150x_probe(SX150x_t *param);
bool sx150x_reset(SX150x_t *param);
bool sx150x_open(SX150x_t *param);
/*#####################################################*/
#define new_sx150x SX150x_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_sx150x(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "sx150x.c"
#endif
/*#####################################################*/
#endif /* LIB_DEVICE_SX150X_H_ */
