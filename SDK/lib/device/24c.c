/*
 *  lib/device/24c.c
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

#include "24c.h"
#include "api/twi_api.h"
/*#####################################################*/
bool E2promRead(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned long Addr, unsigned char *Buff, unsigned int DataLen)
{
	TwiStruct->MasterSlaveAddr = MasterSlaveAddr | ((Addr >> 16) & 0x01);
    unsigned int i;

    TwiStruct->TxBuff[0] = Addr>>8;
    TwiStruct->TxBuff[1] = Addr;

    TwiStruct->tCount = 0;
    TwiStruct->rCount = 0;
    if(SetupI2CReception(TwiStruct, 2, DataLen) == false) return false;

    for (i = 0; i < DataLen; i++ )
    {
    	Buff[i] = TwiStruct->RxBuff[i];
    }
    return true;
}
/*#####################################################*/
bool E2promWrite(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned long Addr, unsigned char *Buff, unsigned int DataLen)
{
	TwiStruct->MasterSlaveAddr = MasterSlaveAddr | ((Addr >> 16) & 0x01);
    unsigned int i;

    TwiStruct->TxBuff[0] = Addr>>8;
    TwiStruct->TxBuff[1] = Addr;

    TwiStruct->tCount = 0;
    TwiStruct->rCount = 0;
    for (i = 0; i < DataLen; i++ )
    {
    	TwiStruct->TxBuff[i + 2] = Buff[i];
    }
    return SetupI2CTransmit(TwiStruct, DataLen + 2);
}
/*#####################################################*/
bool E2promRead_max16kb(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned int Addr, unsigned char *Buff, unsigned int DataLen)
{
	TwiStruct->MasterSlaveAddr = MasterSlaveAddr | ((Addr >> 8) & 0x07);
    unsigned int i;

    TwiStruct->TxBuff[0] = Addr;

    TwiStruct->tCount = 0;
    TwiStruct->rCount = 0;
    if(SetupI2CReception(TwiStruct, 1, DataLen) == false) return false;

    for (i = 0; i < DataLen; i++ )
    {
    	Buff[i] = TwiStruct->RxBuff[i];
    }
    return true;
}
/*#####################################################*/
bool E2promWrite_max16kb(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned int Addr, unsigned char *Buff, unsigned int DataLen)
{
	TwiStruct->MasterSlaveAddr = MasterSlaveAddr | ((Addr >> 8) & 0x07);
    unsigned int i;

    TwiStruct->TxBuff[0] = Addr;

    TwiStruct->tCount = 0;
    TwiStruct->rCount = 0;
    for (i = 0; i < DataLen; i++ )
    {
    	TwiStruct->TxBuff[i + 1] = Buff[i];
    }
    return SetupI2CTransmit(TwiStruct, DataLen + 1);
}
/*#####################################################*/



