/*
 * twi_interface.h
 *
 * Created: 2/8/2013 5:00:12 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_
/*#####################################################*/
/*! Transaction status defines. */
#define TWIM_STATUS_READY              0
#define TWIM_STATUS_BUSY               1
//#####################################################
//#define TWIM_WRITE_BUFFER_SIZE         8
//#define TWIM_READ_BUFFER_SIZE          8

typedef enum TWIM_RESULT_enum {
	TWIM_RESULT_UNKNOWN          = (0x00<<0),
	TWIM_RESULT_OK               = (0x01<<0),
	TWIM_RESULT_BUFFER_OVERFLOW  = (0x02<<0),
	TWIM_RESULT_ARBITRATION_LOST = (0x03<<0),
	TWIM_RESULT_BUS_ERROR        = (0x04<<0),
	TWIM_RESULT_NACK_RECEIVED    = (0x05<<0),
	TWIM_RESULT_FAIL             = (0x06<<0),
} TWIM_RESULT_t;
/*#####################################################*/
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes);
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes);
bool _twi_open(new_twi* TwiStruct);
void _twi_close(new_twi* TwiStruct);
/*#####################################################*/
#include "twi_interface.c"
/*#####################################################*/
#endif /* TWI_INTERFACE_H_ */
/*#####################################################*/
