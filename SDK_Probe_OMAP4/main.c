/***************************************************************************************/
/*    OMAP4 Multiplatform SDK test application.                                       */
/*    Copyright (C) 2013  Iulian Gheorghiu.                                            */
/*                                                                                     */
/*    This program is free software; you can redistribute it and/or                    */
/*    modify it under the terms of the GNU General Public License                      */
/*    as published by the Free Software Foundation; either version 2                   */
/*    of the License, or (at your option) any later version.                           */
/*                                                                                     */
/*    This program is distributed in the hope that it will be useful,                  */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    */
/*    GNU General Public License for more details.                                     */
/*                                                                                     */
/*    You should have received a copy of the GNU General Public License                */
/*    along with this program.  If not, see <http://www.gnu.org/licenses/>             */
/***************************************************************************************/
#include "board_init.h"

int main(void) {
	
	board_init();
	unsigned int cnt = 0;
	//unsigned int *tmp = (void*)(unsigned int)0x30080;
	//tmp[0] = cnt;

	while(1)
	{
		cnt = 2000000;
		while(--cnt);
		gpio_out(LED1, 1);
		gpio_out(LED2, 0);
		cnt = 2000000;
		while(--cnt);
		gpio_out(LED1, 0);
		gpio_out(LED2, 1);
	}
}
