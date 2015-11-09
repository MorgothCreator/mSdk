/*
 * mpr121.h
 *
 * Created: 5/31/2015 12:09:21 PM
 *  Author: eu
 */ 


#ifndef MPR121_H_
#define MPR121_H_

#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/gpio_api.h"

#define MPR121_ADDR	0x5A
// Match key inputs with electrode numbers
#define STAR 0
#define SEVEN 1
#define FOUR 2
#define ONE 3
#define ZERO 4
#define EIGHT 5
#define FIVE 6
#define TWO 7
#define POUND 8
#define NINE 9
#define SIX 10
#define THREE 11

// MPR121 Register Defines
#define	ELE0_L	0x04
#define	ELE0_M	0x05
#define	ELE1_L	0x06
#define	ELE1_M	0x07
#define	ELE2_L	0x08
#define	ELE2_M	0x09
#define	ELE3_L	0x0A
#define	ELE3_M	0x0B
#define	ELE4_L	0x0C
#define	ELE4_M	0x0D
#define	ELE5_L	0x0E
#define	ELE5_M	0x0F
#define	ELE6_L	0x10
#define	ELE6_M	0x11
#define	ELE7_L	0x12
#define	ELE7_M	0x13
#define	ELE8_L	0x14
#define	ELE8_M	0x15
#define	ELE9_L	0x16
#define	ELE9_M	0x17
#define	ELE10_L	0x18
#define	ELE10_M	0x19
#define	ELE11_L	0x1A
#define	ELE11_M	0x1B
#define	ELEPROX_L	0x1C
#define	ELEPROX_M	0x1D

#define MHD_R	0x2B
#define NHD_R	0x2C
#define	NCL_R 	0x2D
#define	FDL_R	0x2E
#define	MHD_F	0x2F
#define	NHD_F	0x30
#define	NCL_F	0x31
#define	FDL_F	0x32

#define	ELE0_T	0x41
#define	ELE0_R	0x42
#define	ELE1_T	0x43
#define	ELE1_R	0x44
#define	ELE2_T	0x45
#define	ELE2_R	0x46
#define	ELE3_T	0x47
#define	ELE3_R	0x48
#define	ELE4_T	0x49
#define	ELE4_R	0x4A
#define	ELE5_T	0x4B
#define	ELE5_R	0x4C
#define	ELE6_T	0x4D
#define	ELE6_R	0x4E
#define	ELE7_T	0x4F
#define	ELE7_R	0x50
#define	ELE8_T	0x51
#define	ELE8_R	0x52
#define	ELE9_T	0x53
#define	ELE9_R	0x54
#define	ELE10_T	0x55
#define	ELE10_R	0x56
#define	ELE11_T	0x57
#define	ELE11_R	0x58
#define	FIL_CFG	0x5B
//#define	FIL_CFG	0x5D
#define	ELE_CFG	0x5E
#define GPIO_CTRL0	0x73
#define	GPIO_CTRL1	0x74
#define GPIO_DATA	0x75
#define	GPIO_DIR	0x76
#define	GPIO_EN		0x77
#define	GPIO_SET	0x78
#define	GPIO_CLEAR	0x79
#define	GPIO_TOGGLE	0x7A
#define	ATO_CFG0	0x7B
#define	ATO_CFGU	0x7D
#define	ATO_CFGL	0x7E
#define	ATO_CFGT	0x7F
#define	MPR_RESET	0x80


// Global Constants
#define TOU_THRESH	0x0F
#define	REL_THRESH	0x0A

typedef struct  
{
	unsigned char addr;
	unsigned char data;
}mpr121_setup;

typedef struct
{
	unsigned short old_keys;
	Twi_t* TWI;
	Gpio_t* IrqPin;
}mpr121_t;

typedef struct
{
	unsigned short pushed;
	unsigned short released;
}mpr121_ret_t;

bool mpr121_read_short(mpr121_t *structure, unsigned char reg, unsigned short *return_data);
bool mpr121_read(mpr121_t *structure, unsigned char reg, unsigned char *return_data);
bool mpr121_write(mpr121_t *structure, unsigned char reg, unsigned char data);
bool mpr121_init(mpr121_t *structure);
/*#####################################################*/
#define new_mpr121 mpr121_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_mpr121(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mpr121.c"
#endif
/*#####################################################*/
#endif /* MPR121_H_ */
