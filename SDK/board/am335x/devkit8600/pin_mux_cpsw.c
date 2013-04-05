/*
 *  lib/api/pin_mux_cpsw.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "board_properties.h"
#include "pin_mux_cpsw.h"

/**
 * \brief   This function sets the RGMII mode for both ports
 *
 * \param   None
 *
 * \return  None.
 */
void EVMPortMIIModeSelect(void)
{
    /* Select MII, Internal Delay mode */
    HWREG(SOC_CONTROL_REGS + CONTROL_GMII_SEL) = 0x0A;
}

unsigned int CPSWPinMuxSetup(unsigned int MiiNr)
{
	switch(MiiNr)
	{
	case 1:

			HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_COL) =   CONTROL_CONF_MII1_COL_CONF_MII1_COL_RXACTIVE | CPSW_RGMII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXEN) =  CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXDV) =  CONTROL_CONF_MII1_RXDV_CONF_MII1_RXDV_RXACTIVE | CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD3) =  CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD2) =  CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD1) =  CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD0) =  CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXCLK) = CONTROL_CONF_MII1_TXCLK_CONF_MII1_TXCLK_RXACTIVE | CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXCLK) = CONTROL_CONF_MII1_RXCLK_CONF_MII1_RXCLK_RXACTIVE | CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD3) =  CONTROL_CONF_MII1_RXD3_CONF_MII1_RXD3_RXACTIVE | CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD2) =  CONTROL_CONF_MII1_RXD2_CONF_MII1_RXD2_RXACTIVE | CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD1) =  CONTROL_CONF_MII1_RXD1_CONF_MII1_RXD1_RXACTIVE | CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD0) =  CONTROL_CONF_MII1_RXD0_CONF_MII1_RXD0_RXACTIVE | CPSW_MII_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_DATA) =  CONTROL_CONF_MDIO_DATA_CONF_MDIO_DATA_RXACTIVE | CONTROL_CONF_MDIO_DATA_CONF_MDIO_DATA_PUTYPESEL | CPSW_MDIO_SEL_MODE;
		    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_CLK) =   CONTROL_CONF_MDIO_CLK_CONF_MDIO_CLK_PUTYPESEL | CPSW_MDIO_SEL_MODE;
		break;
	default:
		return 0;
	}
	return 1;
}

