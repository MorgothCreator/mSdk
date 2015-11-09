/*
 * mpr121.c
 *
 * Created: 5/31/2015 12:09:32 PM
 *  Author: eu
 */ 

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
#include <stdbool.h>
#include "config.h"
#include "mpr121.h"
#include "api/twi_api.h"
#include "api/twi_def.h"


bool mpr121_read_short(mpr121_t *structure, unsigned char reg, unsigned short *return_data)
{
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPR121_ADDR;
	TwiStruct->TxBuff[0] = reg;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	memcpy(return_data, TwiStruct->RxBuff, 2);
	return true;
}

bool mpr121_read(mpr121_t *structure, unsigned char reg, unsigned char *return_data)
{
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPR121_ADDR;
	TwiStruct->TxBuff[0] = reg;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	*return_data = TwiStruct->RxBuff[0];
	return true;
}

bool mpr121_write(mpr121_t *structure, unsigned char reg, unsigned char data)
{
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPR121_ADDR;
	TwiStruct->TxBuff[0] = reg;
	TwiStruct->TxBuff[1] = data;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

#ifdef FLASH_DEVICE
static const mpr121_setup mpr121_set[] PROGMEM = {
#else
static const mpr121_setup mpr121_set[] = {
#endif
	{MPR_RESET, 0x63},
	{MHD_R, 0x01},
	{NCL_R, 0x00},
	{FDL_R, 0x00},
	{NHD_F, 0x01},
	{NCL_F, 0xFF},
	{FDL_F, 0x02},

	{ELE0_T, TOU_THRESH},
	{ELE0_R, REL_THRESH},
	
	{ELE1_T, TOU_THRESH},
	{ELE1_R, REL_THRESH},
	
	{ELE2_T, TOU_THRESH},
	{ELE2_R, REL_THRESH},
	
	{ELE3_T, TOU_THRESH},
	{ELE3_R, REL_THRESH},
	
	{ELE4_T, TOU_THRESH},
	{ELE4_R, REL_THRESH},
	
	{ELE5_T, TOU_THRESH},
	{ELE5_R, REL_THRESH},
	
	{ELE6_T, TOU_THRESH},
	{ELE6_R, REL_THRESH},
	
	{ELE7_T, TOU_THRESH},
	{ELE7_R, REL_THRESH},
	
	{ELE8_T, TOU_THRESH},
	{ELE8_R, REL_THRESH},
	
	{ELE9_T, TOU_THRESH},
	{ELE9_R, REL_THRESH},
	
	{ELE10_T, TOU_THRESH},
	{ELE10_R, REL_THRESH},
	
	{ELE11_T, TOU_THRESH},
	{ELE11_R, REL_THRESH},
	
	{FIL_CFG, 0x01},
	{ELE_CFG, 0x0C},
	
	};

void mpr121_idle(mpr121_t *structure, mpr121_ret_t *return_keys)
{
	if(!structure->TWI)
		return false;
	/* Check if interrupt pin is provided, if not skip irq check. */
	if(structure->IrqPin)
	{
		if(gpio_in(structure->IrqPin))
			return false;
	}
	unsigned short keys;
	mpr121_read_short(structure, 0, &keys);
	unsigned short keys_back = keys;
	bool return_new_event = false;
	int cnt = 0;
	mpr121_ret_t return_keys_tmp;
	return_keys_tmp.pushed = 0;
	return_keys_tmp.released = 0;
	for(; cnt < 12; cnt++)
	{
		if((keys & 0x01) != (structure->old_keys & 0x01))
		{
			if(keys & 0x01)
			{
				return_keys_tmp.pushed |= 1 << cnt;
				return_new_event = true;
			}
			else
			{
				return_keys_tmp.released |= 1 << cnt;
				return_new_event = true;
			}
		}
		keys = keys >> 1;
		structure->old_keys = structure->old_keys >> 1;
	}
	structure->old_keys = keys_back;
	memcpy(&return_keys_tmp, return_keys, sizeof(mpr121_ret_t));
	return return_new_event;
}

bool mpr121_init(mpr121_t *structure)
{
  int cnt = 0;
  for (; cnt < sizeof(mpr121_set) / sizeof(mpr121_setup); cnt++)
  {
#ifdef FLASH_DEVICE
	if(!mpr121_write(structure, pgm_read_byte(&mpr121_set[cnt].addr) , pgm_read_byte(&mpr121_set[cnt].data)))
		return false;
#else
	if(!mpr121_write(structure, mpr121_set[cnt].addr , mpr121_set[cnt].data))
		return false;
#endif
  }
  /*mpr121Write(ATO_CFG0, 0x0B);
  mpr121Write(ATO_CFGU, 0xC9);	// USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   mpr121Write(ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
  mpr121Write(ATO_CFGT, 0xB5);*/	// Target = 0.9*USL = 0xB5 @3.3V
  return true;
}

