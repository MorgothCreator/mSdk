/*
 * pin_mux_spi.c
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "pin_mux_spi.h"
#include "driver/control_module.h"


unsigned int pin_mux_spi_sclk(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, false);
	return 1;
}

unsigned int pin_mux_spi_d0(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, true);
	return 1;
}

unsigned int pin_mux_spi_d1(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, true);
	return 1;
}

unsigned int pin_mux_spi_cs0(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, false);
	return 1;
}

unsigned int pin_mux_spi_cs1(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, false);
	return 1;
}

unsigned int pin_mux_spi_cs2(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, false);
	return 1;
}

unsigned int pin_mux_spi_cs3(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, false);
	return 1;
}



