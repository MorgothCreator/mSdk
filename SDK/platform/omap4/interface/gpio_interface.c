/*
 * gpio_interface.c
 *
 *  Created on: Jan 13, 2013
 *      Author: XxXx
 */

#include <stdlib.h>
#include "gpio_interface.h"
#include "driver/gpio.h"
#include "clk/clk_gpio.h"
#include "pinmux/pin_mux_gpio.h"
#include "api/gpio_def.h"


/*#####################################################*/
void _gpio_init(unsigned int GpioModuleNr)
{
	unsigned int BaseAddr = 0;
	switch (GpioModuleNr)
	{
	case 0:
		BaseAddr = GPIO0_addr;
		break;
	case 1:
		BaseAddr = GPIO1_addr;
		break;
	case 2:
		BaseAddr = GPIO2_addr;
		break;
	case 3:
		BaseAddr = GPIO3_addr;
		break;
	case 4:
		BaseAddr = GPIO4_addr;
		break;
	case 5:
		BaseAddr = GPIO5_addr;
		break;
	}
	GPIOModuleClkConfig(GpioModuleNr);
    GPIOModuleReset(BaseAddr);
	GPIOModuleEnable(BaseAddr);
}
/*#####################################################*/
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, unsigned int Direction)
{
    new_gpio* GpioStruct = new_(new_gpio);
    if(GpioStruct == NULL) return NULL;
	unsigned int BaseAddr = 0;
	switch (PortNr)
	{
	case 0:
		BaseAddr = GPIO0_addr;
		break;
	case 1:
		BaseAddr = GPIO1_addr;
		break;
	case 2:
		BaseAddr = GPIO2_addr;
		break;
	case 3:
		BaseAddr = GPIO3_addr;
		break;
	case 4:
		BaseAddr = GPIO4_addr;
		break;
	case 5:
		BaseAddr = GPIO5_addr;
		break;
	default:
		return NULL;
	}
	GpioStruct->BaseAddr = BaseAddr;
	GpioStruct->PinNr = PinNr;
	GpioStruct->Direction = Direction;
	gpio_mux_setup(PortNr, PinNr, 3);
	GPIODirModeSet(BaseAddr, PinNr, Direction);
	return GpioStruct;
}
/*#####################################################*/
void _gpio_free(new_gpio *gpio_struct)
{
	if(!gpio_struct) return;
	switch (gpio_struct->BaseAddr)
	{
	case GPIO0_addr:
		gpio_mux_setup(0, gpio_struct->PinNr, 0);
		break;
	case GPIO1_addr:
		gpio_mux_setup(1, gpio_struct->PinNr, 0);
		break;
	case GPIO2_addr:
		gpio_mux_setup(2, gpio_struct->PinNr, 0);
		break;
	case GPIO3_addr:
		gpio_mux_setup(3, gpio_struct->PinNr, 0);
		break;
	case GPIO4_addr:
		gpio_mux_setup(4, gpio_struct->PinNr, 0);
		break;
	case GPIO5_addr:
		gpio_mux_setup(5, gpio_struct->PinNr, 0);
		break;
	}
	free(gpio_struct);
}
/*#####################################################*/
bool _gpio_out(new_gpio *gpio_struct, unsigned int State)
{
	if(!gpio_struct) return false;
	GPIOPinWrite(gpio_struct->BaseAddr, gpio_struct->PinNr, State);
	return true;
}
/*#####################################################*/
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction)
{
	if(!gpio_struct) return false;
	GPIODirModeSet(gpio_struct->BaseAddr, gpio_struct->PinNr, Direction);
	return true;
}
/*#####################################################*/
signed int _gpio_in(new_gpio *gpio_struct)
{
	if(!gpio_struct) return -1;
	return GPIOPinRead(gpio_struct->BaseAddr, gpio_struct->PinNr)>>gpio_struct->PinNr;
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
