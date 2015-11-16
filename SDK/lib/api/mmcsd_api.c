/*
 *  lib/api/mmcsd_api.c
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

#include "mmcsd_api.h"
#include "interface/hs_mmcsd_interface.h"
#include "api/gpio_def.h"
/*#####################################################*/
void mmcsd_init(unsigned int unit_nr, new_gpio* Cs, new_gpio* StatusLed)
{
	_mmcsd_init(unit_nr, Cs, StatusLed);
}
/*#####################################################*/
void mmcsd_idle(unsigned int unit_nr)
{
	_mmcsd_idle(unit_nr);
}
/*#####################################################*/
void mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{
	_mmcsd_ioctl(unit_nr, command,  buffer);
}
/*#####################################################*/
unsigned int mmcsd_write(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return MMCSDWriteCmdSend(_ctrl, ptr, block, nblks);
}
/*#####################################################*/
unsigned int mmcsd_read(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return MMCSDReadCmdSend(_ctrl, ptr, block, nblks);
}
/*#####################################################*/
