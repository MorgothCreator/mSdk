/*
 * board_properties.h
 *
 * Created: 2/16/2013 9:34:07 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef BOARD_PROPERTIES_H_
#define BOARD_PROPERTIES_H_
/*#####################################################*/
#include <stdbool.h>
/*#####################################################*/
#define USE_SELECTED_FONTS
//#define FLASH_DEVICE
#define FFT_BUFFER_SIZE_LOG		8
#define USE_FFT_TABLES
#define USED_FFT_TEMP_BUFFERS	double
#define USE_FFT_STATIC_BUFF
#define FFT_INPUT_DATA_TYPE		signed char
#define FFT_OUTPUT_DATA_TYPE	unsigned long
/*#####################################################*/
#define FOSC32                  (32768)
#define OSC32_STARTUP           (AVR32_SCIF_OSCCTRL32_STARTUP_8192_RCOSC)

#define BOARD_OSC32_IS_XTAL     true
#define BOARD_OSC32_HZ          FOSC32
#define BOARD_OSC32_STARTUP_US  (71000)
#define BOARD_OSC32_PINSEL      (0)


#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_DFLL

#define CONFIG_DFLL0_SOURCE         GENCLK_SRC_OSC32K
#define CONFIG_DFLL0_MUL            ((66000000 * 2) / BOARD_OSC32_HZ)
#define CONFIG_DFLL0_DIV            2

// Disable all non-essential peripheral clocks
#define CONFIG_SYSCLK_INIT_CPUMASK  0
#define CONFIG_SYSCLK_INIT_PBAMASK  0
#define CONFIG_SYSCLK_INIT_PBBMASK  0
#define CONFIG_SYSCLK_INIT_HSBMASK  0

/*#####################################################*/
#define ph7_62_RGB_Port					1

#define ph7_62_R1_PinPosition			11
#define ph7_62_R1_PinMask				(1 << ph7_62_R1_PinPosition)
#define ph7_62_R2_PinPosition			10
#define ph7_62_R2_PinMask				(1 << ph7_62_R2_PinPosition)

#define ph7_62_G1_PinPosition			0
#define ph7_62_G1_PinMask				(1 << ph7_62_G1_PinPosition)
#define ph7_62_G2_PinPosition			3
#define ph7_62_G2_PinMask				(1 << ph7_62_G2_PinPosition)

#define ph7_62_B1_PinPosition			2
#define ph7_62_B1_PinMask				(1 << ph7_62_B1_PinPosition)
#define ph7_62_B2_PinPosition			1
#define ph7_62_B2_PinMask				(1 << ph7_62_B2_PinPosition)

#define ph7_62_Sclk_Port				0
#define ph7_62_Sclk_PinPosition			11
#define ph7_62_Sclk_PinMask				(1 << ph7_62_Sclk_PinPosition)

#define ph7_62_Latch_Port				0
#define ph7_62_Latch_PinPosition		22
#define ph7_62_Latch_PinMask			(1 << ph7_62_Latch_PinPosition)

#define ph7_62_Oe_Port					0
#define ph7_62_Oe_PinPosition			17
#define ph7_62_Oe_PinMask				(1 << ph7_62_Oe_PinPosition)

#define ph7_62_ABC_Port					0
#define ph7_62_ABC_PinPosition			14
#define ph7_62_ABC_PinMask				(7 << ph7_62_ABC_PinPosition)

#define ph7_62_Width					32
#define ph7_62_Height					16

#define ph7_62_PanelsNr					2
/*#####################################################*/
#include "sys/clock/sysclk.h"
/*#####################################################*/
// Configure CPU bus clock to 40MHz from DFFL

// Configure DFFL to 40MHz from external 32KHz crystal
/*#####################################################*/
#endif /* BOARD_PROPERTIES_H_ */
/*#####################################################*/
