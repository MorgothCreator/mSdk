/*
 * bioloid_test.c
 *
 *  Created on: May 9, 2013
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */
#include "project.h"

#ifdef BIOLOID_TEST
#include "string.h"
#include "bioloid_test.h"
#include "board_init.h"
#include "api/timer_api.h"
#include "device/dxl.h"

#define REPORT_TYPE		"%u|"

unsigned char dxl_id_table[] = {
		  1,
		  2,
		  3,
		  4,
		  5,
		  6,
		  7,
		  8,
		  9,
		 10,
		 11,
		 12,
		 13,
		 14,
		 15,
		 16,
		 17,
		 18
};

void display_value(unsigned long value) {
#ifndef _TINY_PRINT_
	UARTprintf(DebugCom, "%*u|", 4, value);
#else
	if(value < 10) {
		UARTprintf(DebugCom, "   %u|", value);
	} else if(value < 100) {
		UARTprintf(DebugCom,  "  %u|", value);
	} else if(value < 1000) {
		UARTprintf(DebugCom,   " %u|", value);
	} else {
		UARTprintf(DebugCom,    "%u|", value);
	}
#endif
}

int main(void)
{
	board_init();
	timer(TimerReadSensors);
    timer_interval(&TimerReadSensors, 1000);
	timer(TimerBlinkLed);
    timer_interval(&TimerBlinkLed, 1000);
	/*ms5611_prom_data ms5611_prom_data;
	ms5611_init(&ms5611_prom_data, TWI[0]);
	mpu60x0_init(TWI[0], 0);
	mhc5883_init(TWI[0]);*/
    bool Led1Status = false;
	while(1)
	{
		if(timer_tick(&TimerBlinkLed)) {
			if(Led1Status) {
				gpio_out(LED1, 0);
				timer_interval(&TimerBlinkLed, 100);
				timer_enable(&TimerBlinkLed);
				Led1Status = false;
			} else {
				gpio_out(LED1, 1);
				timer_interval(&TimerBlinkLed, 900);
				timer_enable(&TimerBlinkLed);
				Led1Status = true;
			}
		}
		if(timer_tick(&TimerReadSensors)) {
			DXL_DATA_t buffer[18];
			memset(&buffer, 0, sizeof(buffer));
			unsigned char dxl_err[18];
			memset(&dxl_err, 0, sizeof(dxl_err));
			DXL_COMM_ERR err_ret[18];
			memset(&err_ret, 0, sizeof(err_ret));
			unsigned char dxl_cnt;
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				err_ret[dxl_cnt]  = dxl_read(DXL, (unsigned char *)&buffer[dxl_cnt] , dxl_cnt  + 1 , 0, sizeof(buffer[0]), &dxl_err[dxl_cnt]);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL data:                   ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(dxl_cnt + 1);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL com status            = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(err_ret[dxl_cnt]);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL err                   = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(dxl_err[dxl_cnt]);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL model nr              = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].model_number);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL version               = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].version);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL ID                    = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].id);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL baud rate             = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].baud_rate);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL return delay time     = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].return_delay_time);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL cw angle limit        = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].cw_angle_limit);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL ccw angle limit       = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].ccw_angle_limit);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL limit temperature     = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].limit_temperature);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL down limit voltage    = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].down_limit_voltage);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL up limit voltage      = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].up_limit_voltage);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL mdxl torque limit     = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].mdxl_torque);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL status return level   = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].status_return_level);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL alarm led             = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].alarm_led);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL alarm shutdown        = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].alarm_shutdown);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL down calibration      = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].down_calibration);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL up calibration        = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].up_calibration);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL torque enable         = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].torque_enable);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL led                   = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].led);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL cw compliance margin  = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].cw_compliance_margin);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL ccw compliance margin = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].ccw_compliance_margin);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL cw compliance slope   = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].cw_compliance_slope);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL ccw compliance slope  = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].ccw_compliance_slope);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL goal position         = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].goal_position);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL moving speed          = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].moving_speed);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL torque limit          = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].torque_limit);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL present position      = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].present_position);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL present speed         = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].present_speed);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL present load          = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].present_load);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL present voltage       = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].present_voltage);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL present temperature   = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].present_temperature);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL registered instruction= ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].registered_instruction);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL moving                = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].moving);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL lock                  = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].lock);
			UARTPuts(DebugCom,   "\n\r", -1);

			UARTPuts(DebugCom, "DXL punch                 = ", -1);
			for(dxl_cnt = 0; dxl_cnt < sizeof(dxl_id_table); dxl_cnt++)
				display_value(buffer[dxl_cnt].punch);
			UARTPuts(DebugCom,   "\n\r\n\r", -1);
/*
 * Send to each DXL the position and speed.
 */
			DXL_SYNK_IND_PACKET_t DXL_PACK[18];
