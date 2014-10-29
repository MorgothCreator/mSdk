/*
 *  lib/device/tps65910a.c
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

/*#####################################################*/
#include "sys/plat_properties.h"
#if (defined USE_TPS65910A) || (defined evmAM335x) || (defined evmskAM335x) || (defined devkit8600)
#include "tps65910a.h"
#include "api/twi_api.h"
#include "include/hw/hw_tps65910.h"
#include "include/hw/hw_types.h"
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*
** Configure vdd2 for various parameters such as
** 		-	Multiplier
** 		-	Maximum Load Current
** 		-	Time step - voltage change per us(micro sec)
** 		-	Supply state (on (high/low power mode), off)
*/
void configureVdd2(new_twi* TwiStruct,unsigned int opVolMultiplier, unsigned maxLoadCurrent,
							unsigned int timeStep, unsigned int supplyState)
{
	TwiStruct->TxBuff[0] = VDD2_REG;
	TwiStruct->TxBuff[1] = (
	               (opVolMultiplier << PMIC_VDD2_REG_VGAIN_SEL_SHIFT) |
		       (maxLoadCurrent << PMIC_VDD2_REG_ILMAX_SHIFT)      |
		       (timeStep << PMIC_VDD2_REG_TSTEP_SHIFT)            |
		       (supplyState << PMIC_VDD2_REG_ST_SHIFT)
					 );
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CTransmit(TwiStruct, 2);
}


/*
** Select the VDD2 value. VDD2_OP_REG or VDD2_SR_REG.
*/
void selectVdd2Source(new_twi* TwiStruct,unsigned int vdd1Source)
{
    /*	Read reg value	*/
	TwiStruct->TxBuff[0] = VDD2_OP_REG;
	TwiStruct->RxBuff[0] = 0; // clear receive buffer
	TwiStruct->RxBuff[0] = 0;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CReception(TwiStruct, 1, 1);

    /*	Modify reg value	*/
    vdd1Source = (TwiStruct->RxBuff[0] & (~PMIC_VDD2_OP_REG_CMD)) |
		 (vdd1Source << PMIC_VDD2_OP_REG_CMD_SHIFT);

	/*	Write reg value	*/
    TwiStruct->TxBuff[0] = VDD2_OP_REG;
    TwiStruct->TxBuff[1] = vdd1Source;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;

	SetupI2CTransmit(TwiStruct, 2);
}


/*
** set VDD2_OP voltage value
*/
void setVdd2OpVoltage(new_twi* TwiStruct,unsigned int opVolSelector)
{
    /*	Read reg value	*/
	TwiStruct->TxBuff[0] = VDD2_OP_REG;
	TwiStruct->RxBuff[0] = 0; // clear receive buffer
	TwiStruct->RxBuff[1] = 0;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CReception(TwiStruct, 1, 1);

    /*	Modify reg value	*/
    opVolSelector = (TwiStruct->RxBuff[0] & (~PMIC_VDD2_OP_REG_SEL)) |
		    (opVolSelector << PMIC_VDD2_OP_REG_SEL_SHIFT);

    /*	Write reg value	*/
    TwiStruct->TxBuff[0] = VDD2_OP_REG;
    TwiStruct->TxBuff[1] = opVolSelector;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CTransmit(TwiStruct, 2);

     /*	Read reg value to verify */
	TwiStruct->TxBuff[0] = VDD2_OP_REG;
	TwiStruct->RxBuff[0] = 0; // clear receive buffer
	TwiStruct->RxBuff[1] = 0;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;

	SetupI2CReception(TwiStruct, 1, 1);

    while((TwiStruct->RxBuff[0] & PMIC_VDD2_OP_REG_SEL) != (opVolSelector << PMIC_VDD2_OP_REG_SEL_SHIFT));
}


/*
** set VDD2_SR voltage value
*/
void setVdd2SrVoltage(new_twi* TwiStruct,unsigned int opVolSelector)
{
    /*	Write reg value	*/
	TwiStruct->TxBuff[0] = VDD2_SR_REG;
	TwiStruct->TxBuff[1] = opVolSelector;
	TwiStruct->tCount = 0;
	SetupI2CTransmit(TwiStruct, 2);
}


/*
** Select I2C interface whether SR I2C or Control I2C
*/
void selectI2CInstance(new_twi* TwiStruct,unsigned int i2cInstance)
{
    /*	Read reg value	*/
	TwiStruct->TxBuff[0] = VDD2_OP_REG;
	TwiStruct->RxBuff[0] = 0; // clear receive buffer
	TwiStruct->RxBuff[1] = 0;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;

	SetupI2CReception(TwiStruct, 1, 1);

    /*	Modify reg value */
    i2cInstance = (TwiStruct->RxBuff[0] & PMIC_DEVCTRL_REG_SR_CTL_I2C_SEL) |
		  (i2cInstance << PMIC_DEVCTRL_REG_SR_CTL_I2C_SEL_SHIFT);

    /*	Write reg value	*/
    TwiStruct->TxBuff[0] = DEVCTRL_REG;
    TwiStruct->TxBuff[1] = i2cInstance;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;

	SetupI2CTransmit(TwiStruct, 2);
}

