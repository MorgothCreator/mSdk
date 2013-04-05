/*
 * gpio_interface.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include <stdbool.h>
#include "api/gpio_def.h"
/*#####################################################*/
void _gpio_init(unsigned int GpioModuleNr)
{

}
/*#####################################################*/
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, unsigned int Direction)
{
    new_gpio* GpioStruct = new_(new_gpio);
    if(GpioStruct == NULL) return NULL;

    IO_pioen(PortNr, PinNr);
	GpioStruct->PinNr = PinNr;
	GpioStruct->PortNr = PortNr;
	GpioStruct->Direction = Direction;
	//GPIODirModeSet(BaseAddr, PinNr, Direction);
	switch(Direction)
	{
	case 0:
		IO_setoutput(PortNr, PinNr);
		break;
	case 1:
		IO_setinput(PortNr, PinNr);
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
		IO_pinclear(gpio_struct->PortNr, gpio_struct->PinNr);
		break;
	case 1:
		IO_pinset(gpio_struct->PortNr, gpio_struct->PinNr);
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
		IO_setoutput(gpio_struct->PortNr, gpio_struct->PinNr);
		break;
	case 1:
		IO_setinput(gpio_struct->PortNr, gpio_struct->PinNr);
		break;
	}
	return true;
}
/*#####################################################*/
signed int _gpio_in(new_gpio *gpio_struct)
{
	if(!gpio_struct) return -1;
	return (signed int)IO_pinget(gpio_struct->PortNr, gpio_struct->PinNr);
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
