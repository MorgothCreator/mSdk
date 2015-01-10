/*
 * bioloid_test.c
 *
 *  Created on: jan 5, 2013
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */
#include "project.h"

#ifdef SENSORS_TEST
#include "string.h"
#include "sensors_test.h"
#include "board_init.h"
#include "api/timer_api.h"
#include "device/ms5611.h"
#include "device/mpu60x0.h"
#include "device/mhc5883.h"
#include "device/sht11.h"
#include "device/dxl.h"


int main(void)
{
	board_init();
	timer(TimerReadSensors);
    timer_interval(&TimerReadSensors, 1000);
	timer(TimerBlinkLed);
    timer_interval(&TimerBlinkLed, 1000);
	mpu60x0_init(MPU60x0, 0);
	mhc5883_init(MHC5883);
    bool Led1Status = false;
	unsigned char sht11_status_reg = 0;
	bool sht11_read_mode = false;
	while(1)
	{
		if(sht11_read_mode) {
			/*
			 * Send start temperature measure
			 * The sht11_write function will send the command if internal flag 'busy' is false and return true.
			 * If flag busy is true will return true.
			 */
			if(sht11_write(SHT11, SHT11_START_MEASURE_TEMPERATURE, &sht11_status_reg)) {
				/*
				 * Pool SGT11 and read data if is ready
				 * If read is complete will return true, else will return false.
				 * When read is complete the busy flag will be set to false, this will allow
				 * sht11_write to send another command if is called.
				 */
				if(sht11_read(SHT11))
					/*
					 * Read complete.
					 * Now you can send another command.
					 */
					sht11_read_mode = false;
			}
		} else {
			/*
			 * Send start humidity measure
			 * The sht11_write function will send the command if internal flag 'busy' is false and return true.
			 * If flag busy is true will return true.
			 */
			if(sht11_write(SHT11, SHT11_START_MEASURE_HUMIDITY, &sht11_status_reg)) {
				/*
				 * Pool SGT11 and read data if is ready
				 * If read is complete will return true, else will return false.
				 * When read is complete the busy flag will be set to false, this will allow
				 * sht11_write to send another command if is called.
				 */
				if(sht11_read(SHT11))
					/*
					 * Read complete.
					 * Now you can send another command.
					 */
					sht11_read_mode = true;
			}
		}
		/* Send start ranging */
		if(srf02_start(SRF02, SRF02_START_RANGING))
			/*
			 * Pool SRF02 and read data if is ready
			 * If data is not ready, this function will not freeze,
			 * If data ready will return true, if not wil return false.
			 */
			srf02_read(SRF02);

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
			ms5611_display_preasure_result(MS5611, MS5611_CONVERT_OSR_1024);
			mpu60x0_temperature_display_result(MPU60x0, 0);
			mpu60x0_giroscope_display_result(MPU60x0, 0);
			mpu60x0_accelerometer_display_result(MPU60x0, 0);
			mhc5883_display_result(MHC5883);
			sht11_display_data(SHT11);
			srf02_display_data(SRF02);
			UARTprintf(DebugCom, "ADC1:\n\rCH0 = %d, CH1 = %d, TempSensor = %2.2f\n\r\n\r", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], (float)(((float)1775 - (float)ADC[0]->ConvResult[2]) / 5.337) + (float)25);

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
		}
	}
}








#endif
