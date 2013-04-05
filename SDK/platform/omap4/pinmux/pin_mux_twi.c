/*
 * pin_mux_twi.c
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */
/*#####################################################*/
#include <stdbool.h>
#include "pin_mux_twi.h"
#include "driver/control_module.h"
/*#####################################################*/
unsigned int pin_mux_twi_scl(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, true, 1, true);
	return 1;
}

unsigned int pin_mux_twi_sda(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, true, 1, true);
	return 1;
}
/*#####################################################*/

