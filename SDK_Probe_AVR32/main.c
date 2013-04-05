/*
 * main.c
 *
 *  Created on: Jan 8, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "board_init.h"
#include "api/gpio_api.h"
#include "api/timer_api.h"

#include "lib/gfx/checkbox.h"

tCheckBox *check = NULL;

#define UseMpeg12

#ifdef UseMpeg12
#include "lib/gfx/mpeg12/mpeg12_api.h"
mpeg_struct_t *MpegStruct;
#endif

#define MpegState_Play	1
#define MpegState_Stop	2

#define MpegOnFullScreen
int Fps = 0;
int MpegPlayerState = MpegState_Stop;
static volatile unsigned int CntDisplayRTC = 0;
static volatile unsigned int PlayerCommands = MpegState_Play;

timer(TimerLeds);
signed int LedsCnt = 0;
/*#####################################################*/
//timer(TimerScanTouch);
/*#####################################################*/


int main(void)
{
	board_init();

	gpio_out(LED1, 0);
	gpio_out(LED2, 0);
	gpio_out(LED3, 0);
/*******************************************************/
	//timer_interval(&TimerScanTouch, 40);

    timer_interval(&TimerLeds, 333);




	check = new_checkbox(ScreenBuff);
	tControlCommandData control_comand;
	memset(&control_comand, 0x00, sizeof(tControlCommandData));

	while(1)
	{
		if(timer_tick(&TimerLeds))
		{
			switch(LedsCnt++)
			{
			case 0:
				gpio_out(LED2, 1);
				gpio_out(LED3, 1);
				gpio_out(LED1, 0);
				break;
			case 1:
				gpio_out(LED1, 1);
				gpio_out(LED3, 1);
				gpio_out(LED2, 0);
				break;
			case 2:
				gpio_out(LED1, 1);
				gpio_out(LED2, 1);
				gpio_out(LED3, 0);
				break;
			}
			if(LedsCnt > 2) LedsCnt = 0;
		}
		//if(timer_tick(&TimerScanTouch))
		//{
			checkbox(check, &control_comand);
			mmcsd_idle(NULL);
#ifdef UseMpeg12
			if(FILE1)
			{
				if(MpegPlayerState == MpegState_Stop && PlayerCommands == MpegState_Play)
				{
					if(!memcmp(FILE1->FileInfo_PointedFileInDirectoryExtension, "MPG", 3))
					{
						//delay_ms(1000);
						if(_FatData_OpenFile(FILE1))
						{
							//sample2(ScreenBuff, FILE1);
							MpegPlayerState = MpegState_Play;
							MpegStruct = calloc(1, sizeof(mpeg_struct_t));
							MpegStruct->EnableFrameLimit = false;
#ifdef MpegOnFullScreen
							MpegStruct->CallbackDisplayFrameVariable = (unsigned int)ScreenBuff;
							MpegStruct->CallbackDisplayFrame = screen_put_rgb_array_24;
							MpegStruct->mpeg_convert = mpeg2convert_bgr24;
#else
							MpegStruct->CallbackDisplayFrameVariable = (unsigned int)PICTUREBOX1;
							MpegStruct->CallbackDisplayFrame = picturebox_put_rgb_array_32;
#endif
							//PROGRESSVIDEO->Maximum = FILE1->FileInfo_CurrentOppenedFile_FileSize;
							mpeg_instance_init(MpegStruct, FILE1);
						}
						else PlayerCommands = MpegState_Stop;
					}
					else PlayerCommands = MpegState_Stop;
				}
			}
#endif
		//}
#ifdef UseMpeg12
		if(MpegPlayerState == MpegState_Play)
    	{
    		if((mpeg_idle(MpegStruct, ScreenBuff, FILE1) == 0 && FILE1 != 0) || PlayerCommands == MpegState_Stop)
    		{
    			mpeg_free(MpegStruct);
    			_FatData_CloseFile(FILE1);
    			if(MpegStruct) free(MpegStruct);
    			MpegPlayerState = MpegState_Stop;
				if(FILE1 && PlayerCommands == MpegState_Play)
				{
					if(_FatData_Dn(FILE1) == true);
					else
					{
						_FatData_GoToRoot(FILE1);
					}
				}
    		}
    	}
#endif
	}
	return 0;
}
