/*
 * gpio_interface.c
 *
 *  Created on: Mar 21, 2013
 *      Author: XxXx
 */
#include "stdio.h"
#include "stdlib.h"
#include "gpio_def.h"
#include "stm32f10x_conf.h"
#include "include/stm32f10x.h"
#include "gpio_interface.h"
#include "driver/stm32f10x_gpio.h"
#include "sys/system_stm32f10x.h"
#include "api/gpio_def.h"

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
};

const unsigned int GET_PORT_CLK_ADDR[] = {RCC_APB2Periph_GPIOA
#ifdef RCC_APB2Periph_GPIOB
		,RCC_APB2Periph_GPIOB
#endif
#ifdef RCC_APB2Periph_GPIOC
		,RCC_APB2Periph_GPIOC
#endif
#ifdef RCC_APB2Periph_GPIOD
		,RCC_APB2Periph_GPIOD
#endif
#ifdef RCC_APB2Periph_GPIOE
		,RCC_APB2Periph_GPIOE
#endif
#ifdef RCC_APB2Periph_GPIOF
		,RCC_APB2Periph_GPIOF
#endif
#ifdef RCC_APB2Periph_GPIOG
		,RCC_APB2Periph_GPIOG
#endif
#ifdef RCC_APB2Periph_GPIOH
		,RCC_APB2Periph_GPIOH
#endif
#ifdef RCC_APB2Periph_GPIOI
		,RCC_APB2Periph_GPIOI
#endif
};


bool _gpio_init(gpio_port_enum GpioModuleNr)
{
	unsigned int BaseAddr = 0;
	switch (GpioModuleNr)
	{
#ifdef GPIOA
		case IOA:
			BaseAddr = RCC_APB2Periph_GPIOA;
			break;
#endif
#ifdef GPIOB
		case IOB:
			BaseAddr = RCC_APB2Periph_GPIOB;
			break;
#endif
#ifdef GPIOC
		case IOC:
			BaseAddr = RCC_APB2Periph_GPIOC;
			break;
#endif
#ifdef GPIOD
		case IOD:
			BaseAddr = RCC_APB2Periph_GPIOD;
			break;
#endif
#ifdef GPIOE
		case IOE:
			BaseAddr = RCC_APB2Periph_GPIOE;
			break;
#endif
#ifdef GPIOF
		case IOF:
			BaseAddr = RCC_APB2Periph_GPIOF;
			break;
#endif
#ifdef GPIOG
		case IOG:
			BaseAddr = RCC_APB2Periph_GPIOG;
			break;
#endif
#ifdef GPIOH
		case IOH:
			BaseAddr = RCC_APB2Periph_GPIOH;
			break;
#endif
#ifdef GPIOI
		case IOI:
			BaseAddr = RCC_APB2Periph_GPIOI;
			break;
#endif
		default:
			return false;
	}
	RCC_APB2PeriphClockCmd(BaseAddr, ENABLE);
	return true;
}
/*#####################################################*/
new_gpio *_gpio_assign(gpio_port_enum PortNr, unsigned int PinNr, gpio_type_enum function, bool Multipin)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	new_gpio* GpioStruct = new_(new_gpio);
	if(GpioStruct == NULL) return NULL;
	unsigned int BaseAddr = 0;
	switch (PortNr)
	{
#ifdef GPIOA
		case IOA:
			BaseAddr = (unsigned int)GPIOA;
			break;
#endif
#ifdef GPIOB
		case IOB:
			BaseAddr = (unsigned int)GPIOB;
			break;
#endif
#ifdef GPIOC
		case IOC:
			BaseAddr = (unsigned int)GPIOC;
			break;
#endif
#ifdef GPIOD
		case IOD:
			BaseAddr = (unsigned int)GPIOD;
			break;
#endif
#ifdef GPIOE
		case IOE:
			BaseAddr = (unsigned int)GPIOE;
			break;
#endif
#ifdef GPIOF
		case IOF:
			BaseAddr = (unsigned int)GPIOF;
			break;
#endif
#ifdef GPIOG
		case IOG:
			BaseAddr = (unsigned int)GPIOG;
			break;
#endif
#ifdef GPIOH
		case IOH:
			BaseAddr = (unsigned int)GPIOH;
			break;
#endif
#ifdef GPIOI
		case IOI:
			BaseAddr = (unsigned int)GPIOI;
			break;
#endif
		default:
		return NULL;
	}
	GpioStruct->BaseAddr = BaseAddr;
	GpioStruct->Pin = PinNr;
	GpioStruct->Direction = function;
	GPIO_InitStructure.GPIO_Pin = 1 << PinNr;
	switch(function)
	{
	case GPIO_AIN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		break;
	case GPIO_IN_FLOATING:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		break;
	case GPIO_IN_PULL_DOWN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		break;
	case GPIO_IN_PULL_UP:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		break;
	case GPIO_OUT_OPEN_DRAIN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		break;
	case GPIO_OUT_PUSH_PULL:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		break;
	case GPIO_ALTERNATIVE_OPEN_DRINE:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
		break;
	case GPIO_ALTERNATIVE_PUSH_PULL:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		break;
	default:
		return false;

	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init((GPIO_TypeDef *)BaseAddr, &GPIO_InitStructure);
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
	if(State) BaseAddr->BSRR = 1 << gpio_struct->Pin;
	else  BaseAddr->BRR = 1 << gpio_struct->Pin;
	return true;
}
/*#####################################################*/
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction)
{
	if(!gpio_struct) return false;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 1 << gpio_struct->Pin;
	if(Direction == GPIO_DIR_OUTPUT) GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	else if(Direction == GPIO_DIR_INPUT) GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init((GPIO_TypeDef *)BaseAddr, &GPIO_InitStructure);
	return true;
}
/*#####################################################*/
signed int _gpio_in(new_gpio *gpio_struct)
{
	if(!gpio_struct) return -1;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;
	return GPIO_ReadInputDataBit(BaseAddr, 1 << gpio_struct->Pin);
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

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 1 << gpio_struct->Pin;
	if(value == 0) GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	else if(value == 1) GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init((GPIO_TypeDef *)BaseAddr, &GPIO_InitStructure);
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
		break;
	case GPIO_IN_FLOATING:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		break;
	case GPIO_IN_PULL_DOWN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		break;
	case GPIO_IN_PULL_UP:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		break;
	case GPIO_OUT_OPEN_DRAIN:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		break;
	case GPIO_OUT_PUSH_PULL:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		break;
	case GPIO_ALTERNATIVE_OPEN_DRINE:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
		break;
	case GPIO_ALTERNATIVE_PUSH_PULL:
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		break;
	default:
		return false;

	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init((GPIO_TypeDef *)BaseAddr, &GPIO_InitStructure);
	return true;
}
/*#####################################################*/
