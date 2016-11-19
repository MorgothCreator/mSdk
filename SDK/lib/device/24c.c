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
    unsigned char addr[2];
    addr[0] = Addr>>8;
    addr[1] = Addr;
    if(twi.trx(TwiStruct, MasterSlaveAddr | ((Addr >> 16) & 0x01), addr, 2, Buff, DataLen) == false)
    	return false;
    return true;
}
/*#####################################################*/
bool E2promWrite(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned long Addr, unsigned char *Buff, unsigned int DataLen)
{
	TwiStruct->MasterSlaveAddr = MasterSlaveAddr | ((Addr >> 16) & 0x01);
    unsigned int i;

    unsigned char *buff = malloc(DataLen + 2);
    buff[0] = Addr>>8;
    buff[1] = Addr;

    for (i = 0; i < DataLen; i++ )
    {
    	buff[i + 2] = Buff[i];
    }
    return twi.tx(TwiStruct, MasterSlaveAddr | ((Addr >> 16) & 0x01), buff, DataLen + 2);
}
/*#####################################################*/
bool E2promRead_max16kb(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned int Addr, unsigned char *Buff, unsigned int DataLen)
{
	TwiStruct->MasterSlaveAddr = MasterSlaveAddr | ((Addr >> 8) & 0x07);
    unsigned int i;

    unsigned char addr = Addr;

    if(twi.trx(TwiStruct, MasterSlaveAddr | ((Addr >> 8) & 0x07), &addr, 1, Buff, DataLen) == false)
    	return false;

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
    unsigned char *buff = malloc(DataLen + 1);
    buff[0] = Addr;
    for (i = 0; i < DataLen; i++ )
    {
    	buff[i + 1] = Buff[i];
    }
    return twi.tx(TwiStruct, MasterSlaveAddr | ((Addr >> 8) & 0x07), buff, DataLen + 1);
}
/*#####################################################*/



