/*
 * gpio_interface.c
 *
 *  Created on: Feb 8, 2016
 *      Author: John Smith
 */

#include <stdio.h>
#include <stdlib.h>
#include "gpio_interface.h"
#include "gpio_interface_def.h"
#include "api/gpio_def.h"
#include "driver/ioc.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ioc.h"
#include "inc/hw_types.h"
#include "driver/prcm.h"
#include "driver/sys_ctrl.h"

extern const uint32_t g_pui32ModuleCG[];

void _gpio_init(unsigned int GpioModuleNr)
{
    /*PRCMPeripheralRunEnable(g_pui32ModuleCG[11]);
    PRCMPeripheralSleepEnable(g_pui32ModuleCG[11]);
    PRCMPeripheralDeepSleepEnable(g_pui32ModuleCG[11]);
    PRCMLoadSet();
    while(!PRCMLoadGet())
    { }*/
}

new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, gpio_type_enum function, bool Multipin)
{
    new_gpio* GpioStruct = new_(new_gpio);
    if(GpioStruct == NULL)
    	return NULL;
	GpioStruct->BaseAddr = 0;
	GpioStruct->Pin = PinNr;
	GpioStruct->Direction = function;
	GpioStruct->PortNr = PortNr;
	GpioStruct->Multipin = Multipin;
	unsigned long pin_func = 0;
	switch(function)
	{
	case GPIO_OUT_PUSH_PULL:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << PinNr;
		break;
	case GPIO_IN_FLOATING:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << PinNr);
		break;
	case GPIO_IN_PULL_DOWN:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_DOWN | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << PinNr);
		break;
	case GPIO_IN_PULL_UP:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << PinNr);
		break;
	case GPIO_OUT_OPEN_DRAIN:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_OPEN_DRAIN_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << PinNr;
		break;
	default:
		_gpio_free(GpioStruct);
		return NULL;
	}
	IOCPortConfigureSet(PinNr, IOC_PORT_GPIO, pin_func);
	return GpioStruct;
}

void _gpio_free(new_gpio *gpio_struct)
{
    if(gpio_struct == NULL)
    	return;
	IOCPortConfigureSet(gpio_struct->Pin, IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
	HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << gpio_struct->Pin);
	free(gpio_struct);
}

bool _gpio_out(new_gpio *gpio_struct, unsigned int State)
{
   if(gpio_struct == NULL)
    	return false;
	if(State)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << gpio_struct->Pin;
	else
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << gpio_struct->Pin;

	return false;
}

bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction)
{
	return false;
}

signed int _gpio_in(new_gpio *gpio_struct)
{
	return (HWREG(GPIO_BASE + GPIO_O_DIN31_0) >> gpio_struct->Pin) & 0x01;
}

bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool value)
{
	return false;
}

bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value)
{
	return false;
}

bool _gpio_function_set(new_gpio *gpio_struct, gpio_type_enum function)
{
    if(gpio_struct == NULL)
    	return NULL;
	unsigned long pin_func = 0;
	switch(function)
	{
	case GPIO_OUT_PUSH_PULL:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << gpio_struct->Pin;
		break;
	case GPIO_IN_FLOATING:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << gpio_struct->Pin);
		break;
	case GPIO_IN_PULL_DOWN:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_DOWN | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << gpio_struct->Pin;
		break;
	case GPIO_IN_PULL_UP:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << gpio_struct->Pin;
		break;
	case GPIO_OUT_OPEN_DRAIN:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_OPEN_DRAIN_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << gpio_struct->Pin;
		break;
	default:
		return false;
	}
	IOCPortConfigureSet(gpio_struct->Pin, IOC_PORT_GPIO, pin_func);
	return true;
}


