/*
 * board_properties.h
 *
 * Created: 2/16/2013 9:34:07 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef BOARD_PROPERTIES_H_
#define BOARD_PROPERTIES_H_

#include "sys/dsp.h"

//#define FLASH_DEVICE
#define FFT_BUFFER_SIZE_LOG		9
#define USE_FFT_TABLES
#define USED_FFT_TEMP_REAL_BUFFERS	float
#define USE_FFT_STATIC_BUFF
#define FFT_INPUT_DATA_TYPE		signed char
#define FFT_OUTPUT_DATA_TYPE	float
/*#####################################################*/
#define FOSC32                  (32768)
#define OSC32_STARTUP           (AVR32_SCIF_OSCCTRL32_STARTUP_8192_RCOSC)

#define BOARD_OSC32_IS_XTAL     true
#define BOARD_OSC32_HZ          FOSC32
#define BOARD_OSC32_STARTUP_US  (71000)
#define BOARD_OSC32_PINSEL      (0)


#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_DFLL

#define CONFIG_DFLL0_SOURCE         GENCLK_SRC_OSC32K
#define CONFIG_DFLL0_MUL            ((50000000 * 2) / BOARD_OSC32_HZ)
#define CONFIG_DFLL0_DIV            2

// Disable all non-essential peripheral clocks
#define CONFIG_SYSCLK_INIT_CPUMASK  0
#define CONFIG_SYSCLK_INIT_PBAMASK  0
#define CONFIG_SYSCLK_INIT_PBBMASK  0
#define CONFIG_SYSCLK_INIT_HSBMASK  0

#include "sys/clock/sysclk.h"
/*#####################################################*/
// Configure CPU bus clock to 40MHz from DFFL

// Configure DFFL to 40MHz from external 32KHz crystal
/*#####################################################*/
#endif /* BOARD_PROPERTIES_H_ */
/*#####################################################*/
