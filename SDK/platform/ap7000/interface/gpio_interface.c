/*
 * gpio_interface.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include <stdbool.h>
#include <avr32/io.h>
#include <avr32/ap7000.h>
#include "api/gpio_def.h"
#include "driver/gpio.h"
/*#####################################################*/
void _gpio_init(unsigned int GpioModuleNr)
{

}
/*#####################################################*/
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int Pin, unsigned int Direction, bool Multipin)
{
    new_gpio* GpioStruct = new_(new_gpio);
    if(GpioStruct == NULL) return NULL;

    IO_pioen(PortNr, Pin);
	GpioStruct->Pin = Pin;
	GpioStruct->PortNr = PortNr;
	GpioStruct->Direction = Direction;
	GpioStruct->Multipin = Multipin;
    volatile  avr32_pio_t  *pio;
    switch (PortNr) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return;
    }
	GpioStruct->BaseAddr = pio;
	//GPIODirModeSet(BaseAddr, PinNr, Direction);
	switch(Direction)
	{
	case 0:
		IO_setoutput(PortNr, Pin);
		break;
	case 1:
		IO_setinput(PortNr, Pin);
		break;
	}
	return GpioStruct;
}
/*#####################################################*/
void _gpio_free(new_gpio *gpio_struct)
{
	if(gpio_struct) free(gpio_struct);
}
/*#####################################################*/
bool _gpio_out(new_gpio *gpio_struct, unsigned int State)
{
	if(!gpio_struct) return false;
	switch(State)
	{
	case 0:
		IO_pinclear(gpio_struct->PortNr, gpio_struct->Pin);
		break;
	case 1:
		IO_pinset(gpio_struct->PortNr, gpio_struct->Pin);
		break;
	}
	return true;
}
/*#####################################################*/
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction)
{
	if(!gpio_struct) return false;
	switch(Direction)
	{
	case 0:
		IO_setoutput(gpio_struct->PortNr, gpio_struct->Pin);
		break;
	case 1:
		IO_setinput(gpio_struct->PortNr, gpio_struct->Pin);
		break;
	}
	return true;
}
/*#####################################################*/
signed int _gpio_in(new_gpio *gpio_struct)
{
	if(!gpio_struct) return -1;
	if(gpio_struct->Multipin)
	{
		volatile  avr32_pio_t  *pio = (avr32_pio_t  *)gpio_struct->BaseAddr;
		return pio->pdsr & gpio_struct->Pin;
	}
	else return (signed int)IO_pinget(gpio_struct->PortNr, gpio_struct->Pin);
}
/*#####################################################*/
bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool enable)
{
	if(!gpio_struct) return false;
	return true;//GPIOUpDnEnable(gpio_struct->BaseAddr, gpio_struct->PinNr, enable);
}
/*#####################################################*/
bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value)
{
	if(!gpio_struct) return false;
	return true;//GPIOUpDn(gpio_struct->BaseAddr, gpio_struct->PinNr, value);
}
/*#####################################################*/
