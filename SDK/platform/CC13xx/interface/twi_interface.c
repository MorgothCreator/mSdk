/*
 * twi_interface.c
 *
 * Created: 2/8/2013 5:00:27 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#include "twi_interface.h"
//#include "driver/twi.h"
//#include "int/int_twi.h"
//#####################################################
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	return false;
}
/*#####################################################*/
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	return false;
}
/*#####################################################*/
bool _twi_open(new_twi* TwiStruct)
{
	return false;
}
/*#####################################################*/
void _twi_close(new_twi* TwiStruct)
{
	//TWI_close(TwiStruct);
}
/*#####################################################*/
