/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA External Bus Interface driver source file.
 *
 *      This file contains the function implementations for the XMEGA EBI driver.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the XMEGA EBI module.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 *      Several functions use the following construct:
 *          "some_register = ... | (some_parameter ? SOME_BIT_bm : 0) | ..."
 *      Although the use of the ternary operator ( if ? then : else ) is discouraged,
 *      in some occasions the operator makes it possible to write pretty clean and
 *      neat code. In this driver, the construct is used to set or not set a
 *      configuration bit based on a boolean input parameter, such as
 *      the "some_parameter" in the example above.
 *
 * \par Application note:
 *      AVR1312: Using the XMEGA External Bus Interface
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 479 $
 * $Date: 2007-03-05 16:30:19 +0100 (ma, 05 mar 2007) $  \n
 *
 * Copyright (c) 2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "ebi.h"


/*! \brief Enable EBI module and configure memory types without enabling any Chip Select modules.
 *
 *  This function enables the EBI module. Note that individual
 *  Chip Select submodules must be enabled using EBI_EnableSRAM(...),
 *  EBI_EnableLPC(...) or EBI_EnableSDRAM(...).
 *
 *  This function also applies various parameters regarding SRAM, LPC and SDRAM
 *  operation, however these does not come into play before a Chip Select submodule
 *  is enabled.
 *
 *  \param  sdramDataWidth  SDRAM module to use 4-bit or 8-bit data bus.
 *  \param  lpcMode         ALE mode for SRAM LPC operation.
 *  \param  sramMode        ALE mode for ordinary SRAM operation.
 *  \param  interfaceMode   EBI module to use 2-port, 3-port or 4-port interface. Note that SDRAM needs 3-port or 4-port.
 */
void EBI_Enable( EBI_SDDATAW_t sdramDataWidth,
                 EBI_LPCMODE_t lpcMode,
                 EBI_SRMODE_t sramMode,
                 EBI_IFMODE_t interfaceMode )
{
	/* These fields fill up the whole register, so we don't have to protect any bits with masks. */
	EBI.CTRL = (uint8_t) sdramDataWidth | lpcMode | sramMode | interfaceMode;
}



/*! \brief Enable a Chip Select module for SRAM operation.
 *
 *  This function enables one Chip Select submodule for SRAM operation.
 *
 *  \param  chipSelect       Pointer to Chip Select register submodule.
 *  \param  addrSpace        Address space covered by this Chip Select submodule.
 *  \param  baseAddr         Base address. This will be truncated to a 4K boundary.
 *  \param  waitStateCycles  Wait state cycle count for SRAM operation.
 */
void EBI_EnableSRAM( volatile EBI_CS_t * chipSelect,
                     EBI_CS_ASPACE_t addrSpace,
                     uint32_t baseAddr,
                     uint8_t waitStateCycles )
{
	/* Configure SRAM settings. */
	chipSelect->CTRLB = (chipSelect->CTRLB & ~EBI_CS_SRWS_gm) |
	                    (waitStateCycles & EBI_CS_SRWS_gm); /* Truncate too large values. */

	/* Truncate the base address to an address space boundary. */
	chipSelect->BASEADDR = (((uint32_t) baseAddr)>>8) & (0xFFFF<<(addrSpace>>2));

	/* Last part of configuration and then enable Chip Select module in SRAM mode. */
	chipSelect->CTRLA = (chipSelect->CTRLA & ~(EBI_CS_ASPACE_gm | EBI_CS_MODE_gm)) |
	                    addrSpace | EBI_CS_MODE_SRAM_gc;
}



/*! \brief Enable a Chip Select module for LPC operation.
 *
 *  This function enables one Chip Select submodule for SRAM LPC operation.
 *
 *  \param  chipSelect       Pointer to Chip Select register submodule.
 *  \param  addrSpace        Address space covered by this Chip Select submodule.
 *  \param  baseAddr         Base address. This will be truncated to a 4K boundary.
 *  \param  waitStateCycles  Wait state cycle count for SRAM LPC operation.
 */
