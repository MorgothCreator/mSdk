/*
 * Mpeg_api.h
 *
 *  Created on: Sep 20, 2012
 *      Author: XxXx
 */

#ifndef MPEG_API_H_
#define MPEG_API_H_

#include <stdbool.h>
#include "config.h"
#include "include/mpeg2.h"
#include "include/mpeg2convert.h"
#include "api/timer_api.h"
#include "../controls_definition.h"
#include "lib/fs/fat.h"
#include "api/uart_api.h"
//#include "../../../api/Rtc_api.h"


//#define MpegState_Play	1
//#define MpegState_Stop	2

typedef struct
{
	bool FrameReady;
	unsigned char EnableFrameLimit;
	unsigned int size;
	unsigned int framenum;
	unsigned int CallbackDisplayFrameVariable;
	unsigned int temporal_reference;
	void(*CallbackDisplayFrame)(void*, unsigned char*, unsigned int, unsigned int,unsigned int, unsigned int);
	mpeg2_convert_t *mpeg_convert;
	mpeg2dec_t * decoder;
	const mpeg2_info_t * info;
	mpeg2_state_t state;
	timer(FrameDisplay);
	unsigned char buffer[4096];
}mpeg_struct_t;

int mpeg_instance_init(mpeg_struct_t *Mpeg_Struct, FileInfo_t *mpgfile);
unsigned int mpeg_idle(mpeg_struct_t *Mpeg_Struct, new_screen* ScreenBuff, FileInfo_t *mpgfile);
void mpeg_free(mpeg_struct_t *Mpeg_Struct);

//#include "mpeg12_api.c"

#endif /* MPEG_API_H_ */
