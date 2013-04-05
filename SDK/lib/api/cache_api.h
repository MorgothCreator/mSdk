/*
 *  lib/api/cache_api.h
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

#ifndef CACHE_API_H_
#define CACHE_API_H_

#include "sys/cache.h"
#include "sys/cp15.h"

void cache_data_enable();
void cache_inst_enable();
void cache_all_enable();
void cache_data_disable();
void cache_inst_disable();
void cache_all_disable();
void cache_inst_invalidate_all();
void cache_inst_invalidate_buff(void* startAddr, unsigned int numBytes);
void cache_data_clean_invalidate_all();
void cache_data_clean_invalidate_buff(void* startAddr, unsigned int numBytes);

#ifdef HEADER_INCLUDE_C_FILES
#include "cache_api.c"
#endif


#endif /* CACHE_API_H_ */
