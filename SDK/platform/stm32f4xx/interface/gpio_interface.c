/*
 * gpio_interface.c
 *
 *  Created on: Mar 21, 2013
 *      Author: XxXx
 */
#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "gpio_interface.h"
#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "sys/system_stm32f4xx.h"
#include "api/gpio_def.h"
#include "driver/stm32f4xx_hal_gpio.h"

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
#ifdef GPIOK
		,GPIOK
#endif
#ifdef GPIOL
		,GPIOL
#endif
};
#if 0
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
#endif
void _gpio_init(unsigned int GpioModuleNr)
{
	//unsigned int BaseAddr = 0;
	switch (GpioModuleNr)
	{
#ifdef __HAL_RCC_GPIOA_CLK_ENABLE
	case 0:
			//BaseAddr = RCC_AHB1Periph_GPIOA;
			__HAL_RCC_GPIOA_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOB_CLK_ENABLE
		case 1:
			//BaseAddr = RCC_AHB1Periph_GPIOB;
			__HAL_RCC_GPIOB_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOC_CLK_ENABLE
		case 2:
			//BaseAddr = RCC_AHB1Periph_GPIOC;
			__HAL_RCC_GPIOC_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOD_CLK_ENABLE
		case 3:
			//BaseAddr = RCC_AHB1Periph_GPIOD;
			__HAL_RCC_GPIOD_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOE_CLK_ENABLE
		case 4:
			//BaseAddr = RCC_AHB1Periph_GPIOE;
			__HAL_RCC_GPIOE_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOF_CLK_ENABLE
		case 5:
			//BaseAddr = RCC_AHB1Periph_GPIOF;
			__HAL_RCC_GPIOF_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOG_CLK_ENABLE
		case 6:
			//BaseAddr = RCC_AHB1Periph_GPIOG;
			__HAL_RCC_GPIOG_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOH_CLK_ENABLE
		case 7:
			//BaseAddr = RCC_AHB1Periph_GPIOH;
			__HAL_RCC_GPIOH_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOI_CLK_ENABLE
		case 8:
			//BaseAddr = RCC_AHB1Periph_GPIOI;
			__HAL_RCC_GPIOI_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOJ_CLK_ENABLE
		case 9:
			//BaseAddr = RCC_AHB1Periph_GPIOI;
			__HAL_RCC_GPIOJ_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOK_CLK_ENABLE
		case 10:
			//BaseAddr = RCC_AHB1Periph_GPIOI;
			__HAL_RCC_GPIOK_CLK_ENABLE();
			break;
#endif
#ifdef __HAL_RCC_GPIOL_CLK_ENABLE
		case 11:
			//BaseAddr = RCC_AHB1Periph_GPIOI;
			__HAL_RCC_GPIOK_CLL_ENABLE();
			break;
#endif
		default:
			return;
	}
	//RCC_AHB1PeriphClockCmd(BaseAddr, ENABLE);
}
/*#####################################################*/
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int Pin, gpio_type_enum function, bool Multipin)
{
	new_gpio* GpioStruct = new_(new_gpio);
	if(GpioStruct == NULL)
		return NULL;
	unsigned int BaseAddr = 0;
	switch (PortNr)
	{
#ifdef GPIOA
		case 0:
			BaseAddr = (unsigned int)GPIOA;
			break;
#endif
#ifdef GPIOB
		case 1:
			BaseAddr = (unsigned int)GPIOB;
			break;
#endif
#ifdef GPIOC
		case 2:
			BaseAddr = (unsigned int)GPIOC;
			break;
#endif
#ifdef GPIOD
		case 3:
			BaseAddr = (unsigned int)GPIOD;
			break;
#endif
#ifdef GPIOE
		case 4:
			BaseAddr = (unsigned int)GPIOE;
			break;
#endif
#ifdef GPIOF
		case 5:
			BaseAddr = (unsigned int)GPIOF;
			break;
#endif
#ifdef GPIOG
		case 6:
			BaseAddr = (unsigned int)GPIOG;
			break;
#endif
#ifdef GPIOH
		case 7:
			BaseAddr = (unsigned int)GPIOH;
			break;
#endif
#ifdef GPIOI
		case 8:
			BaseAddr = (unsigned int)GPIOI;
			break;
#endif
#ifdef GPIOJ
		case 9:
			BaseAddr = (unsigned int)GPIOJ;
			break;
#endif
#ifdef GPIOK
		case 10:
			BaseAddr = (unsigned int)GPIOK;
			break;
#endif
#ifdef GPIOL
		case 11:
			BaseAddr = (unsigned int)GPIOL;
			break;
#endif
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
	if(!gpio_struct)
		return;
	HAL_GPIO_DeInit((GPIO_TypeDef*)gpio_struct->BaseAddr, 1 << gpio_struct->Pin);
	free(gpio_struct);
}
/*#####################################################*/
bool _gpio_out(new_gpio *gpio_struct, unsigned int State)
{
	if(!gpio_struct)
		return false;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;
	if(gpio_struct->Multipin)
	{
		//BaseAddr->BSRR |= State & gpio_struct->Pin;
		//BaseAddr->BSRR |= ((~State) & gpio_struct->Pin) << 16;
		BaseAddr->ODR = (BaseAddr->ODR & ~gpio_struct->Pin) | (State & gpio_struct->Pin);
	}
	else
	{
		unsigned int state = State;
		if(gpio_struct->inverse)
			state = (~state) & 0x01;
		if(state)
			BaseAddr->BSRR |= 1 << gpio_struct->Pin;
		else
			BaseAddr->BSRR |= 1 << (gpio_struct->Pin + 16);
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
	if(!gpio_struct)
		return -1;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;
	if(gpio_struct->Multipin)
	{
		/*int cnt = 0;
		int returned_state = 0;
		for(; cnt < 16; cnt ++)
		{
			if(gpio_struct->Pin & (1 << cnt))
				returned_state |= HAL_GPIO_ReadPin(BaseAddr, 1 << cnt) << cnt;
		}*/
		return BaseAddr->IDR & gpio_struct->Pin;
	}
	else
	{
		if(gpio_struct->inverse)
			return ~(HAL_GPIO_ReadPin(BaseAddr, 1 << gpio_struct->Pin));
		else
			return HAL_GPIO_ReadPin(BaseAddr, 1 << gpio_struct->Pin);
	}
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
				BaseAddr->PUPDR &= ~(3 << (cnt * 2));
				if(value) BaseAddr->PUPDR |= (((uint32_t)GPIO_PULLUP) << (cnt * 2));
				else BaseAddr->PUPDR |= (((uint32_t)GPIO_PULLDOWN) << (cnt * 2));
			}
			tmp = tmp >> 1;
		}
	}
	else
	{
		BaseAddr->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)gpio_struct->Pin * 2));
		BaseAddr->PUPDR &= ~(3 << (gpio_struct->Pin * 2));
		if(value) BaseAddr->PUPDR |= (((uint32_t)GPIO_PULLUP) << (gpio_struct->Pin * 2));
		else BaseAddr->PUPDR |= (((uint32_t)GPIO_PULLDOWN) << (gpio_struct->Pin * 2));
	}
	return true;
}
/*#####################################################*/
bool _gpio_function_set(new_gpio *gpio_struct, gpio_type_enum function)
{
	if(!gpio_struct) return false;
	GPIO_TypeDef *BaseAddr = (GPIO_TypeDef *)gpio_struct->BaseAddr;

	GPIO_InitTypeDef  GPIO_InitStructure;
	if(gpio_struct->Multipin == false)
		GPIO_InitStructure.Pin = 1 << gpio_struct->Pin;
	else
		GPIO_InitStructure.Pin = gpio_struct->Pin;
	switch(function)
	{
	case GPIO_AIN:
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		break;
	case GPIO_IN_FLOATING:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		break;
	case GPIO_IN_PULL_DOWN:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		break;
	case GPIO_IN_PULL_UP:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		break;
	case GPIO_OUT_OPEN_DRAIN:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		break;
	case GPIO_OUT_PUSH_PULL:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		break;
	default:
		return false;

	}
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init((GPIO_TypeDef *)BaseAddr, &GPIO_InitStructure);
	return true;
}
/*#####################################################*/


