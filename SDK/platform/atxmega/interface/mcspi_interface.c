/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */ 

#include "mcspi_interface.h"
#include "driver/spi.h"

extern unsigned long CoreFreq;

bool _mcspi_open(new_mcspi *McspiStruct)
{
	return spi_enable(McspiStruct);
}
/*#####################################################*/
void _mcspi_close(new_mcspi *McspiStruct)
{
	spi_disable(McspiStruct);
}
/*#####################################################*/
bool _mcspi_transfer(Mcspi_t *McspiStruct)
{
	unsigned char response = SPI_MasterInterruptTransceivePacket(McspiStruct);
	//memcpy(McspiStruct->Buff, McspiStruct->Buff + NumOfBytesSend, NumOfBytesReceive);
	if(response) return false;
	return true;
}
/*#####################################################*/
unsigned char _mcspi_SendByte(Mcspi_t *McspiStruct, unsigned char byte)
{
	return 0;
}
/*#####################################################*/
bool _mcspi_set_baud(Mcspi_t *McspiStruct, unsigned long baud)
{
	return false;
}
