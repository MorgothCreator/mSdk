/*
 * mcspi_interface.c
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include <stdlib.h>
#include "mcspi_interface.h"
#include "driver/mcspi.h"
#include "api/mcspi_def.h"
#include "pinmux/pin_mux_spi.h"
#include "clk/clk_mcspi.h"
#include "aintc/aintc_mcspi.h"
/*#####################################################*/
#define MCSPI_IN_CLK                     (48000000u)
/*#####################################################*/
/*
** This function will activate/deactivate CS line and also enable Tx and Rx
** interrupts of McSPI peripheral.
*/
void _mcspi_transfer(Mcspi_t *McspiStruct)
{
	McspiStruct->BuffTmp = McspiStruct->Buff;
	/* SPIEN line is forced to low state.*/
    McSPICSAssert(McspiStruct->BaseAddr, McspiStruct->Channel);

    /* Enable the Tx/Rx interrupts of McSPI.*/
    McSPIIntEnable(McspiStruct->BaseAddr, MCSPI_INT_TX_EMPTY(McspiStruct->Channel) |
                   MCSPI_INT_RX_FULL(McspiStruct->Channel));

    /* Enable the McSPI channel for communication.*/
    McSPIChannelEnable(McspiStruct->BaseAddr, McspiStruct->Channel);

    /* Wait until control returns back from McSPI ISR.*/
    while(McspiStruct->flag);

    McspiStruct->flag = 1;

    /* Force SPIEN line to the inactive state.*/
    McSPICSDeAssert(McspiStruct->BaseAddr, McspiStruct->Channel);

    /* Disable the McSPI channel.*/
    McSPIChannelDisable(McspiStruct->BaseAddr, McspiStruct->Channel);
}
/*#####################################################*/
static void McSPIPinMux(Mcspi_t *McspiStruct)
{
	pin_mux_spi_sclk(McspiStruct->ClkPin);
	pin_mux_spi_d0(McspiStruct->D0Pin);
	pin_mux_spi_d1(McspiStruct->D1Pin);
	if(McspiStruct->Cs0Pin) pin_mux_spi_cs0(McspiStruct->Cs0Pin);
	if(McspiStruct->Cs1Pin) pin_mux_spi_cs1(McspiStruct->Cs1Pin);
	if(McspiStruct->Cs2Pin) pin_mux_spi_cs2(McspiStruct->Cs2Pin);
	if(McspiStruct->Cs3Pin) pin_mux_spi_cs3(McspiStruct->Cs3Pin);
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
    McSPIMasterModeConfig(McspiStruct->BaseAddr, MCSPI_SINGLE_CH, MCSPI_TX_RX_MODE, MCSPI_DATA_LINE_COMM_MODE_1, McspiStruct->Channel);

    /* Configure the McSPI bus clock depending on clock mode. */
    McSPIClkConfig(McspiStruct->BaseAddr, MCSPI_IN_CLK, McspiStruct->BaudRate, McspiStruct->Channel, MCSPI_CLK_MODE_0);

    /* Configure the word length.*/
    McSPIWordLengthSet(McspiStruct->BaseAddr, MCSPI_WORD_LENGTH(8), McspiStruct->Channel);

    /* Set polarity of SPIEN to low.*/
    McSPICSPolarityConfig(McspiStruct->BaseAddr, MCSPI_CS_POL_LOW, McspiStruct->Channel);

    /* Enable the transmitter FIFO of McSPI peripheral.*/
    McSPITxFIFOConfig(McspiStruct->BaseAddr, MCSPI_TX_FIFO_ENABLE, McspiStruct->Channel);

    /* Enable the receiver FIFO of McSPI peripheral.*/
    McSPIRxFIFOConfig(McspiStruct->BaseAddr, MCSPI_RX_FIFO_ENABLE, McspiStruct->Channel);
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
			case 2:
				McspiStruct->BaseAddr = SOC_SPI_2_REGS;
				Mcspi_Int_Service.Mcspi2 = McspiStruct;
				break;
			case 3:
				McspiStruct->BaseAddr = SOC_SPI_3_REGS;
				Mcspi_Int_Service.Mcspi3 = McspiStruct;
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

