//#######################################################################################
// Spi Driver
//
// File Name            :"IO_Spi.c"
// Location             :"Drivers/IO_Spi.c"
// Title                :Dma IO Driver
// Date                 :12.07.2011
// Version              :1.0
// Target MCU           :All ATXmega
// AUTHOR				:Gheorghiu Iulian
//			Romania
//			morgoth.creator@gmail.com
//			http://digitalelectronicsandprograming.blogspot.com/
//			http://morgothatxmegaprograming.blogspot.com/
//			http://devboardshop.servehttp.com
//
// DESCRIPTION:
//  This is a Hardware Dma driver
//	
//
//#######################################################################################
//							Example of how to init one Spi hardware unit
//
/*	Spi_t SpiMemoryInterface;
	//SpiMemoryInterface.Spi_IntBuffAddr					= (void *)malloc(256);
	//SpiMemoryInterface.Spi_IntBuffPointer				= 0;
	SpiMemoryInterface.Spi_Number						= 1;
	SpiMemoryInterface.Spi_BaudDiv						= _SpiClkDiv4;
	SpiMemoryInterface.Spi_IntLevel						= _Spi_Int_Disabled;
	SpiMemoryInterface.Spi_Clk2X						= true;
	SpiMemoryInterface.Spi_Mode							= _SpiModeRisingSample;
	SpiMemoryInterface.Spi_Master						= true;
	spi_enable(&SpiMemoryInterface,true);*/
