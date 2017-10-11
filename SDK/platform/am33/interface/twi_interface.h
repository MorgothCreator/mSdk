/*
 * twi_interface.h
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/twi_def.h"
#include "../aintc/aintc_twi.h"
#include "../pinmux/pin_mux_twi.h"
#include "../clk/clk_twi.h"
#include "../include/hsi2c.h"
/*#####################################################*/
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes);
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes);
bool _I2C_trx(struct Twi_s* param, unsigned char addr, unsigned char *buff_send, unsigned int bytes_send, unsigned char *buff_receive, unsigned int bytes_receive);
bool _I2C_tx(struct Twi_s* param, unsigned char addr, unsigned char *buff_receive, unsigned int bytes_receive);
bool _twi_open(new_twi* TwiStruct);
void _twi_close(new_twi* TwiStruct);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "twi_interface.c"
#endif
/*#####################################################*/
#endif /* TWI_INTERFACE_H_ */
/*#####################################################*/