#define MOT1		270
			DXL_PACK[0].id = 1;
			DXL_PACK[0].data[0] = (unsigned char)((unsigned short)MOT1);// Goal position
			DXL_PACK[0].data[1] = (unsigned char)((unsigned short)MOT1 >> 8);
			DXL_PACK[0].data[2] = 0;// Speed
			DXL_PACK[0].data[3] = 2;
#define MOT2		736
			DXL_PACK[1].id = 2;
			DXL_PACK[1].data[0] = (unsigned char)((unsigned short)MOT2);// Goal position
			DXL_PACK[1].data[1] = (unsigned char)((unsigned short)MOT2 >> 8);
			DXL_PACK[1].data[2] = 0;// Speed
			DXL_PACK[1].data[3] = 2;
#define MOT3		376
			DXL_PACK[2].id = 3;
			DXL_PACK[2].data[0] = (unsigned char)((unsigned short)MOT3);// Goal position
			DXL_PACK[2].data[1] = (unsigned char)((unsigned short)MOT3 >> 8);
			DXL_PACK[2].data[2] = 0;// Speed
			DXL_PACK[2].data[3] = 2;
#define MOT4		662
			DXL_PACK[3].id = 4;
			DXL_PACK[3].data[0] = (unsigned char)((unsigned short)MOT4);// Goal position
			DXL_PACK[3].data[1] = (unsigned char)((unsigned short)MOT4 >> 8);
			DXL_PACK[3].data[2] = 0;// Speed
			DXL_PACK[3].data[3] = 2;
#define MOT5		300
			DXL_PACK[4].id = 5;
			DXL_PACK[4].data[0] = (unsigned char)((unsigned short)MOT5);// Goal position
			DXL_PACK[4].data[1] = (unsigned char)((unsigned short)MOT5 >> 8);
			DXL_PACK[4].data[2] = 0;// Speed
			DXL_PACK[4].data[3] = 2;
#define MOT6		726
			DXL_PACK[5].id = 6;
			DXL_PACK[5].data[0] = (unsigned char)((unsigned short)MOT6);// Goal position
			DXL_PACK[5].data[1] = (unsigned char)((unsigned short)MOT6 >> 8);
			DXL_PACK[5].data[2] = 0;// Speed
			DXL_PACK[5].data[3] = 2;
#define MOT7		376
			DXL_PACK[6].id = 7;
			DXL_PACK[6].data[0] = (unsigned char)((unsigned short)MOT7);// Goal position
			DXL_PACK[6].data[1] = (unsigned char)((unsigned short)MOT7 >> 8);
			DXL_PACK[6].data[2] = 0;// Speed
			DXL_PACK[6].data[3] = 2;
#define MOT8		660
			DXL_PACK[7].id = 8;
			DXL_PACK[7].data[0] = (unsigned char)((unsigned short)MOT8);// Goal position
			DXL_PACK[7].data[1] = (unsigned char)((unsigned short)MOT8 >> 8);
			DXL_PACK[7].data[2] = 0;// Speed
			DXL_PACK[7].data[3] = 2;
#define MOT9		512
			DXL_PACK[8].id = 9;
			DXL_PACK[8].data[0] = (unsigned char)((unsigned short)MOT9);// Goal position
			DXL_PACK[8].data[1] = (unsigned char)((unsigned short)MOT9 >> 8);
			DXL_PACK[8].data[2] = 0;// Speed
			DXL_PACK[8].data[3] = 2;
#define MOT10		512
			DXL_PACK[9].id = 10;
			DXL_PACK[9].data[0] = (unsigned char)((unsigned short)MOT10);// Goal position
			DXL_PACK[9].data[1] = (unsigned char)((unsigned short)MOT10 >> 8);
			DXL_PACK[9].data[2] = 0;// Speed
			DXL_PACK[9].data[3] = 2;
#define MOT11		512
			DXL_PACK[10].id = 11;
			DXL_PACK[10].data[0] = (unsigned char)((unsigned short)MOT11);// Goal position
			DXL_PACK[10].data[1] = (unsigned char)((unsigned short)MOT11 >> 8);
			DXL_PACK[10].data[2] = 0;// Speed
			DXL_PACK[10].data[3] = 2;
#define MOT12		512
			DXL_PACK[11].id = 12;
			DXL_PACK[11].data[0] = (unsigned char)((unsigned short)MOT12);// Goal position
			DXL_PACK[11].data[1] = (unsigned char)((unsigned short)MOT12 >> 8);
			DXL_PACK[11].data[2] = 0;// Speed
			DXL_PACK[11].data[3] = 2;
#define MOT13		512
			DXL_PACK[12].id = 13;
			DXL_PACK[12].data[0] = (unsigned char)((unsigned short)MOT13);// Goal position
			DXL_PACK[12].data[1] = (unsigned char)((unsigned short)MOT13 >> 8);
			DXL_PACK[12].data[2] = 0;// Speed
			DXL_PACK[12].data[3] = 2;