/*
** Configure VDD1 for various parameters such as
** 		-	Multiplier
** 		-	Maximum Load Current
** 		-	Time step - voltage change per us(micro sec)
** 		-	Supply state (on (high/low power mode), off)
*/
void configureVdd1(new_twi* TwiStruct,unsigned int opVolMultiplier, unsigned maxLoadCurrent,
							unsigned int timeStep, unsigned int supplyState)
{
	TwiStruct->TxBuff[0] = VDD1_REG;
	TwiStruct->TxBuff[1] = (
						(opVolMultiplier << PMIC_VDD1_REG_VGAIN_SEL_SHIFT) |
						(maxLoadCurrent << PMIC_VDD1_REG_ILMAX_SHIFT) |
						(timeStep << PMIC_VDD1_REG_TSTEP_SHIFT) |
						(supplyState << PMIC_VDD1_REG_ST_SHIFT)
					 );
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CTransmit(TwiStruct, 2);
}



/*
** Select the VDD1 value. VDD1_OP_REG or VDD1_SR_REG.
*/
void selectVdd1Source(new_twi* TwiStruct,unsigned int vdd1Source)
{
    /*	Read reg value	*/
	TwiStruct->TxBuff[0] = VDD1_OP_REG;
	TwiStruct->RxBuff[0] = 0; // clear receive buffer
	TwiStruct->RxBuff[1] = 0;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CReception(TwiStruct, 1, 1);

    /*	Modify reg value */
    vdd1Source = (TwiStruct->RxBuff[0] & (~PMIC_VDD1_OP_REG_CMD)) |
		 (vdd1Source << PMIC_VDD1_OP_REG_CMD_SHIFT);

     /*	Write reg value	*/
    TwiStruct->TxBuff[0] = VDD1_OP_REG;
    TwiStruct->TxBuff[1] = vdd1Source;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CTransmit(TwiStruct, 2);
}


/*
** set VDD1_OP voltage value
*/
void setVdd1OpVoltage(new_twi* TwiStruct,unsigned int opVolSelector)
{
    /*	Read reg value	*/
	TwiStruct->TxBuff[0] = VDD1_OP_REG;
	TwiStruct->RxBuff[0] = 0; // clear receive buffer
	TwiStruct->RxBuff[1] = 0;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CReception(TwiStruct, 1, 1);

    /*	Modify reg value */
    opVolSelector = (TwiStruct->RxBuff[0] & (~PMIC_VDD1_OP_REG_SEL)) |
		    (opVolSelector << PMIC_VDD1_OP_REG_SEL_SHIFT);

    /*	Write reg value	*/
    TwiStruct->TxBuff[0] = VDD1_OP_REG;
    TwiStruct->TxBuff[1] = opVolSelector;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CTransmit(TwiStruct, 2);

    /*	Read reg value to verify */
	TwiStruct->TxBuff[0] = VDD1_OP_REG;
	TwiStruct->RxBuff[0] = 0; // clear receive buffer
	TwiStruct->RxBuff[1] = 0;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	SetupI2CReception(TwiStruct, 1, 1);

    while((TwiStruct->RxBuff[0] & PMIC_VDD1_OP_REG_SEL) != (opVolSelector << PMIC_VDD1_OP_REG_SEL_SHIFT));

}
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
void _pmic_config(new_twi* TwiStruct)
{
    /* Configure PMIC slave address */
    I2CMasterSlaveAddrSet(TwiStruct->BaseAddr, PMIC_CNTL_I2C_SLAVE_ADDR);

	/* Select SR I2C(0) */
    selectI2CInstance(TwiStruct, PMIC_DEVCTRL_REG_SR_CTL_I2C_SEL_CTL_I2C);

    /* Configure vdd1- need to validate these parameters */
    configureVdd1(TwiStruct, PMIC_VDD1_REG_VGAIN_SEL_X1, PMIC_VDD1_REG_ILMAX_1_5_A,
		 PMIC_VDD1_REG_TSTEP_12_5, PMIC_VDD1_REG_ST_ON_HI_POW);

    /* Select the source for VDD1 control */
    selectVdd1Source(TwiStruct, PMIC_VDD1_OP_REG_CMD_OP);

}
/*#####################################################*/
/**
 * tps65910_reg_read() - Generic function that can read a TPS65217 register
 * @regOffset:          Source register address
 * @src_val:          Address of destination variable
 */
void _pmic_reg_read(new_twi* TwiStruct,unsigned char regOffset, unsigned char* dest)
{
}
/*#####################################################*/
/**
 *  tps65910a_reg_write() - Generic function that can write a TPS65217 PMIC
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
}
/*#####################################################*/
/**
 *  tps65910a_voltage_update() - Controls output voltage setting for the DCDC1,
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
}
/*#####################################################*/
void _pmic_wled_enable(new_twi* TwiStruct)
{
}
/*#####################################################*/
void _pmic_wled_level(new_twi* TwiStruct, unsigned char level)
{
}
/*#####################################################*/
#endif



