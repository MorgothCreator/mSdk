/*
 *  lib/api/edma_api.h
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

#ifndef EDMA_API_H_
#define EDMA_API_H_
/*#####################################################*/
#include "include/interrupt.h"
#include "include/edma.h"
#include "include/hw/hw_control_AM335x.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_cm_wkup.h"
#include "include/hw/hw_cm_per.h"
#include "include/hw/hw_types.h"
/*#####################################################*/
/* EDMA3 Event queue number. */
#define EVT_QUEUE_NUM             (0)
/*#####################################################*/
/* EDMA callback function array */
extern void (*cb_Fxn[EDMA3_NUM_TCC]) (unsigned int tcc, unsigned int status);
//extern Uart_Int_Service_t Uart_Int_Service;
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "edma_api.c"
#endif
/*#####################################################*/
#endif /* EDMA_API_H_ */
