/*
 * twi.h
 *
 * Created: 2/18/2013 4:36:18 PM
 *  Author: XxXx
 */ 


#ifndef TWI_H_
#define TWI_H_

/*! \name Error Codes for the Module
 */
//! @{
#define TWI_SUCCESS              0
#define TWI_INVALID_ARGUMENT    -1
#define TWI_ARBITRATION_LOST    -2
#define TWI_NO_CHIP_FOUND       -3
#define TWI_RECEIVE_OVERRUN     -4
#define TWI_RECEIVE_NACK        -5
#define TWI_SEND_OVERRUN        -6
#define TWI_SEND_NACK           -7
//! @}


#include "twi.c"

#endif /* TWI_H_ */