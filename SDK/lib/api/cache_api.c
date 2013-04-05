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

#include "sys/cache.h"
/*#####################################################*/
void cache_data_enable()
{
	CacheEnable(CACHE_DCACHE);
}
/*#####################################################*/
void cache_inst_enable()
{
	CacheEnable(CACHE_ICACHE);
}
/*#####################################################*/
void cache_all_enable()
{
	CacheEnable(CACHE_ALL);
}
/*#####################################################*/
void cache_data_disable()
{
	CacheDisable(CACHE_DCACHE);
}
/*#####################################################*/
void cache_inst_disable()
{
	CacheDisable(CACHE_ICACHE);
}
/*#####################################################*/
void cache_all_disable()
{
	CacheDisable(CACHE_ALL);
}
/*#####################################################*/
void cache_inst_invalidate_all()
{
	CacheInstInvalidateAll();
}
/*#####################################################*/
void cache_inst_invalidate_buff(void* startAddr, unsigned int numBytes)
{
	CacheInstInvalidateBuff((unsigned int)startAddr, numBytes);
}
/*#####################################################*/
void cache_data_clean_invalidate_all()
{
	CacheDataCleanInvalidateAll();
}
/*#####################################################*/
void cache_data_clean_invalidate_buff(void* startAddr, unsigned int numBytes)
{
	CacheDataInvalidateBuff((unsigned int)startAddr, numBytes);
}
/*#####################################################*/

