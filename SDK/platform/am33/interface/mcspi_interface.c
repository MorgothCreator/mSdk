/*
 * mcspi_interface.c
 *
 *  Created on: Dec 13, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include <stdbool.h>
#include <stdlib.h>
#include "../include/mcspi.h"
#include "api/mcspi_def.h"
#include "mcspi_interface.h"
#include "../pinmux/pin_mux_spi.h"
#include "../clk/clk_mcspi.h"
#include "../aintc/aintc_mcspi.h"

extern Mcspi_Int_Service_t Mcspi_Int_Service;
/*#####################################################*/
#define MCSPI_IN_CLK                     (48000000u)
/*#####################################################*/
void _mcspi_assert(Mcspi_t *McspiStruct)
{
	McSPICSAssert(McspiStruct->BaseAddr, McspiStruct->CsSelect);
}
void _mcspi_deassert(Mcspi_t *McspiStruct)
{
	McSPICSDeAssert(McspiStruct->BaseAddr, McspiStruct->CsSelect);
}
/*
** This function will activate/deactivate CS line and also enable Tx and Rx
** interrupts of McSPI peripheral.
*/
void _mcspi_transfer(Mcspi_t *McspiStruct)
{
	McspiStruct->BuffTmp = McspiStruct->Buff;
	/* SPIEN line is forced to low state.*/
    if(!McspiStruct->DisableCsHandle)
    	McSPICSAssert(McspiStruct->BaseAddr, McspiStruct->CsSelect);

    /* Enable the Tx/Rx interrupts of McSPI.*/
    McSPIIntEnable(McspiStruct->BaseAddr, MCSPI_INT_TX_EMPTY(McspiStruct->CsSelect) |
                   MCSPI_INT_RX_FULL(McspiStruct->CsSelect));

    /* Enable the McSPI channel for communication.*/
    McSPIChannelEnable(McspiStruct->BaseAddr, McspiStruct->CsSelect);

    /* Wait until control returns back from McSPI ISR.*/
    while(McspiStruct->flag);

    McspiStruct->flag = 1;

    /* Force SPIEN line to the inactive state.*/
    if(!McspiStruct->DisableCsHandle)
    	McSPICSDeAssert(McspiStruct->BaseAddr, McspiStruct->CsSelect);

    /* Disable the McSPI channel.*/
    McSPIChannelDisable(McspiStruct->BaseAddr, McspiStruct->CsSelect);
}
/*#####################################################*/
unsigned char _mcspi_SendByte(Mcspi_t *McspiStruct, unsigned char byte) {
	return 0;
}
/*#####################################################*/
static void McSPIPinMux(Mcspi_t *McspiStruct)
{
	/*switch(McspiStruct->McspiNr)
	{
		case 0:
			pin_mux_spi0_sclk(McspiStruct->SckPin);
			pin_mux_spi0_d0(McspiStruct->MosiPin);
			pin_mux_spi0_d1(McspiStruct->MisoPin);
			if(McspiStruct->Cs0Pin) pin_mux_spi0_cs0(McspiStruct->Cs0Pin);
			if(McspiStruct->Cs1Pin) pin_mux_spi0_cs1(McspiStruct->Cs1Pin);
			break;
		case 1:
			pin_mux_spi1_sclk(McspiStruct->SckPin);
			pin_mux_spi1_d0(McspiStruct->MosiPin);
			pin_mux_spi1_d1(McspiStruct->MisoPin);
			if(McspiStruct->Cs0Pin) pin_mux_spi1_cs0(McspiStruct->Cs0Pin);
			if(McspiStruct->Cs1Pin) pin_mux_spi1_cs1(McspiStruct->Cs1Pin);
			break;
	}*/
	pin_mux_spi_mosi(McspiStruct->MosiPort, McspiStruct->MosiPin);
	pin_mux_spi_miso(McspiStruct->MisoPort, McspiStruct->MisoPin);
	pin_mux_spi_sck(McspiStruct->SckPort, McspiStruct->SckPin);
	pin_mux_spi_cs0(McspiStruct->CsPort[0], McspiStruct->CsPin[0]);
	pin_mux_spi_cs1(McspiStruct->CsPort[1], McspiStruct->CsPin[1]);
}
/*#####################################################*/
/*
** This function will call the necessary McSPI APIs which will configure the
** McSPI controller.
*/
static void McSPISetUp(Mcspi_t *McspiStruct)
{

	/* Reset the McSPI instance.*/
    McSPIReset(McspiStruct->BaseAddr);

    /* Enable chip select pin.*/
    McSPICSEnable(McspiStruct->BaseAddr);

    /* Enable master mode of operation.*/
    McSPIMasterModeEnable(McspiStruct->BaseAddr);

    /* Perform the necessary configuration for master mode.*/
    McSPIMasterModeConfig(McspiStruct->BaseAddr, MCSPI_SINGLE_CH, MCSPI_TX_RX_MODE, MCSPI_DATA_LINE_COMM_MODE_1, McspiStruct->CsSelect);

    /* Configure the McSPI bus clock depending on clock mode. */
    McSPIClkConfig(McspiStruct->BaseAddr, MCSPI_IN_CLK, McspiStruct->ClkDiv[McspiStruct->CsSelect], McspiStruct->CsSelect, MCSPI_CLK_MODE_0);

    /* Configure the word length.*/
    McSPIWordLengthSet(McspiStruct->BaseAddr, MCSPI_WORD_LENGTH(8), McspiStruct->CsSelect);

    /* Set polarity of SPIEN to low.*/
    McSPICSPolarityConfig(McspiStruct->BaseAddr, MCSPI_CS_POL_LOW, McspiStruct->CsSelect);

    /* Enable the transmitter FIFO of McSPI peripheral.*/
    McSPITxFIFOConfig(McspiStruct->BaseAddr, MCSPI_TX_FIFO_ENABLE, McspiStruct->CsSelect);

    /* Enable the receiver FIFO of McSPI peripheral.*/
    McSPIRxFIFOConfig(McspiStruct->BaseAddr, MCSPI_RX_FIFO_ENABLE, McspiStruct->CsSelect);
}
/*#####################################################*/
bool _mcspi_open(Mcspi_t *McspiStruct)
{
	if(McspiStruct->BuffSize)
	{
	    switch(McspiStruct->McspiNr)
	    {
			case 0:
				McspiStruct->BaseAddr = SOC_SPI_0_REGS;
				Mcspi_Int_Service.Mcspi0 = McspiStruct;
				break;
			case 1:
				McspiStruct->BaseAddr = SOC_SPI_1_REGS;
				Mcspi_Int_Service.Mcspi1 = McspiStruct;
				break;
	    }
		McspiStruct->Buff = (unsigned char*)malloc(McspiStruct->BuffSize);
		if(!McspiStruct->Buff) return false;
		McspiStruct->flag = 1;
		McSPIModuleClkConfig(McspiStruct->McspiNr);
		McSPIPinMux(McspiStruct);
		McSPISetUp(McspiStruct);
		McSPIAintcConfigure(McspiStruct->McspiNr);
		return true;
	}
	return false;
}
/*#####################################################*/
void _mcspi_close(Mcspi_t *McspiStruct)
{
	McSPIAintcUnConfigure(McspiStruct->McspiNr);
	McSPIReset(McspiStruct->BaseAddr);
    McSPIModuleClkUnConfig(McspiStruct->McspiNr);
    switch(McspiStruct->McspiNr)
	{
		case 0:
			Mcspi_Int_Service.Mcspi0 = NULL;
			break;
		case 1:
			Mcspi_Int_Service.Mcspi1 = NULL;
			break;
	}
}
/*#####################################################*/
bool _mcspi_set_baud(Mcspi_t *McspiStruct, unsigned long baud)
{
	/* Configure the McSPI bus clock depending on clock mode. */
	McSPIClkConfig(McspiStruct->BaseAddr, MCSPI_IN_CLK, McspiStruct->ClkDiv[McspiStruct->CsSelect], McspiStruct->CsSelect, MCSPI_CLK_MODE_0);
	return true;
}
