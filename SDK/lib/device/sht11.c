/*
 *  device/sht11.c
 *
 *  Copyright (C) 2014  Iulian Gheorghiu <morgoth.creator@gmail.com>
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

#include "stdbool.h"
#include "sht11.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/timer_api.h"
#include "interface/gpio_interface_def.h"

#ifdef HAVE_GPIO_OUT_OPEN_DRAIN

void sht11_delay(SHT11_t *structure)
{
	unsigned int delay = structure->state_delay;
	while(1) {
		if(!delay--) return;
	}
}

bool sht11(SHT11_t *structure, unsigned char cmd, unsigned char *status_reg)
{
	if(structure->busy) return true;
	timer_interval(&structure->Timeout_Timer, 500);
	structure->busy = true;
	gpio_function_set(structure->Scl, GPIO_OUT_OPEN_DRAIN);
	gpio_function_set(structure->Sda, GPIO_OUT_OPEN_DRAIN);
	/* Send transmision start
	 *        ___       ___
	 * data =    \_____/
	 *          ___   ___
	 * clk  = _/   \_/   \__
	 */
	gpio_out(structure->Scl, 0);
	sht11_delay(structure);
	gpio_out(structure->Scl, 1);
	sht11_delay(structure);
	gpio_out(structure->Sda, 0);
	sht11_delay(structure);
	gpio_out(structure->Scl, 0);
	sht11_delay(structure);
	gpio_out(structure->Scl, 1);
	sht11_delay(structure);
	gpio_out(structure->Sda, 1);
	sht11_delay(structure);
	gpio_out(structure->Scl, 0);
	sht11_delay(structure);
	/*
	 * Send command.
	 */
	unsigned char data_cnt = 0;
	unsigned char _cmd = cmd;
	for(; data_cnt < 8; data_cnt++) {
		if(_cmd & 0x80)
			gpio_out(structure->Sda, 1);
		else
			gpio_out(structure->Sda, 0);
		sht11_delay(structure);
		gpio_out(structure->Scl, 1);
		sht11_delay(structure);
		gpio_out(structure->Scl, 0);
		if(data_cnt == 7) {
			gpio_out(structure->Sda, 1);
			gpio_function_set(structure->Sda, GPIO_IN_PULL_UP);
		}
		sht11_delay(structure);
		_cmd = _cmd << 1;
	}
	gpio_out(structure->Scl, 1);
	sht11_delay(structure);
	signed int ack = gpio_in(structure->Sda);
	gpio_out(structure->Scl, 0);
	sht11_delay(structure);
	if(ack) {
		structure->busy = false;
		return false;
	}
	structure->reg_inst = cmd;
	/*
	 * Write status register
	 */
	if(cmd == SHT11_WRITE_STATUS_REG) {
		gpio_function_set(structure->Sda, GPIO_OUT_OPEN_DRAIN);
		unsigned char st_reg = *status_reg;
		for(data_cnt = 0; data_cnt < 8; data_cnt++) {
			if(st_reg & 0x80)
				gpio_out(structure->Sda, 1);
			else
				gpio_out(structure->Sda, 0);
			sht11_delay(structure);
			gpio_out(structure->Scl, 1);
			sht11_delay(structure);
			gpio_out(structure->Scl, 0);
			if(data_cnt == 7) {
				gpio_out(structure->Sda, 1);
				gpio_function_set(structure->Sda, GPIO_IN_PULL_UP);
			}
			sht11_delay(structure);
			st_reg = st_reg << 1;
		}
		gpio_out(structure->Scl, 1);
		sht11_delay(structure);
		signed int ack = gpio_in(structure->Sda);
		gpio_out(structure->Scl, 0);
		sht11_delay(structure);
		if(ack) {
			structure->busy = false;
			return false;
		}
	} else
		/*
		 * Read status register.
		 */
		if(cmd == SHT11_READ_STATUS_REG) {
		unsigned char st_reg = 0;
		for(data_cnt = 0; data_cnt < 8; data_cnt++) {
			st_reg = st_reg << 1;
			gpio_out(structure->Scl, 1);
			sht11_delay(structure);
			if(gpio_in(structure->Sda))
				st_reg |= 1;
			gpio_out(structure->Scl, 0);
			if(data_cnt == 7) {
				gpio_function_set(structure->Sda, GPIO_OUT_OPEN_DRAIN);
				gpio_out(structure->Sda, 0);
			}
			sht11_delay(structure);
		}
		gpio_out(structure->Scl, 1);
		sht11_delay(structure);
		gpio_out(structure->Scl, 0);
		gpio_function_set(structure->Sda, GPIO_IN_PULL_UP);
		sht11_delay(structure);
		unsigned char chk = 0;
		/*
		 * Read checksum.
		 */
		for(data_cnt = 0; data_cnt < 8; data_cnt++) {
			chk = chk << 1;
			gpio_out(structure->Scl, 1);
			sht11_delay(structure);
			if(gpio_in(structure->Sda))
				chk |= 1;
			gpio_out(structure->Scl, 0);
			sht11_delay(structure);
		}
		gpio_out(structure->Scl, 1);
		sht11_delay(structure);
		gpio_out(structure->Scl, 0);
		sht11_delay(structure);
		//if(ack) return false;
	}
	return true;
}

