/*
 * srf02.c
 *
 *  Created on: Jan 6, 2015
 *      Author:  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */

#include "stdbool.h"
#include "srf02.h"
#include "board_init.h"
#include "api/twi_api.h"
#include "api/timer_api.h"
#include "api/uart_api.h"

bool srf02_start(SRF02_t *structure, SRF02_COMMANDS command) {
	if(structure->busy)
		return true;
	timer_interval(&structure->Timeout_Timer, 80);
	structure->busy = true;
	Twi_t* TwiStruct = structure->TWI;
	unsigned char dev_addr = 0x70;
	if(structure->addr)
		dev_addr = structure->addr;

    unsigned char tmp[2];
    switch(command)
    {
    case SRF02_START_RANGING:
    case SRF02_FAKE_RANGING:
        tmp[0] = 0;
        tmp[1] = command + structure->measure_unit;
    	if(twi.tx(TwiStruct, dev_addr, tmp, 2))
    		structure->reg_inst = command;
    	break;
    default:
    	return false;
    }
    return true;
}

bool srf02_read(SRF02_t *structure) {
	if(timer_tick(&structure->Timeout_Timer)) {
		structure->busy = false;
		return false;
	}
	if(!structure->busy)
		return false;

	Twi_t* TwiStruct = structure->TWI;
	unsigned char dev_addr = 0x70;
	if(structure->addr)
		dev_addr = structure->addr;

    unsigned char reg = 2;
    unsigned char result[2];
	switch(structure->reg_inst)
    {
    case SRF02_START_RANGING:
    case SRF02_FAKE_RANGING:
        if(!twi.trx(TwiStruct, dev_addr, &reg, 1, result, 2))
        	return false;
        else {
        	structure->range_value = result[0] << 8;
        	structure->range_value |= result[1];
        	return true;
        }
    	break;
    default:
    	return false;
    }
    return true;
}

void srf02_display_data(SRF02_t *structure) {
	switch(structure->measure_unit)
	{
	case SRF02_INCH:
		uart.printf(DebugCom, "SRF02: Range = %u inch\n\r", (unsigned long)structure->range_value);
		break;
	case SRF02_CENTIMETER:
		uart.printf(DebugCom, "SRF02: Range = %u centimeters\n\r", (unsigned long)structure->range_value);
		break;
	case SRF02_MICROSECONDS:
		uart.printf(DebugCom, "SRF02: Range = %u microseconds\n\r", (unsigned long)structure->range_value);
		break;
	}
}

