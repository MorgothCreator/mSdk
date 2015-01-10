/*
 *  lib/lib/gfx/paint.h
 *
 *  Copyright (C) 2015  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  Created on: Jan 9, 2015
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
/*#####################################################*/
#ifndef PAINT_H_
#define PAINT_H_
/*#####################################################*/
#include "stdbool.h"
/*#####################################################*/
typedef struct Gfx_paint_res_s{

}Gfx_paint_res_t;
/*#####################################################*/
typedef struct Gfx_res_s {
	struct {
		Gfx_paint_res_t en_pull;
		Gfx_paint_res_t en_move;
		Gfx_paint_res_t en_push;
		Gfx_paint_res_t dis_pull;
		Gfx_paint_res_t dis_move;
		Gfx_paint_res_t dis_push;
	}button;
	struct {
		Gfx_paint_res_t en_pull_chk;
		Gfx_paint_res_t en_move_chk;
		Gfx_paint_res_t en_push_chk;
		Gfx_paint_res_t dis_pull_chk;
		Gfx_paint_res_t dis_move_chk;
		Gfx_paint_res_t dis_push_chk;
		Gfx_paint_res_t en_pull_uchk;
		Gfx_paint_res_t en_move_uchk;
		Gfx_paint_res_t en_push_uchk;
		Gfx_paint_res_t dis_pull_uchk;
		Gfx_paint_res_t dis_move_uchk;
		Gfx_paint_res_t dis_push_uchk;

		Gfx_paint_res_t en_pull;
		Gfx_paint_res_t en_move;
		Gfx_paint_res_t en_push;
		Gfx_paint_res_t dis_pull;
		Gfx_paint_res_t dis_move;
		Gfx_paint_res_t dis_push;
	}checkbox;
	struct {
		Gfx_paint_res_t en_item_pull;
		Gfx_paint_res_t en_item_move;
		Gfx_paint_res_t en_item_push;
		Gfx_paint_res_t dis_item_pull;
		Gfx_paint_res_t dis_item_move;
		Gfx_paint_res_t dis_item_push;

		Gfx_paint_res_t en_btn_pull;
		Gfx_paint_res_t en_btn_move;
		Gfx_paint_res_t en_btn_push;
		Gfx_paint_res_t dis_btn_pull;
		Gfx_paint_res_t dis_btn_move;
		Gfx_paint_res_t dis_btn_push;
	}listbox;
	struct {
		Gfx_paint_res_t en_back;
		Gfx_paint_res_t dis_back;
		Gfx_paint_res_t en_progress;
		Gfx_paint_res_t dis_progress;
	}progress;
	struct {
		Gfx_paint_res_t en_btn_ud_pull;
		Gfx_paint_res_t en_btn_ud_move;
		Gfx_paint_res_t en_btn_ud_push;
		Gfx_paint_res_t dis_btn_ud_pull;
		Gfx_paint_res_t dis_btn_ud_move;
		Gfx_paint_res_t dis_btn_ud_push;

		Gfx_paint_res_t en_btn_m_pull;
		Gfx_paint_res_t en_btn_m_move;
		Gfx_paint_res_t en_btn_m_push;
		Gfx_paint_res_t dis_btn_m_pull;
		Gfx_paint_res_t dis_btn_m_move;
		Gfx_paint_res_t dis_btn_m_push;

		Gfx_paint_res_t en_back;
		Gfx_paint_res_t dis_back;
	}scroll;

}Gfx_res_t;
/*#####################################################*/
#endif /* PAINT_H_ */
/*#####################################################*/