#define MOT14		512
			DXL_PACK[13].id = 14;
			DXL_PACK[13].data[0] = (unsigned char)((unsigned short)MOT13);// Goal position
			DXL_PACK[13].data[1] = (unsigned char)((unsigned short)MOT14 >> 8);
			DXL_PACK[13].data[2] = 0;// Speed
			DXL_PACK[13].data[3] = 2;
#define MOT15		512
			DXL_PACK[14].id = 15;
			DXL_PACK[14].data[0] = (unsigned char)((unsigned short)MOT15);// Goal position
			DXL_PACK[14].data[1] = (unsigned char)((unsigned short)MOT15 >> 8);
			DXL_PACK[14].data[2] = 0;// Speed
			DXL_PACK[14].data[3] = 2;
#define MOT16		512
			DXL_PACK[15].id = 16;
			DXL_PACK[15].data[0] = (unsigned char)((unsigned short)MOT16);// Goal position
			DXL_PACK[15].data[1] = (unsigned char)((unsigned short)MOT16 >> 8);
			DXL_PACK[15].data[2] = 0;// Speed
			DXL_PACK[15].data[3] = 2;
#define MOT17		512
			DXL_PACK[16].id = 17;
			DXL_PACK[16].data[0] = (unsigned char)((unsigned short)MOT17);// Goal position
			DXL_PACK[16].data[1] = (unsigned char)((unsigned short)MOT17 >> 8);
			DXL_PACK[16].data[2] = 0;// Speed
			DXL_PACK[16].data[3] = 2;
#define MOT18		512
			DXL_PACK[17].id = 18;
			DXL_PACK[17].data[0] = (unsigned char)((unsigned short)MOT18);// Goal position
			DXL_PACK[17].data[1] = (unsigned char)((unsigned short)MOT18 >> 8);
			DXL_PACK[17].data[2] = 0;// Speed
			DXL_PACK[17].data[3] = 2;
			/*
			 *  Send synk write to all 18 DXL's position and speed.
			 */
			dxl_synk_write(DXL, DXL_GOAL_POSITION_L, DXL_PACK, 4, 18);
			/*
			 *  Send position and speed for each DXL without synk.
			 */
			/*unsigned char aditional_info[48];
			unsigned char aditional_info_len;
			dxl_write_data(DXL, 1, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[0].data, 4, aditional_info, &aditional_info_len, &dxl_err[0]);
			dxl_write_data(DXL, 2, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[1].data, 4, aditional_info, &aditional_info_len, &dxl_err[1]);
			dxl_write_data(DXL, 3, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[2].data, 4, aditional_info, &aditional_info_len, &dxl_err[2]);
			dxl_write_data(DXL, 4, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[3].data, 4, aditional_info, &aditional_info_len, &dxl_err[3]);
			dxl_write_data(DXL, 5, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[4].data, 4, aditional_info, &aditional_info_len, &dxl_err[4]);
			dxl_write_data(DXL, 6, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[5].data, 4, aditional_info, &aditional_info_len, &dxl_err[5]);
			dxl_write_data(DXL, 7, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[6].data, 4, aditional_info, &aditional_info_len, &dxl_err[6]);
			dxl_write_data(DXL, 8, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[7].data, 4, aditional_info, &aditional_info_len, &dxl_err[7]);
			dxl_write_data(DXL, 9, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[8].data, 4, aditional_info, &aditional_info_len, &dxl_err[8]);
			dxl_write_data(DXL, 10, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[9].data, 4, aditional_info, &aditional_info_len, &dxl_err[9]);
			dxl_write_data(DXL, 11, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[10].data, 4, aditional_info, &aditional_info_len, &dxl_err[10]);
			dxl_write_data(DXL, 12, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[11].data, 4, aditional_info, &aditional_info_len, &dxl_err[11]);
			dxl_write_data(DXL, 13, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[12].data, 4, aditional_info, &aditional_info_len, &dxl_err[12]);
			dxl_write_data(DXL, 14, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[13].data, 4, aditional_info, &aditional_info_len, &dxl_err[13]);
			dxl_write_data(DXL, 15, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[14].data, 4, aditional_info, &aditional_info_len, &dxl_err[14]);
			dxl_write_data(DXL, 16, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[15].data, 4, aditional_info, &aditional_info_len, &dxl_err[15]);
			dxl_write_data(DXL, 17, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[16].data, 4, aditional_info, &aditional_info_len, &dxl_err[16]);
			dxl_write_data(DXL, 18, DXL_GOAL_POSITION_L, (unsigned char *)&DXL_PACK[17].data, 4, aditional_info, &aditional_info_len, &dxl_err[17]);*/
		}
	}
	return 0;
}
#endif
