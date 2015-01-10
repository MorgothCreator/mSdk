/*
 * gpio_interface.c
 *
 *  Created on: Mar 21, 2013
 *      Author: XxXx
 */
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "gpio_interface.h"
#include "driver/stm32f4xx_gpio.h"
#include "sys/system_stm32f4xx.h"
#include "api/gpio_def.h"
#include "stm32f4xx_gpio.h"

GPIO_TypeDef* GET_GPIO_PORT_ADDR[] = { GPIOA
#ifdef GPIOB
		,GPIOB
#endif
#ifdef GPIOC
		,GPIOC
#endif
#ifdef GPIOD
		,GPIOD
#endif
#ifdef GPIOE
		,GPIOE
#endif
#ifdef GPIOF
		,GPIOF
#endif
#ifdef GPIOG
		,GPIOG
#endif
#ifdef GPIOH
		,GPIOH
#endif
#ifdef GPIOI
		,GPIOI
#endif
#ifdef GPIOJ
		,GPIOJ
#endif
};

const unsigned int GET_PORT_CLK_ADDR[] = {RCC_AHB1Periph_GPIOA
#ifdef RCC_AHB1Periph_GPIOB
		,RCC_AHB1Periph_GPIOB
#endif
#ifdef RCC_AHB1Periph_GPIOC
		,RCC_AHB1Periph_GPIOC
#endif
#ifdef RCC_AHB1Periph_GPIOD
		,RCC_AHB1Periph_GPIOD
#endif
#ifdef RCC_AHB1Periph_GPIOE
		,RCC_AHB1Periph_GPIOE
#endif
#ifdef RCC_AHB1Periph_GPIOF
		,RCC_AHB1Periph_GPIOF
#endif
#ifdef RCC_AHB1Periph_GPIOG
		,RCC_AHB1Periph_GPIOG
#endif
#ifdef RCC_AHB1Periph_GPIOH
		,RCC_AHB1Periph_GPIOH
#endif
#ifdef RCC_AHB1Periph_GPIOI
		,RCC_AHB1Periph_GPIOI
#endif
#ifdef RCC_AHB1Periph_GPIOJ
		,RCC_AHB1Periph_GPIOJ
#endif
};

void _gpio_init(unsigned int GpioModuleNr)
{
	unsigned int BaseAddr = 0;
	switch (GpioModuleNr)
	{
		case 0:
			BaseAddr = RCC_AHB1Periph_GPIOA;
			break;
		case 1:
			BaseAddr = RCC_AHB1Periph_GPIOB;
			break;
		case 2:
			BaseAddr = RCC_AHB1Periph_GPIOC;
			break;
		case 3:
			BaseAddr = RCC_AHB1Periph_GPIOD;
			break;
		case 4:
			BaseAddr = RCC_AHB1Periph_GPIOE;
			break;
		case 5:
			BaseAddr = RCC_AHB1Periph_GPIOF;
			break;
		case 6:
			BaseAddr = RCC_AHB1Periph_GPIOG;
			break;
		case 7:
			BaseAddr = RCC_AHB1Periph_GPIOH;
			break;
		case 8:
			BaseAddr = RCC_AHB1Periph_GPIOI;
			break;
		default:
			return;
	}
	RCC_AHB1PeriphClockCmd(BaseAddr, ENABLE);
}
/*#####################################################*/
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int Pin, gpio_type_enum function, bool Multipin)
{
	new_gpio* GpioStruct = new_(new_gpio);
	if(GpioStruct == NULL) return NULL;
	unsigned int BaseAddr = 0;
	switch (PortNr)
	{
		case 0:
			BaseAddr = (unsigned int)GPIOA;
			break;
		case 1:
			BaseAddr = (unsigned int)GPIOB;
			break;
		case 2:
			BaseAddr = (unsigned int)GPIOC;
			break;
		case 3:
			BaseAddr = (unsigned int)GPIOD;
			break;
		case 4:
			BaseAddr = (unsigned int)GPIOE;
			break;
		case 5:
			BaseAddr = (unsigned int)GPIOF;
			break;
		case 6:
			BaseAddr = (unsigned int)GPIOG;
			break;
		case 7:
			BaseAddr = (unsigned int)GPIOH;
			break;
		case 8:
			BaseAddr = (unsigned int)GPIOI;
			break;
		default:
		return NULL;
	}
	GpioStruct->BaseAddr = BaseAddr;
	GpioStruct->Pin = Pin;
	GpioStruct->Direction = function;
	GpioStruct->PortNr = PortNr;
	GpioStruct->Multipin = Multipin;
	_gpio_function_set(GpioStruct, function);
	return GpioStruct;
}
/*#####################################################*/
void _gpio_free(new_gpio *gpio_struct)
{
	if(!gpio_struct) return;
	GPIO_DeInit((GPIO_TypeDef*)gpio_struct->BaseAddr);
	free(gpio_struct);
}
/*#####################################################*/
bool _gpio_out(new_gpio *gpio_struct, unsigned int State)
{
	if(!gpio_struct) return false;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;
	if(gpio_struct->Multipin)
	{
		BaseAddr->BSRRH = State & gpio_struct->Pin;
		BaseAddr->BSRRL = (~State) & gpio_struct->Pin;
	}
	else
	{
		if(State) BaseAddr->BSRRH = 1 << gpio_struct->Pin;
		else  BaseAddr->BSRRL = 1 << gpio_struct->Pin;
	}
	return true;
}
/*#####################################################*/
bool _gpio_direction(new_gpio *gpio_struct, gpio_type_enum function)
{
	return _gpio_function_set(gpio_struct, function);
}
/*#####################################################*/
signed int _gpio_in(new_gpio *gpio_struct)
{
	if(!gpio_struct) return -1;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;
	if(gpio_struct->Multipin) return GPIO_ReadInputData(BaseAddr) & gpio_struct->Pin;
	else return GPIO_ReadInputDataBit(BaseAddr, 1 << gpio_struct->Pin);
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
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;
	unsigned char cnt = 0;
	unsigned int tmp = gpio_struct->Pin;
	if(gpio_struct->Multipin)
	{
		for(; cnt < 16; cnt++)
		{
			if(tmp & 1)
			{
				BaseAddr->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)cnt * 2));
				if(value) BaseAddr->PUPDR |= (((uint32_t)GPIO_PuPd_UP) << (cnt * 2));
				else BaseAddr->PUPDR |= (((uint32_t)GPIO_PuPd_DOWN) << (cnt * 2));
			}
			tmp = tmp >> 1;
		}
	}
	else
	{
		BaseAddr->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)gpio_struct->Pin * 2));
		if(value) BaseAddr->PUPDR |= (((uint32_t)GPIO_PuPd_UP) << (gpio_struct->Pin * 2));
		else BaseAddr->PUPDR |= (((uint32_t)GPIO_PuPd_DOWN) << (gpio_struct->Pin * 2));
	}
	return true;
}
/*#####################################################*/
bool _gpio_function_set(new_gpio *gpio_struct, gpio_type_enum function)
{
	if(!gpio_struct) return false;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 1 << gpio_struct->Pin;
	switch(function)
	{
	case GPIO_AIN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;
	case GPIO_IN_FLOATING:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;
	case GPIO_IN_PULL_DOWN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		break;
	case GPIO_IN_PULL_UP:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		break;
	case GPIO_OUT_OPEN_DRAIN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		break;
	case GPIO_OUT_PUSH_PULL:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		break;
	default:
		return false;

	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init((GPIO_TypeDef *)BaseAddr, &GPIO_InitStructure);
	return true;
}
/*#####################################################*/


