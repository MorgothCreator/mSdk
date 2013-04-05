/*
 * twi.c
 *
 * Created: 2/8/2013 5:10:05 PM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include "twi.h"
#include "api/twi_def.h"
#include "int/int_twi.h"

extern unsigned long CoreFreq;

TWI_t* Twi_GetSetBaseAddress(Twi_t *TwiStruct)
{
	TWI_t* Temp = 0;
	switch(TwiStruct->TwiNr)
	{
#ifdef use_twi0
		case(0):
			PORTC.DIRCLR = 1<<0;
			PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc;
			PORTC.DIRCLR = 1<<1;
			PORTC.PIN1CTRL = PORT_OPC_PULLUP_gc;
			TWI_ServiceAddresses.Twi0 = TwiStruct;
			Temp = &TWIC;
			break; 
#endif
#ifdef use_twi1
		case(1):
			PORTD.DIRCLR = 1<<0;
			PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc;
			PORTD.DIRCLR = 1<<1;
			PORTD.PIN1CTRL = PORT_OPC_PULLUP_gc;
			TWI_ServiceAddresses.Twi1 = TwiStruct;
			Temp = &TWID;
			break; 
#endif
#ifdef use_twi2
		case(2):
			PORTE.DIRCLR = 1<<0;
			PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc;
			PORTE.DIRCLR = 1<<1;
			PORTE.PIN1CTRL = PORT_OPC_PULLUP_gc;
			TWI_ServiceAddresses.Twi2 = TwiStruct;
			Temp = &TWIE;
			break; 
#endif
#ifdef use_twi3
		case(3):
			PORTF.DIRCLR = 1<<0;
			PORTF.PIN0CTRL = PORT_OPC_PULLUP_gc;
			PORTF.DIRCLR = 1<<1;
			PORTF.PIN1CTRL = PORT_OPC_PULLUP_gc;
			TWI_ServiceAddresses.Twi3 = TwiStruct;
			Temp = &TWIF;
			break; 
#endif
	}
	TwiStruct->BaseAddr = (unsigned int)(void*)Temp;
	return Temp;
}
/*! \brief Initialize the TWI module.
 *
 *  TWI module initialization function.
 *  Enables master read and write interrupts.
 *  Remember to enable interrupts globally from the main application.
 *
 *  \param twi                      The TWI_Master_t struct instance.
 *  \param module                   The TWI module to use.
 *  \param intLevel                 Master interrupt level.
 *  \param baudRateRegisterSetting  The baud rate register value.
 */
/*void TWI_MasterInit(TWI_Master_t *twi, TWI_t *module, TWI_MASTER_INTLVL_t intLevel, unsigned char baudRateRegisterSetting)
{
	twi->interface = module;
	twi->interface->MASTER.CTRLA = intLevel | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;
	twi->interface->MASTER.BAUD = baudRateRegisterSetting;
	twi->interface->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}*/
//#####################################################
bool TWI_open(Twi_t* TwiStruct) 
{
	if(!TwiStruct) return false;
	TWI_t* Return = Twi_GetSetBaseAddress(TwiStruct);
	
	unsigned char* BuffersAddr = (unsigned char*)calloc(1, TwiStruct->RxBuffSize);
	if(BuffersAddr == 0) return false;
	TwiStruct->RxBuff = BuffersAddr;
	BuffersAddr = (unsigned char*)calloc(1, TwiStruct->TxBuffSize);
	if(BuffersAddr == 0)
	{
		if(TwiStruct->RxBuff) free(TwiStruct->RxBuff);
		return false;
	}
	TwiStruct->TxBuff = BuffersAddr;
	
	Return->MASTER.BAUD = (CoreFreq/(TwiStruct->BaudRate<<1))-5;
	unsigned char Temp = Return->MASTER.CTRLA;
	if(TwiStruct->Priority) 
	{
		Temp = (Temp&(~TWI_MASTER_INTLVL_gm))|(TwiStruct->Priority<<TWI_MASTER_INTLVL_gp);
		Temp = (Temp&(~TWI_MASTER_RIEN_bm))|(1<<TWI_MASTER_RIEN_bp);
		Temp = (Temp&(~TWI_MASTER_WIEN_bm))|(1<<TWI_MASTER_WIEN_bp);
	}
	else 
	{
		Temp &= (~TWI_MASTER_INTLVL_gm);
		Temp &= (~TWI_MASTER_RIEN_bm);
		Temp &= (~TWI_MASTER_WIEN_bm);
	}
	Temp = (Temp&(~TWI_MASTER_ENABLE_bm))|(1<<TWI_MASTER_ENABLE_bp);
	Return->MASTER.CTRLA = Temp;
	Return->MASTER.CTRLB |= TWI_MASTER_TIMEOUT_gm;
	Return->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
	return true;
}
//#####################################################
bool TWI_reset(Twi_t* TwiStruct)
{
	TWI_t* Return = Twi_GetSetBaseAddress(TwiStruct);
	Return->MASTER.CTRLA &= ~TWI_MASTER_ENABLE_bm;
	_delay_us(10);
	Return->MASTER.CTRLA |= TWI_MASTER_ENABLE_bm;
	return true;
}
//#####################################################
void TWI_close(Twi_t *twi)
{
	TWI_t* module_addr = (TWI_t*)twi->BaseAddr;
	module_addr->MASTER.CTRLA &= ~TWI_MASTER_ENABLE_bm;
	if(twi->RxBuff) free(twi->RxBuff);
	if(twi->TxBuff) free(twi->TxBuff);
	if(twi) free(twi);
}
//#####################################################
