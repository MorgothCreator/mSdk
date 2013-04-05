/*
 * gpio_interface.c
 *
 * Created: 2/16/2013 9:59:35 PM
 *  Author: XxXx
 */ 

#include "gpio_interface.h"
#include "driver/gpio.h"

#include "api/gpio_def.h"

void _gpio_init(unsigned int GpioModuleNr)
{
/*	unsigned int BaseAddr = 0;
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
	}
	GPIOModuleClkConfig(GpioModuleNr);
	GPIOModuleReset(BaseAddr);
	GPIOModuleEnable(BaseAddr);*/
}
/*#####################################################*/
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, unsigned int Direction)
{
	new_gpio* GpioStruct = new_(new_gpio);
	if(GpioStruct == NULL) return NULL;
	unsigned int BaseAddr = 0;
	/*switch (PortNr)
	{
		case 0:
		BaseAddr = (unsigned int)(void*)&PORTA;
		break;
		default:
		return NULL;
	}*/
	GpioStruct->BaseAddr = BaseAddr;
	GpioStruct->PinNr = PinNr;
	GpioStruct->Direction = Direction;
	GpioStruct->PortNr = PortNr;
	//GPIODirModeSet(BaseAddr, PinNr, Direction);
	//gpio_configure_pin((PortNr << 5) + PinNr, GPIO_DIR_INPUT);
	gpio_set_gpio_pin((PortNr << 5) + PinNr);
	return GpioStruct;
}
/*#####################################################*/
void _gpio_free(new_gpio *gpio_struct)
{
	if(!gpio_struct) return;
	gpio_configure_pin((gpio_struct->PortNr << 5) + gpio_struct->PinNr, GPIO_DIR_INPUT);
	free(gpio_struct);
}
/*#####################################################*/
bool _gpio_out(new_gpio *gpio_struct, unsigned int State)
{
	if(!gpio_struct) return false;
	switch(State)
	{
		case 0:
			gpio_set_pin_low((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			break;
		case 1:
			gpio_set_pin_high((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			break;
		default:
			return false;
	}
	return true;
}
/*#####################################################*/
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction)
{
	if(!gpio_struct) return false;
	gpio_configure_pin((gpio_struct->PortNr << 5) + gpio_struct->PinNr, Direction);
	return true;
}
/*#####################################################*/
signed int _gpio_in(new_gpio *gpio_struct)
{
	if(!gpio_struct) return -1;
	return (signed int)gpio_get_pin_value((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
}
/*#####################################################*/
bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool enable)
{
	if(!gpio_struct) return false;
#if defined(AVR32_GPIO_200_H_INCLUDED) || defined(AVR32_GPIO_210_H_INCLUDED) ||	\
defined(AVR32_GPIO_212_H_INCLUDED)
	if(enable == false)
	{
		gpio_disable_pin_pull_up((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
		gpio_disable_pin_pull_down((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
	}
#else
	switch(enable)
	{
		case false:
			gpio_disable_pin_pull_up((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			break;
		case true:
			gpio_enable_pin_pull_up((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			break;
	}
#endif
	return true;
}
/*#####################################################*/
bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value)
{
	if(!gpio_struct) return false;
#if defined(AVR32_GPIO_200_H_INCLUDED) || defined(AVR32_GPIO_210_H_INCLUDED) ||	\
defined(AVR32_GPIO_212_H_INCLUDED)
	switch(value)
	{
		case 0:
			gpio_disable_pin_pull_up((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			gpio_enable_pin_pull_down((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			break;
		case 1:
			gpio_disable_pin_pull_down((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			gpio_enable_pin_pull_up((gpio_struct->PortNr << 5) + gpio_struct->PinNr);
			break;
		default:
			return false;
	}
#endif
	return true;
}
/*#####################################################*/