void EBI_EnableLPC( volatile EBI_CS_t * chipSelect,
                    EBI_CS_ASPACE_t addrSpace,
                    uint32_t baseAddr,
                    uint8_t waitStateCycles )
{
	/* Configure SRAM LPC settings. */
	chipSelect->CTRLB = (chipSelect->CTRLB & ~EBI_CS_SRWS_gm) |
	                    (waitStateCycles & EBI_CS_SRWS_gm); /* Truncate too large values. */

	/* Truncate the base address to an address space boundary. */
	chipSelect->BASEADDR = (((uint32_t) baseAddr)>>8) & (0xFFFF<<(addrSpace>>2));

	/* Last part of configuration and then enable Chip Select module in SRAM LPC mode. */
	chipSelect->CTRLA = (chipSelect->CTRLA & ~(EBI_CS_ASPACE_gm | EBI_CS_MODE_gm)) |
	                    addrSpace | EBI_CS_MODE_LPC_gc;
}



/*! \brief Enable a Chip Select module for SDRAM operation.
 *
 *  This function enables Chip Select submodule 3 for SDRAM operation.
 *  SDARM operation is only supported by Chip Select submodule 3.
 *
 *  \param  addrSpace                     Address space covered by this Chip Select submodule.
 *  \param  baseAddr                      Base address. This will be truncated to a 4K boundary.
 *  \param  casLatency                    False = 2 cycles, true = 3 cycles.
 *  \param  rowBits                       False = 11 row bits, True = 12 row bits.
 *  \param  columnBits                    Column bits configuration. 8-11 column bits.
 *  \param  modeRegisterDelay             Delay between Mode Register and Activate command. 0-3 cycles.
 *  \param  rowCycleDelay                 Delay between Refresh and Activate command. 0-7 cycles.
 *  \param  rowToPrechargeDelay           Delay between Pre-charge and another command. 0-7 cycles.
 *  \param  writeRecoveryDelay            Write Recovery time. 0-3 cycles.
 *  \param  exitSelfRefreshToActiveDelay  Delay between CKE high and Activate command. 0-7 cycles.
 *  \param  rowToColumnDelay              Delay between Activate and Read/Write command. 0-7 cycles.
 *  \param  refreshPeriod                 SDRAM refresh period. 0-1023 cycles.
 *  \param  initializationDelay           SDRAM initialization delay. 0-1023 cycles.
 */
void EBI_EnableSDRAM( EBI_CS_ASPACE_t addrSpace,
                      uint32_t baseAddr,
                      bool casLatency,
                      bool rowBits,
                      EBI_SDCOL_t columnBits,
                      EBI_MRDLY_t modeRegisterDelay,
                      EBI_ROWCYCDLY_t rowCycleDelay,
                      EBI_RPDLY_t rowToPrechargeDelay,
                      EBI_WRDLY_t writeRecoveryDelay,
                      EBI_ESRDLY_t exitSelfRefreshToActiveDelay,
                      EBI_ROWCOLDLY_t rowToColumnDelay,
                      uint16_t refreshPeriod,
                      uint16_t initializationDelay )
{
	/* Set two single-bit fields and one larger field, while protecting reserved bits. */
	EBI.SDRAMCTRLA = (EBI.SDRAMCTRLA & ~(EBI_SDCAS_bm | EBI_SDROW_bm | EBI_SDCOL_gm)) |
	                 (casLatency ? EBI_SDCAS_bm : 0) | (rowBits ? EBI_SDROW_bm : 0) | columnBits;

	/* Set delay counts. Fields fill up the entire registers. */
	EBI.SDRAMCTRLB = (uint8_t) modeRegisterDelay | rowCycleDelay | rowToPrechargeDelay;
	EBI.SDRAMCTRLC = (uint8_t) writeRecoveryDelay | exitSelfRefreshToActiveDelay | rowToColumnDelay;

	/* Set refresh period and initialization delay, truncate too large values. */
	EBI.REFRESH = refreshPeriod & 0x03FF;
	EBI.INITDLY = initializationDelay & 0x3FFF;

	/* Last part of configuration and then enable Chip Select module in SDRAM mode. */
	EBI.CS3.CTRLB = (EBI.CS3.CTRLB & ~(EBI_CS_SDSREN_bm | EBI_CS_SDMODE_gm)) |
	                EBI_CS_SDMODE_NORMAL_gc;

	/* Truncate the base address to an address space boundary. */
	EBI.CS3.BASEADDR = (((uint32_t) baseAddr)>>8) & (0xFFFF<<(addrSpace>>2));

	EBI.CS3.CTRLA = (EBI.CS3.CTRLA & ~(EBI_CS_ASPACE_gm | EBI_CS_MODE_gm)) |
	                addrSpace | EBI_CS_MODE_SDRAM_gc;

	/* Wait for SDRAM to initialize. */
	while ((EBI.CS3.CTRLB & EBI_CS_SDINITDONE_bm) == 0){

	}
}