//#######################################################################################
//#######################################################################################
//#######################################################################################
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "spi.h"
//#####################################################
SpiServiceAddresses Spi_ServiceAddresses;
extern unsigned long CoreFreq;
//#####################################################
SPI_t *Spi_GetSetBaseAddress(Mcspi_t *SpiStruct,unsigned char SpiNumber)
{
	SPI_t *Temp = 0;
	switch(SpiNumber)
	{
		case(0):
			PORTC.DIRSET = 1<<5|1<<7;
			PORTC.OUTSET = 1<<5|1<<7;
			PORTC.PIN6CTRL = 3<<3;
			Spi_ServiceAddresses.Spi0RamServiceAddr = SpiStruct;
			Temp = &SPIC;
			break; 
		case(1):
			PORTD.DIRSET = 1<<5|1<<7;
			PORTD.OUTSET = 1<<5|1<<7;
			PORTD.PIN6CTRL = 3<<3;
			Spi_ServiceAddresses.Spi1RamServiceAddr = SpiStruct;
			Temp = &SPID;
			break; 
#ifdef SPIE
		case(2):
			PORTE.DIRSET = 1<<5|1<<7;
			PORTE.OUTSET = 1<<5|1<<7;
			PORTE.PIN6CTRL = 3<<3;
			Spi_ServiceAddresses.Spi2RamServiceAddr = SpiStruct;
			Temp = &SPIE;
			break; 
#endif
#ifdef SPIF
		case(3):
			PORTF.DIRSET = 1<<5|1<<7;
			PORTF.OUTSET = 1<<5|1<<7;
			PORTF.PIN6CTRL = 3<<3;
			Spi_ServiceAddresses.Spi3RamServiceAddr = SpiStruct;
			Temp = &SPIF;
			break; 
#endif
	}
	SpiStruct->BaseAddr = (unsigned int)(void *)Temp;
	return Temp;
}
//#####################################################
//#####################################################
bool spi_enable(Mcspi_t *SpiStruct)
{
	SPI_t *ModuleAddress = Spi_GetSetBaseAddress(SpiStruct, SpiStruct->McspiNr);
	//SpiStruct->BaseAddr = (unsigned int)(void *)ModuleAddress;
	if((ModuleAddress->CTRL & (~SPI_ENABLE_bm)) == 0)
	{
		SpiStruct->UserData = calloc(1, sizeof(SPI_UserData_t));
		if(!SpiStruct->UserData) return false;
		SPI_UserData_t *UserData = (SPI_UserData_t *)SpiStruct->UserData;
		UserData->Cs_PinMask = 1 << (SpiStruct->CsPin[0] & 0x07);
		switch (SpiStruct->CsPort[0] >> 3)
		{
#ifdef PORTA
			case 0:
			UserData->Cs_Port = &PORTA;
			break;
#endif
#ifdef PORTB
			case 1:
			UserData->Cs_Port = &PORTB;
			break;
#endif
#ifdef PORTC
			case 2:
			UserData->Cs_Port = &PORTC;
			break;
#endif
#ifdef PORTD
			case 3:
			UserData->Cs_Port = &PORTD;
			break;
#endif
#ifdef PORTE
			case 4:
			UserData->Cs_Port = &PORTE;
			break;
#endif
#ifdef PORTF
			case 5:
			UserData->Cs_Port = &PORTF;
			break;
#endif
#ifdef PORTG
			case 6:
			UserData->Cs_Port = &PORTG;
			break;
#endif
#ifdef PORTH
			case 7:
			UserData->Cs_Port = &PORTH;
			break;
#endif
#ifdef PORTI
			case 8:
			UserData->Cs_Port = &PORTI;
			break;
#endif
#ifdef PORTJ
			case 9:
			UserData->Cs_Port = &PORTJ;
			break;
#endif
#ifdef PORTK
			case 10:
			UserData->Cs_Port = &PORTK;
			break;
#endif
#ifdef PORTL
			case 11:
			UserData->Cs_Port = &PORTL;
			break;
#endif
#ifdef PORTM
			case 12:
			UserData->Cs_Port = &PORTM;
			break;
#endif
#ifdef PORTN
			case 13:
			UserData->Cs_Port = &PORTN;
			break;
#endif
#ifdef PORTO
			case 14:
			UserData->Cs_Port = &PORTO;
			break;
#endif
#ifdef PORTP
			case 15:
			UserData->Cs_Port = &PORTP;
			break;
#endif
#ifdef PORTQ
			case 16:
			UserData->Cs_Port = &PORTQ;
			break;
#endif
#ifdef PORTR
			case 17:
			UserData->Cs_Port = &PORTR;
			break;
#endif
		}
		UserData->Cs_Port->DIRSET = UserData->Cs_PinMask;
		UserData->Cs_Port->OUTSET = UserData->Cs_PinMask;
		unsigned char Temp = ModuleAddress->CTRL;
		Temp = (Temp & (~SPI_MASTER_bm)) | (1 << SPI_MASTER_bp);
		Temp = (Temp & (~SPI_CLK2X_bm)) | (1 << SPI_CLK2X_bp);
		Temp = (Temp & (~SPI_MODE_gm)) | (SpiStruct->Mode << SPI_MODE_gp);
		Temp = (Temp & (~SPI_PRESCALER_gm)) | ((CoreFreq / 2 / SpiStruct->BaudRate) << SPI_PRESCALER_gp);
		Temp = (Temp | (1<<SPI_ENABLE_bp));
		ModuleAddress->CTRL = Temp;
		Temp = ModuleAddress->STATUS;
		ModuleAddress->INTCTRL = (ModuleAddress->INTCTRL & (~SPI_INTLVL_gm))|(SpiStruct->Priority<<SPI_INTLVL_gp);
		return true;
	}
	return false;
}

void spi_disable(Mcspi_t *SpiStruct)
{
		SPI_t *ModuleAddress = (SPI_t *)(void *)SpiStruct->BaseAddr;
		ModuleAddress->CTRL = ModuleAddress->CTRL&(~SPI_ENABLE_bm);
		if(SpiStruct->Buff) free((void *)SpiStruct->Buff);
		if(SpiStruct->UserData) free(SpiStruct->UserData);
}
//#####################################################
//#####################################################
//#####################################################
/*! \brief Pulls SPI SS line(s) low in order to address the slave devices.
 *
 *  \param _port         Pointer to the I/O port where the SS pins are located.
 *  \param _pinBM        A bitmask selecting the pins to pull low.
 *
 *  \retval NA
 */
#define SPI_MasterSSLow(_port, _pinBM) ( (_port)->OUTCLR = (_pinBM) )



/*! \brief Releases SPI SS line(s).
 *
 *  \param _port         Pointer to the I/O port where the SS pins are located.
 *  \param _pinBM        A bitmask selecting the pins to release.
 *
 *  \retval NA
 */
