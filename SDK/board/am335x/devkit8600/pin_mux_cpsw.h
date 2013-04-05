/*
 *  lib/api/pin_mux_cpsw.h
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

#ifndef PIN_MUX_MII_H_
#define PIN_MUX_MII_H_
/**********************************************/
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_control_AM335x.h"
#include "include/hw/hw_types.h"
/**********************************************/
#define CPSW_MII_SEL_MODE                   	(0x00u)
#define CPSW_RGMII_SEL_MODE                   	(0x02u)
#define CPSW_MDIO_SEL_MODE                    	(0x00u)
/**********************************************/
void EVMPortMIIModeSelect(void);
unsigned int CPSWPinMuxSetup(unsigned int MiiNr);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_cpsw.c"
#endif
/**********************************************/
#endif /* PIN_MUX_MII_H_ */
