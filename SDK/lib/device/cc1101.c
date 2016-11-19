/*
 * cc1101.c
 *
 *  Created on: Jun 30, 2016
 *      Author: John Smith
 */

#include <api/spi_api.h>
#include <api/spi_def.h>
#include <string.h>
#include <stdbool.h>
#include "cc1101.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/timer_api.h"


/*#####################################################*/
bool CC1101_read(sCC1101Phy* phy, unsigned char address, unsigned char *Buff, unsigned int DataLen)
{
	if(!phy || !phy->SpiStruct)
		return false;
	Mcspi_t* SpiStruct = phy->SpiStruct;
	if(!SpiStruct->Gpio_Cs[phy->spi_instance])
		return false;
	SpiStruct->CsSelect = phy->spi_instance;
	SpiStruct->DisableCsHandle = true;
	spi.assert(SpiStruct);
	timer_interval(&phy->timer, CC1101_MAX_TIMEOUT);
	while(!timer_tick(&phy->timer) && gpio.in(SpiStruct->Gpio_Miso));
	if(gpio.in(SpiStruct->Gpio_Miso))
	{
		spi.deassert(SpiStruct);
		SpiStruct->DisableCsHandle = false;
		return false;
	}

    if (address > 0x2F && address < 0x3E)
    {
    	address |= CC1101_READ_BURST;
    }
    else
    {
    	// Format command (R/W, Burst/Single, Address[5:0]).
    	address = address | CC1101_READ_SINGLE;
    	if (DataLen > 1)
    	{
    		address = address | CC1101_READ_BURST;
    	}
    }
    spi.trx_byte(SpiStruct, address);
	if(!spi.receive(SpiStruct, Buff, DataLen))
	{
		spi.deassert(SpiStruct);
		return false;
	}
	spi.deassert(SpiStruct);
	SpiStruct->DisableCsHandle = false;
	//memcpy(Buff, (void *)SpiStruct->Buff, DataLen);
    return true;
}
/*#####################################################*/
bool CC1101_write(sCC1101Phy* phy, unsigned char address, unsigned char *Buff, unsigned int DataLen)
{
	if(!phy || !phy->SpiStruct)
		return false;
	Mcspi_t* SpiStruct = phy->SpiStruct;
	if(!SpiStruct->Gpio_Cs[phy->spi_instance])
		return false;
	SpiStruct->CsSelect = phy->spi_instance;
	SpiStruct->DisableCsHandle = true;
	spi.assert(SpiStruct);
	timer_interval(&phy->timer, CC1101_MAX_TIMEOUT);
	while(!timer_tick(&phy->timer) && gpio.in(SpiStruct->Gpio_Miso));
	if(gpio.in(SpiStruct->Gpio_Miso))
	{
		gpio.out(SpiStruct->Gpio_Cs[phy->spi_instance], true);
		SpiStruct->DisableCsHandle = false;
		return false;
	}
    // Format command (R/W, Burst/Single, Address[5:0]).
    if (DataLen > 1)
    {
      address = address | CC1101_WRITE_BURST;
    }
    spi.trx_byte(SpiStruct, address);
    //memcpy((void *)SpiStruct->Buff + 1, Buff, DataLen);
    bool state = spi.transmit(SpiStruct, Buff, DataLen);
	spi.deassert(SpiStruct);
	SpiStruct->DisableCsHandle = false;
	return state;
}
/*#####################################################*/
unsigned char CC1101_ReadSyncProblem(sCC1101Phy* phy, unsigned char address)
{
	unsigned char i;
	unsigned char state[4];

	for (i = 0; i < 4; i++)
	{
		CC1101_read(phy, address, (unsigned char*)&state[i], 1);
		// If two consecutive reads yield the same result, then we are guaranteed
		// that the value is valid; no need to continue further...
		if ((i > 0) && (state[i] == state[i-1]))
		{
			break;
		}
	}

	return state[i];
}
/*#####################################################*/
bool CC1101_SetVerifyState(sCC1101Phy* phy, unsigned char command, enum eCC1101MarcState state)
{
	  CC1101_Strobe(phy, command);
	  timer_interval(&phy->timer, CC1101_MAX_TIMEOUT);
	  while (CC1101_GetMarcState(phy) != state)
	  {
	    if (timer_tick(&phy->timer))
	    {
#ifdef CC1101_ERROR_HANDLING
	      CC1101ErrorHandler(eCC1101ErrorTimeout);
#endif
				// TODO: Add a reset-radio routine. Some of the operational states may
				// cause a irreversable state.
	      return false;
	    }
	  }

	  return true;
}
/*#####################################################*/
void CC1101_GdoIit(sCC1101Phy* phy, sCC1101Gdo *gdo[3])
{
	  // Setup the GDOx lines.
	  /**
	   *  If a user does not desire to use the GDOx interface, they may set it to
	   *  NULL. If they set it to NULL, do not initialize anything. Otherwise, setup
	   *  only the GDOx lines that are of a non-NULL value.
	   */
	  if (gdo != NULL)
	  {
		  unsigned char i;      // GDOx initialization iterator

		  // Only allocate iterator if GDOx interface is being used.
		  for (i = 0; i < 3; i++)
		  {
			  if (gdo[i] != NULL)
			  {
				  /**
				   *  Initialize each non-NULL GDOx line. It is assumed that the user will
				   *  set the GDOx interface pointer to NULL if that GDOx line is not
				   *  desired.
				   */
				  phy->gdo[i] = (sCC1101Gdo*)gdo[i];
				  phy->gdo[i]->Init();
			  }
		  }
	  }
}
/*#####################################################*/
unsigned char CC1101_GetReg(sCC1101Phy* phy, unsigned char address)
{
	switch (address)
	{
	 /**
	*  Note: In order to allow for efficient continuous reading of the RSSI
	*  value, the CC1101_RSSI case has been commented out. RSSI is not
	*  protected by the fix for the Errata Notes issue. A fix must be performed
	*  outside of this device driver (such as averaging).
	*/
		//    case CC1101_RSSI:
		case CC1101_FREQEST:
		case CC1101_MARCSTATE:
		case CC1101_RXBYTES:
		case CC1101_TXBYTES:
		case CC1101_WORTIME1:
		case CC1101_WORTIME0:
	    	return CC1101_ReadSyncProblem(phy, address);
	    default:
	    {
	    	unsigned char value;
	    	CC1101_read(phy, address, &value, 1);
	    	return value;
	    }
	}
}
/*#####################################################*/
bool CC1101_SetRag(sCC1101Phy* phy, unsigned char address, unsigned char value)
{
	return CC1101_write(phy, address, &value, 1);
}
/*#####################################################*/
bool CC1101_ReadRegs(sCC1101Phy* phy, unsigned char address, unsigned char *buffer, unsigned char count)
{
	if (count == 1)
	{
		*buffer = CC1101_GetReg(phy, address);
		return true;
	}
	else if (count > 1)
	{
		return CC1101_read(phy, address, buffer, count);
	}
	else
		return false;
}
/*#####################################################*/
bool CC1101_WriteRegs(sCC1101Phy* phy, unsigned char address, unsigned char *buffer, unsigned char count)
{
	return CC1101_read(phy, address, buffer, count);
}
/*#####################################################*/
unsigned char CC1101_ReadFifo(sCC1101Phy* phy, unsigned char *buffer, unsigned char count)
{
	volatile unsigned char rxBytes = CC1101_GetRxFifoCount(phy);

	if (rxBytes < count)
	{
		CC1101_read(phy, CC1101_RXFIFO, buffer, rxBytes);
		return rxBytes;
	}
	else
	{
		CC1101_read(phy, CC1101_RXFIFO, buffer, count);
		return count;
	}
}
/*#####################################################*/
bool CC1101_WriteFifo(sCC1101Phy* phy, unsigned char *buffer, unsigned char count)
{
	return CC1101_write(phy, CC1101_TXFIFO, buffer, count);
}
/*#####################################################*/
bool CC1101_Strobe(sCC1101Phy* phy, unsigned char command)
{
	return CC1101_write(phy, (command & 0xBF), NULL, 0);
}
/*#####################################################*/
bool CC1101_GoOffCrystal(sCC1101Phy* phy)
{
	// CC1101 must be in an IDLE state before powering down. Please see section
	// 19.2 - "Crystal Control" for more inforamtion.
	if (!CC1101_SetVerifyState(phy, CC1101_SIDLE, eCC1101MarcStateIdle))
	{
		return false;
	}
	CC1101_Strobe(phy, CC1101_SXOFF);
	return true;
}
/*#####################################################*/
bool CC1101_Calibrate(sCC1101Phy* phy)
{
	// Calibrate once radio is in IDLE state.
	if (!CC1101_SetVerifyState(phy, CC1101_SIDLE, eCC1101MarcStateIdle))
	{
		return false;
	}
	CC1101_Strobe(phy, CC1101_SCAL);
	return true;
}
/*#####################################################*/
bool CC1101Sleep(sCC1101Phy* phy)
{
	if (!phy->sleep)
	{
		// CC1101 must be in an IDLE state before powering down. Please see section
		// 19.2 - "Crystal Control" for more inforamtion.
		if (!CC1101_SetVerifyState(phy, CC1101_SIDLE, eCC1101MarcStateIdle))
		{
			return false;
		}
		/**
		 *  Once the radio is asleep, any CSn toggling will wake the radio up. We have
		 *  to assume the radio is going to sleep and must therefore set the state
		 *  without the use of CC1101GetMarcState().
		 */
		CC1101_Strobe(phy, CC1101_SPWD);
		phy->sleep = true;
	}
	return true;
}
/*#####################################################*/
void CC1101Wakeup(sCC1101Phy* phy, unsigned char agctest, unsigned char test[3], unsigned char *paTable, unsigned char paTableSize)
{
  /**
   *  Assumes that the SPI Read/Write implementation automatically handles
   *  asserting CSn for this radio (active low) and has waited for the SPI MISO
   *  pin to go low (CHIP_RDYn). If this is not done, this procedure WILL NOT
   *  WORK CORRECTLY.
   */
  if (phy->sleep)
  {
    // If the radio is coming out of a sleep state, perform a wake up routine to
    // reestablish the initial radio state.
    CC1101_Strobe(phy, CC1101_SIDLE);
    phy->sleep = false;

    /**
     *  The last valid calibration results are maintained so calibration is
     *  still valid after waking up from a low power state. The TX/RX FIFOs have
     *  been flushed. If IOCFGx.GDOx_CFG setting is less that 0x20 and
     *  IOCFGx.GDOx_INV is 0(1), GDO0 and GDO2 pins are hardwired to 0(1), and
     *  GDO1 is hardwired to 1(0), until CSn goes low. The following registers
     *  are not retained and must be rewritten: AGCTEST, TEST2, TEST1, TEST0,
     *  PATABLE(contents of PATABLE are lost except the first byte).
     */
    CC1101_write(phy, CC1101_REG_AGCTEST, &agctest, 1);
    CC1101_write(phy, CC1101_REG_TEST2, test, 3);
    CC1101_write(phy, CC1101_PATABLE, paTable, paTableSize);
  }
}
