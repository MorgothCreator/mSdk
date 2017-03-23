/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */ 

#include <stdio.h>
#include "inc/hw_types.h"
#include "mcspi_interface.h"
#include "api/spi_def.h"
#include "driver/ssi.h"
#include "sys/core_init.h"
#include "driver/ioc.h"
//#include "api/gpio_api.h"

extern unsigned long CoreFreq;

bool _mcspi_open(new_mcspi *McspiStruct)
{
	switch(McspiStruct->McspiNr)
	{
	case 0:
		SSIDisable(SSI0_BASE);
		IOCPortConfigureSet(McspiStruct->SckPin, IOC_PORT_MCU_SSI0_CLK, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << McspiStruct->SckPin);
		IOCPortConfigureSet(McspiStruct->MisoPin, IOC_PORT_MCU_SSI0_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->MisoPin);
		IOCPortConfigureSet(McspiStruct->MosiPin, IOC_PORT_MCU_SSI0_TX, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << McspiStruct->MosiPin);
		IOCPortConfigureSet(McspiStruct->CsPin[0], IOC_PORT_MCU_SSI0_FSS, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << McspiStruct->CsPin[0]);
		SSIConfigSetExpClk(SSI0_BASE, CoreFreq, SSI_FRF_MOTO_MODE_3, McspiStruct->Mode, McspiStruct->ClkDiv[McspiStruct->McspiNr], McspiStruct->WordSize);
		McspiStruct->BaseAddr = SSI0_BASE;
		break;
	case 1:
		SSIDisable(SSI1_BASE);
		IOCPortConfigureSet(McspiStruct->SckPin, IOC_PORT_MCU_SSI1_CLK, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << McspiStruct->SckPin);
		IOCPortConfigureSet(McspiStruct->MisoPin, IOC_PORT_MCU_SSI1_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->MisoPin);
		IOCPortConfigureSet(McspiStruct->MosiPin, IOC_PORT_MCU_SSI1_TX, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << McspiStruct->MosiPin);
		IOCPortConfigureSet(McspiStruct->CsPin[0], IOC_PORT_MCU_SSI1_FSS, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << McspiStruct->CsPin[0]);
		SSIConfigSetExpClk(SSI1_BASE, CoreFreq, SSI_FRF_MOTO_MODE_3, McspiStruct->Mode, McspiStruct->ClkDiv[McspiStruct->McspiNr], McspiStruct->WordSize);
		McspiStruct->BaseAddr = SSI1_BASE;
		break;
	default:
		return false;
	}
	SSIEnable(McspiStruct->BaseAddr);
	return true;//spi_enable(McspiStruct);
}
/*#####################################################*/
void _mcspi_close(new_mcspi *McspiStruct)
{
	SSIDisable(McspiStruct->BaseAddr);
	switch(McspiStruct->McspiNr)
	{
	case 0:
		IOCPortConfigureSet(McspiStruct->SckPin, IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->SckPin);
		IOCPortConfigureSet(McspiStruct->MisoPin, IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->MisoPin);
		IOCPortConfigureSet(McspiStruct->MosiPin, IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->MosiPin);
		IOCPortConfigureSet(McspiStruct->CsPin[0], IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->CsPin[0]);
		SSIConfigSetExpClk(SSI0_BASE, CoreFreq, SSI_FRF_MOTO_MODE_0, McspiStruct->Mode, McspiStruct->ClkDiv[McspiStruct->McspiNr], McspiStruct->WordSize);
		McspiStruct->BaseAddr = 0;
		break;
	case 1:
		IOCPortConfigureSet(McspiStruct->SckPin, IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->SckPin);
		IOCPortConfigureSet(McspiStruct->MisoPin, IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->MisoPin);
		IOCPortConfigureSet(McspiStruct->MosiPin, IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->MosiPin);
		IOCPortConfigureSet(McspiStruct->CsPin[0], IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << McspiStruct->CsPin[0]);
		SSIConfigSetExpClk(SSI1_BASE, CoreFreq, SSI_FRF_MOTO_MODE_0, McspiStruct->Mode, McspiStruct->ClkDiv[McspiStruct->McspiNr], McspiStruct->WordSize);
		McspiStruct->BaseAddr = 0;
		break;
	}
}
/*#####################################################*/
bool _mcspi_transfer(new_mcspi *McspiStruct)
{
	if(!McspiStruct->DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << McspiStruct->CsPin[0];

	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < McspiStruct->numOfBytes; transfer_cnt++) {
		McspiStruct->Buff[transfer_cnt] = _mcspi_SendByte(McspiStruct, McspiStruct->Buff[transfer_cnt]);
	}
	if(!McspiStruct->DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << McspiStruct->CsPin[0];
	return true;
}
/*#####################################################*/
unsigned char _mcspi_SendByte(Mcspi_t *McspiStruct, unsigned char byte)
{
	SSIDataPut(McspiStruct->BaseAddr, byte);
	uint32_t pui32Data;
	SSIDataGet(McspiStruct->BaseAddr, &pui32Data);
	return (unsigned char)pui32Data;
}
/*#####################################################*/
bool _mcspi_set_baud(Mcspi_t *McspiStruct, unsigned long baud)
{
	SSIDisable(McspiStruct->BaseAddr);
	SSIConfigSetExpClk(McspiStruct->BaseAddr, CoreFreq, SSI_FRF_MOTO_MODE_0, McspiStruct->Mode, baud, McspiStruct->WordSize);
	SSIEnable(McspiStruct->BaseAddr);
	McspiStruct->ClkDiv[McspiStruct->McspiNr] = baud;
	return true;
}
/*#####################################################*/
void _mcspi_assert(Mcspi_t *McspiStruct)
{
	HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << McspiStruct->CsPin[McspiStruct->McspiNr];
}
/*#####################################################*/
void _mcspi_deassert(Mcspi_t *McspiStruct)
{
	HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << McspiStruct->CsPin[McspiStruct->McspiNr];
}
/*#####################################################*/

