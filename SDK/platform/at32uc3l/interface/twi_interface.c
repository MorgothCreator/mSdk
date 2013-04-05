/*
 * twi_interface.c
 *
 * Created: 2/8/2013 5:00:27 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#include "twi_interface.h"
#include "driver/twi.h"
#include "driver/gpio.h"
//#include "int/int_twi.h"
//#####################################################
/*#####################################################*/
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	return true;//twi_master_write(TwiStruct, TransmitBytes);
}
/*#####################################################*/
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	return true;//twi_master_read(TwiStruct, TransmitBytes, ReceiveBytes);
}
/*#####################################################*/
bool _twi_open(new_twi* TwiStruct)
{
	gpio_map_t TWI_GPIO_MAP = {{ 0, 0 }, { 0, 0 }};

	switch (TwiStruct->TwiNr)
	{
		case 0:
			TwiStruct->BaseAddr = (unsigned int)(void*)&AVR32_TWIM0;
			switch(TwiStruct->SclPin)
			{
				case 0:
					TWI_GPIO_MAP[0].pin = AVR32_TWIMS0_TWCK_0_0_PIN;
					TWI_GPIO_MAP[0].function = AVR32_TWIMS0_TWCK_0_0_FUNCTION;
					break;
				case 1:
					TWI_GPIO_MAP[0].pin = AVR32_TWIMS0_TWCK_0_1_PIN;
					TWI_GPIO_MAP[0].function = AVR32_TWIMS0_TWCK_0_1_FUNCTION;
					break;
				case 2:
					TWI_GPIO_MAP[0].pin = AVR32_TWIMS0_TWCK_0_2_PIN;
					TWI_GPIO_MAP[0].function = AVR32_TWIMS0_TWCK_0_2_FUNCTION;
					break;
				case 3:
					TWI_GPIO_MAP[0].pin = AVR32_TWIMS0_TWCK_0_3_PIN;
					TWI_GPIO_MAP[0].function = AVR32_TWIMS0_TWCK_0_3_FUNCTION;
					break;
			}
			switch(TwiStruct->SdaPin)
			{
				case 0:
					TWI_GPIO_MAP[1].pin = AVR32_TWIMS0_TWD_0_0_PIN;
					TWI_GPIO_MAP[1].function = AVR32_TWIMS0_TWD_0_0_FUNCTION;
					break;
				case 1:
					TWI_GPIO_MAP[1].pin = AVR32_TWIMS0_TWD_0_1_PIN;
					TWI_GPIO_MAP[1].function = AVR32_TWIMS0_TWD_0_1_FUNCTION;
					break;
				case 2:
					TWI_GPIO_MAP[1].pin = AVR32_TWIMS0_TWD_0_0_PIN;
					TWI_GPIO_MAP[1].function = AVR32_TWIMS0_TWD_0_2_FUNCTION;
					break;
			}
			break;
		case 1:
			TwiStruct->BaseAddr = (unsigned int)(void*)&AVR32_TWIM1;
			switch(TwiStruct->SclPin)
			{
				case 0:
					TWI_GPIO_MAP[0].pin = AVR32_TWIMS1_TWCK_0_0_PIN;
					TWI_GPIO_MAP[0].function = AVR32_TWIMS1_TWCK_0_0_FUNCTION;
					break;
				case 1:
					TWI_GPIO_MAP[0].pin = AVR32_TWIMS1_TWCK_0_1_PIN;
					TWI_GPIO_MAP[0].function = AVR32_TWIMS1_TWCK_0_1_FUNCTION;
					break;
			}
			switch(TwiStruct->SdaPin)
			{
				case 0:
					TWI_GPIO_MAP[1].pin = AVR32_TWIMS1_TWD_0_PIN;
					TWI_GPIO_MAP[1].function = AVR32_TWIMS1_TWD_0_FUNCTION;
					break;
			}
			break;
	}		
	if(gpio_enable_module (TWI_GPIO_MAP, sizeof (TWI_GPIO_MAP) / sizeof (TWI_GPIO_MAP[0])) != GPIO_SUCCESS) return false;
	sysclk_enable_peripheral_clock((avr32_twim_t*)TwiStruct->BaseAddr);
	//twi_options_t opt;// = {0, 0, 0};
	///opt.chip = TwiStruct->MasterSlaveAddr;
	//opt.pba_hz = sysclk_get_pba_hz();
	//opt.speed = TwiStruct->BaudRate;
	//twi_master_init((volatile avr32_twim_t*)TwiStruct->BaseAddr, &opt, TwiStruct->TwiNr);
	return true;
}
/*#####################################################*/
void _twi_close(new_twi* TwiStruct)
{
	//TWI_close(TwiStruct);
}
/*#####################################################*/