bool sht11_read(SHT11_t *structure)
{
	if(timer_tick(&structure->Timeout_Timer)) {
		structure->busy = false;
		return false;
	}
	if(!structure->busy) return false;
	if(gpio_in(structure->Sda)) return false;
	unsigned char data_cnt;
	sht11_delay(structure);
	unsigned short data = 0;
	/*
	 * Read MSB data
	 */
	for(data_cnt = 0; data_cnt < 8; data_cnt++) {
		data = data << 1;
		gpio_out(structure->Scl, 1);
		sht11_delay(structure);
		if(gpio_in(structure->Sda))
			data |= 1;
		gpio_out(structure->Scl, 0);
		if(data_cnt == 7) {
			gpio_function_set(structure->Sda, GPIO_OUT_OPEN_DRAIN);
			gpio_out(structure->Sda, 0);
		}
		sht11_delay(structure);
	}
	gpio_out(structure->Scl, 1);
	sht11_delay(structure);
	gpio_out(structure->Scl, 0);
	gpio_function_set(structure->Sda, GPIO_IN_PULL_UP);
	sht11_delay(structure);
	/*
	 * Read LSB data.
	 */
	for(data_cnt = 0; data_cnt < 8; data_cnt++) {
		data = data << 1;
		gpio_out(structure->Scl, 1);
		sht11_delay(structure);
		if(gpio_in(structure->Sda))
			data |= 1;
		gpio_out(structure->Scl, 0);
		if(data_cnt == 7) {
			gpio_function_set(structure->Sda, GPIO_OUT_OPEN_DRAIN);
			gpio_out(structure->Sda, 0);
		}
		sht11_delay(structure);
	}
	gpio_out(structure->Scl, 1);
	sht11_delay(structure);
	gpio_out(structure->Scl, 0);
	gpio_function_set(structure->Sda, GPIO_IN_PULL_UP);
	sht11_delay(structure);
	/*
	 * Read checksum.
	 */
	unsigned char chk = 0;
	for(data_cnt = 0; data_cnt < 8; data_cnt++) {
		chk = chk << 1;
		gpio_out(structure->Scl, 1);
		sht11_delay(structure);
		if(gpio_in(structure->Sda))
			chk |= 1;
		gpio_out(structure->Scl, 0);
		sht11_delay(structure);
	}
	gpio_out(structure->Scl, 1);
	sht11_delay(structure);
	gpio_out(structure->Scl, 0);
	sht11_delay(structure);
	structure->busy = false;
	if(structure->reg_inst == SHT11_START_MEASURE_TEMPERATURE) {
		structure->temperature = structure->vdd_comp + 0.01 * (float)data;
	} else if(structure->reg_inst == SHT11_START_MEASURE_HUMIDITY) {
		structure->humidity = -4 + 0.0405 * (float)data + -0.0000028 * ((float)data * (float)data);
	}
	return true;
}

#endif
