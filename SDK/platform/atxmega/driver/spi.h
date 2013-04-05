#ifndef __IO_Spi_H__
#define __IO_Spi_H__
//#####################################################
#include <avr/io.h>
#include <stdbool.h>
#include "api/gpio_def.h"
//#####################################################
#define _Spi_Int_Disabled					(0)
#define _Spi_Int_Low_Level					(1)
#define _Spi_Int_Med_Level					(2)
#define _Spi_Int_High_Level					(3)
//-----------------------------------------------------
#define _SpiClkDiv4							(0)
#define _SpiClkDiv16						(1)
#define _SpiClkDiv64						(2)
#define _SpiClkDiv128						(3)
//-----------------------------------------------------
#define _SpiModeRisingSample				(0)
#define _SpiModeRisingSetup					(1)
#define _SpiModeFallingSample				(2)
#define _SpiModeFallingSetup				(3)
//-----------------------------------------------------
//Bit position for status of the transmit buffer with interupt
#define _SpiBusy_bp							(0)
#define _SpiSoftBusy_bp						(1)
#define _SpiIfCondTimeOut_bp				(2)
#define _SpiIsFirstByteFlag_bp				(3)
#define _SpiError_bp						(7)
//-----------------------------------------------------
//Comands for interupt transmit buffer(used only by internal routines)
#define _Spi_Command_SendBuffer				(1)
#define _Spi_Command_SendBufferIfCond		(2)
#define _Spi_Command_SendBufferIfCondBusy	(3)
#define _Spi_Command_SendBufferError		(255)
//#####################################################
#define _SpiDefaultDivider					(_SpiClkDiv4)
//#####################################################
#define SPI_OK              0     /*!< \brief The transmission completed successfully. */
#define SPI_INTERRUPTED     1     /*!< \brief The transmission was interrupted by another master. */
#define SPI_BUSY            2     /*!< \brief The SPI module is busy with another transmission. */
//#####################################################
typedef struct
{
	PORT_t *Cs_Port;
	unsigned char Cs_PinMask;
}SPI_UserData_t;
//#####################################################
typedef struct 
{
	Mcspi_t* Spi0RamServiceAddr;
	Mcspi_t* Spi1RamServiceAddr;
#ifdef SPIE
	Mcspi_t* Spi2RamServiceAddr;
#endif
#ifdef SPIF
	Mcspi_t* Spi3RamServiceAddr;
#endif
} SpiServiceAddresses;
//#####################################################
bool spi_enable(Mcspi_t *SpiStruct);
void spi_disable(Mcspi_t *SpiStruct);
unsigned char SPI_MasterInterruptTransceivePacket(Mcspi_t *spi);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "spi.c"
#endif
//#####################################################
//extern SpiServiceAddresses Spi_ServiceAddresses;
//#####################################################
#endif