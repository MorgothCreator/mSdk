/*
 *  lib/device/tps65217.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "board_properties.h"
#ifdef USE_TPS65217
#include "tps65217.h"
#include "../api/twi_api.h"
#include "include/hw/hw_tps65217.h"
#include "include/hw/hw_types.h"
/*#####################################################*/
void _pmic_config(new_twi* TwiStruct)
{
    if(!TwiStruct) return;
	unsigned char pmic_status = 0;

    /* Configure PMIC slave address */
    I2CMasterSlaveAddrSet(TwiStruct->BaseAddr, PMIC_TPS65217_I2C_SLAVE_ADDR);

    _pmic_reg_read(TwiStruct, STATUS, &pmic_status);

    /* Increase USB current limit to 1300mA */
    _pmic_reg_write(TwiStruct, PROT_LEVEL_NONE, POWER_PATH, USB_INPUT_CUR_LIMIT_1300MA,
                       USB_INPUT_CUR_LIMIT_MASK);

    /* Set DCDC2 (MPU) voltage to 1.275V */
    pmic_voltage_update(TwiStruct, DEFDCDC2, DCDC_VOLT_SEL_1275MV);

    /* Set LDO3, LDO4 output voltage to 3.3V */
    _pmic_reg_write(TwiStruct, PROT_LEVEL_2, DEFLS1, LDO_VOLTAGE_OUT_3_3, LDO_MASK);


    _pmic_reg_write(TwiStruct, PROT_LEVEL_2, DEFLS2, LDO_VOLTAGE_OUT_3_3, LDO_MASK);
}
/*#####################################################*/
/**
 * tps65217_reg_read() - Generic function that can read a TPS65217 register
 * @regOffset:          Source register address
 * @src_val:          Address of destination variable
 */
void _pmic_reg_read(new_twi* TwiStruct,unsigned char regOffset, unsigned char* dest)
{
    if(!TwiStruct) return;
    /* Configure PMIC slave address */
	TwiStruct->MasterSlaveAddr = PMIC_TPS65217_I2C_SLAVE_ADDR;
	TwiStruct->TxBuff[0] = regOffset;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;

    SetupI2CReception(TwiStruct, 1, 1);

    *dest = TwiStruct->RxBuff[0];
}
/*#####################################################*/
/**
 *  tps65217_reg_write() - Generic function that can write a TPS65217 PMIC
 *                         register or bit field regardless of protection
 *                         level.
 *
 *  @prot_level:        Register password protection.
 *                      use PROT_LEVEL_NONE, PROT_LEVEL_1, or PROT_LEVEL_2
 *  @regOffset:         Register address to write.
 *  @dest_val:          Value to write.
 *  @mask:              Bit mask (8 bits) to be applied.  Function will only
 *                      change bits that are set in the bit mask.
 *
 *  @return:            None.
 */
void _pmic_reg_write(new_twi* TwiStruct,unsigned char port_level, unsigned char regOffset,
                        unsigned char dest_val, unsigned char mask)
{
    if(!TwiStruct) return;
    unsigned char read_val;
    unsigned xor_reg;

    /* Configure PMIC slave address */
	TwiStruct->MasterSlaveAddr = PMIC_TPS65217_I2C_SLAVE_ADDR;
	TwiStruct->TxBuff[0] = regOffset;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;

    if(mask != MASK_ALL_BITS)
    {
    	SetupI2CReception(TwiStruct, 1, 1);

         read_val = TwiStruct->RxBuff[0];
         read_val &= (~mask);
         read_val |= (dest_val & mask);
         dest_val = read_val;
    }

    if(port_level > 0)
    {
         xor_reg = regOffset ^ PASSWORD_UNLOCK;

         TwiStruct->TxBuff[0] = PASSWORD;
         TwiStruct->TxBuff[1] = xor_reg;
         TwiStruct->tCount = 0;

         SetupI2CTransmit(TwiStruct, 2);
    }

    TwiStruct->TxBuff[0] = regOffset;
    TwiStruct->TxBuff[1] = dest_val;
    TwiStruct->tCount = 0;

    SetupI2CTransmit(TwiStruct, 2);

    if(port_level == PROT_LEVEL_2)
    {
    	TwiStruct->TxBuff[0] = PASSWORD;
    	TwiStruct->TxBuff[1] = xor_reg;
    	TwiStruct->tCount = 0;

         SetupI2CTransmit(TwiStruct, 2);

         TwiStruct->TxBuff[0] = regOffset;
         TwiStruct->TxBuff[1] = dest_val;
         TwiStruct->tCount = 0;

         SetupI2CTransmit(TwiStruct, 2);
    }
}
/*#####################################################*/
/**
 *  tps65217_voltage_update() - Controls output voltage setting for the DCDC1,
 *       DCDC2, or DCDC3 control registers in the PMIC.
 *
 *  @dc_cntrl_reg:      DCDC Control Register address.
 *                      Must be DEFDCDC1, DEFDCDC2, or DEFDCDC3.
 *  @volt_sel:          Register value to set.  See PMIC TRM for value set.
 *
 *  @return:            None.
 */
void pmic_voltage_update(new_twi* TwiStruct,unsigned char dc_cntrl_reg, unsigned char volt_sel)
{
    if(!TwiStruct) return;
    /* set voltage level */
	_pmic_reg_write(TwiStruct, PROT_LEVEL_2, dc_cntrl_reg, volt_sel, MASK_ALL_BITS);

    /* set GO bit to initiate voltage transition */
	_pmic_reg_write(TwiStruct, PROT_LEVEL_2, DEFSLEW, DCDC_GO, DCDC_GO);
}
/*#####################################################*/
void _pmic_wled_enable(new_twi* TwiStruct)
{
    if(!TwiStruct) return;
	_pmic_reg_write(TwiStruct, PROT_LEVEL_NONE, WLEDCTRL1, WLED_CURENT_SYNK_ENABLE_BITMASK | WLED_PWM_DIMMING_FREQ_200, 0xFF );
	_pmic_reg_write(TwiStruct, PROT_LEVEL_NONE, WLEDCTRL2, 80, 0xFF );
}
/*#####################################################*/
void _pmic_wled_level(new_twi* TwiStruct, unsigned char level)
{
    if(!TwiStruct) return;
	if(level > 100) return;
	_pmic_reg_write(TwiStruct, PROT_LEVEL_NONE, WLEDCTRL2, level, 0xFF );
}
/*#####################################################*/
#endif


