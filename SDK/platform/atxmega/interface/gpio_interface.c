/*
 * gpio_interface.c
 *
 * Created: 2/8/2013 12:33:18 AM
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
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int Pin, unsigned int Direction, bool Multipin)
{
	new_gpio* GpioStruct = new_(new_gpio);
	if(GpioStruct == NULL) return NULL;
	unsigned int BaseAddr = 0;
	switch (PortNr)
	{
		case 0:
		BaseAddr = (unsigned int)(void*)&PORTA;
		break;
		case 1:
		BaseAddr = (unsigned int)(void*)&PORTB;
		break;
		case 2:
		BaseAddr = (unsigned int)(void*)&PORTC;
		break;
		case 3:
		BaseAddr = (unsigned int)(void*)&PORTD;
		break;
		case 4:
		BaseAddr = (unsigned int)(void*)&PORTE;
		break;
#ifdef PORTF
		case 5:
		BaseAddr = (unsigned int)(void*)&PORTF;
		break;
#endif
#ifdef PORTH
		case 6:
		BaseAddr = (unsigned int)(void*)&PORTH;
		break;
#endif
#ifdef PORTJ
		case 7:
		BaseAddr = (unsigned int)(void*)&PORTJ;
		break;
#endif
#ifdef PORTK
		case 8:
		BaseAddr = (unsigned int)(void*)&PORTK;
		break;
#endif
#ifdef PORTQ
		case 9:
		BaseAddr = (unsigned int)(void*)&PORTQ;
		break;
#endif
#ifdef PORTR
		case 10:
		BaseAddr = (unsigned int)(void*)&PORTR;
		break;
#endif
		default:
		return NULL;
	}
	GpioStruct->BaseAddr = BaseAddr;
	GpioStruct->Pin = Pin;
	GpioStruct->Direction = Direction;
	GPIODirModeSet(BaseAddr, Pin, Direction);
	return GpioStruct;
}
/*#####################################################*/
void _gpio_free(new_gpio *gpio_struct)
{
	if(!gpio_struct) return;
	GPIODirModeSet(gpio_struct->BaseAddr, gpio_struct->Pin, GPIO_DIR_INPUT);
	free(gpio_struct);
}
/*#####################################################*/
bool _gpio_out(new_gpio *gpio_struct, unsigned int State)
{
	if(!gpio_struct) return false;
	if(gpio_struct->Multipin)
	{
		PORT_t *Port = (PORT_t *)gpio_struct->BaseAddr;
		Port->OUTSET = State & gpio_struct->Pin;
		Port->OUTCLR = (~State) & gpio_struct->Pin;
	}
	else GPIOPinWrite(gpio_struct->BaseAddr, gpio_struct->Pin, State);
	return true;
}
/*#####################################################*/
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction)
{
	if(!gpio_struct) return false;
	if(gpio_struct->Multipin)
	{
		PORT_t *Port = (PORT_t *)gpio_struct->BaseAddr;
		Port->DIRSET = Direction & gpio_struct->Pin;
		Port->DIRCLR = (~Direction) & gpio_struct->Pin;
	}
	else GPIODirModeSet(gpio_struct->BaseAddr, gpio_struct->Pin, Direction);
	return true;
}
/*#####################################################*/
signed int _gpio_in(new_gpio *gpio_struct)
{
	if(!gpio_struct) return -1;
	return GPIOPinRead(gpio_struct->BaseAddr, gpio_struct->Pin);
}
/*#####################################################*/
bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool enable)
{
	if(!gpio_struct) return false;
	return GPIOUpDnEnable(gpio_struct->BaseAddr, gpio_struct->Pin, enable);
}
/*#####################################################*/
bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value)
{
	if(!gpio_struct) return false;
	return GPIOUpDn(gpio_struct->BaseAddr, gpio_struct->Pin, value);
}
/*#####################################################*/
