/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */

#include "mcspi_interface.h"
//#include "driver/spi.h"

extern unsigned long CoreFreq;

bool _mcspi_open(new_mcspi *McspiStruct)
{
	return true;//spi_enable(McspiStruct);
}
/*#####################################################*/
void _mcspi_close(new_mcspi *McspiStruct)
{
	//spi_disable(McspiStruct);
}
/*#####################################################*/
bool _mcspi_transfer(Mcspi_t *McspiStruct)
{
	/*McspiStruct->numOfBytes = NumOfBytesSend + NumOfBytesReceive;
	unsigned char response = SPI_MasterInterruptTransceivePacket(McspiStruct);
	//memcpy(McspiStruct->Buff, McspiStruct->Buff + NumOfBytesSend, NumOfBytesReceive);
	if(response) return false;*/
	return true;
}
/*#####################################################*/
