/*
 * twi.c
 *
 * Created: 2/18/2013 4:36:08 PM
 *  Author: XxXx
 */ 

#include "twi.h"


int twi_set_speed(volatile avr32_twim_t *twi, unsigned int speed, unsigned long pba_hz)
{
	unsigned int cldiv;
	unsigned int ckdiv = 0;

	cldiv = (pba_hz / speed) - 4;

	// cldiv must fit in 8 bits, ckdiv must fit in 3 bits
	while ((cldiv > 0xFF) && (ckdiv < 0x7))
	{
		// increase clock divider
		ckdiv++;
		// divide cldiv value
		cldiv /= 2;
	}
	// set clock waveform generator register
	twi->cwgr = ( cldiv | (cldiv << AVR32_TWIM_CWGR_HIGH_OFFSET) | (ckdiv << AVR32_TWIM_CWGR_EXP_OFFSET) | (cldiv << AVR32_TWIM_CWGR_STASTO_OFFSET) | (3 << AVR32_TWIM_CWGR_DATA_OFFSET));

	return TWI_SUCCESS;
}