#define SPI_MasterSSHigh(_port, _pinBM) ( (_port)->OUTSET = (_pinBM) )
/*! \brief Common SPI master interrupt service routine.
 *
 *  This function is called by the SPI interrupt service handlers. For each
 *  SPI module that uses this driver, the ISR should call this function with
 *  a pointer to the related SPI_Master_t struct as argument.
 *
 *  \param spi        Pointer to the modules own SPI_Master_t struct.
 */
void SPI_MasterInterruptHandler(Mcspi_t *spi)
{
	unsigned char data;
	unsigned int bytesTransceived = spi->BytesCnt;
	SPI_t* Spi_BaseAddr = (SPI_t *)(void *)spi->BaseAddr;
	SPI_UserData_t *UserData = (SPI_UserData_t *)spi->UserData;
	/* If SS pin interrupt (SS used and pulled low).
	*  No data received at this point. */
	if ( !(Spi_BaseAddr->CTRL & SPI_MASTER_bm) ) {
		//spi->interrupted = true;
		SPI_MasterSSHigh(UserData->Cs_Port, UserData->Cs_PinMask);

		spi->flag = false;
	}

	else {  /* Data interrupt. */

		/* Store received data. */
		data = Spi_BaseAddr->DATA;
		spi->Buff[bytesTransceived] = data;

		/* Next byte. */
		bytesTransceived++;

		/* If more data. */
		if (bytesTransceived < spi->numOfBytes) {
			/* Put data byte in transmit data register. */
			data = spi->Buff[bytesTransceived];
			Spi_BaseAddr->DATA = data;
		}

		/* Transmission complete. */
		else {

			/* Release SS to slave(s). */
			SPI_MasterSSHigh(UserData->Cs_Port, UserData->Cs_PinMask);

			spi->flag = false;
		}
	}
	/* Write back bytesTransceived to data packet. */
	spi->BytesCnt = bytesTransceived;
}



/*! \brief Start transmission.
 *
 *  This function starts a SPI transmission. A data packet must be prepared
 *  for transmission first.
 *
 *  \param spi                The SPI_Master_t struct instance.
 *  \param dataPacket         The SPI_dataPacket_t struct instance.
 *
 *  \return                   Status code
 *  \retval SPI_OK            The transmission was completed successfully.
 *  \retval SPI_BUSY          The SPI module is busy.
 *  \retval SPI_INTERRUPTED   The transmission was interrupted by another master.
 */
unsigned char SPI_MasterInterruptTransceivePacket(Mcspi_t *spi/*, SPI_DataPacket_t *dataPacket*/)
{
	unsigned char data;
	SPI_t* Spi_BaseAddr = (SPI_t *)(void *)spi->BaseAddr;
	SPI_UserData_t *UserData = (SPI_UserData_t *)spi->UserData;
	bool interrupted = spi->interrupted;

	/* If ongoing transmission. */
	if (spi->flag == true) {
		return (SPI_BUSY);
	}

	/* If interrupted by other master. */
	else if (interrupted) {
		/* If SS released. */
		if (UserData->Cs_Port->OUT & UserData->Cs_PinMask) {
			/* No longer interrupted. */
			interrupted = false;
		}
		else {
			return (SPI_INTERRUPTED);
		}
	}

	/* NOT interrupted by other master.
	* Start transmission. */
	spi->flag = true;
	spi->interrupted = false;

	/* SS to slave(s) low.*/
	SPI_MasterSSLow(UserData->Cs_Port, UserData->Cs_PinMask);

	spi->BytesCnt = 0;

	/* Start sending data. */
	data = spi->Buff[0];
	Spi_BaseAddr->DATA = data;
	while(spi->flag);
	/* Successs */
	return (SPI_OK);
}
//#####################################################
	ISR(SPIC_INT_vect)
	{
		SPI_MasterInterruptHandler(Spi_ServiceAddresses.Spi0RamServiceAddr);
	}
	ISR(SPID_INT_vect)
	{
		SPI_MasterInterruptHandler(Spi_ServiceAddresses.Spi1RamServiceAddr);
	}
#ifdef SPIE
	ISR(SPIE_INT_vect)
	{
		SPI_MasterInterruptHandler(Spi_ServiceAddresses.Spi2RamServiceAddr);
	}
#endif
#ifdef SPIF
	ISR(SPIF_INT_vect)
	{
		SPI_MasterInterruptHandler(Spi_ServiceAddresses.Spi3RamServiceAddr);
	}
#endif
//#####################################################
