/*
 * pin_mux_mii.h
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
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
void EVMPortRGMIIModeSelect(void);
unsigned int CPSWPinMuxSetup(unsigned int MiiNr);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_cpsw.c"
#endif
/**********************************************/
#endif /* PIN_MUX_MII_H_ */
