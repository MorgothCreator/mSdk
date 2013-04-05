/*
 * gpio.c
 *
 * Created: 2/8/2013 12:48:00 AM
 *  Author: XxXx
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "gpio.h"

void GPIODirModeSet(unsigned int base_addr, unsigned char pin_nr, unsigned char direction)
{
	PORT_t *Port = (PORT_t *)base_addr;
	switch (direction)
	{
	case 0:
		Port->DIRSET = 1<<pin_nr;
		break;
	case 1:
		Port->DIRCLR = 1<<pin_nr;
		break;
	}
}

void GPIOPinWrite(unsigned int base_addr, unsigned char pin_nr, unsigned char state)
{
	PORT_t *Port = (PORT_t *)base_addr;
	switch (state)
	{
		case 0:
		Port->OUTCLR = 1<<pin_nr;
		break;
		case 1:
		Port->OUTSET = 1<<pin_nr;
		break;
	}
}

unsigned char GPIOPinRead(unsigned int base_addr, unsigned char pin_nr)
{
	PORT_t *Port = (PORT_t *)base_addr;
	return Port->IN & (1<<pin_nr);
}

bool GPIOUpDnEnable(unsigned int base_addr, unsigned char pin_nr, bool enable)
{
	PORT_t *Port = (PORT_t *)base_addr;
	volatile unsigned char* pin_ctrl = ((unsigned char*)&Port->PIN0CTRL) + (pin_nr & 0x07);
	if (!enable)
	{
		*pin_ctrl &= ~PORT_OPC_gm;
	}
	return true;
}

bool GPIOUpDn(unsigned int base_addr, unsigned char pin_nr, unsigned char value)
{
	PORT_t *Port = (PORT_t *)base_addr;
	volatile unsigned char* pin_ctrl = ((unsigned char*)&Port->PIN0CTRL) + (pin_nr & 0x07);
	switch(value)
	{
		case 0:
			*pin_ctrl = (*pin_ctrl & ~PORT_OPC_gm) | PORT_OPC_PULLDOWN_gc;
			break;
		case 1:
			*pin_ctrl = (*pin_ctrl & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc;
			break;
	}
	return true;
}

