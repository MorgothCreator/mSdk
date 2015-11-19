/*
 * Mpeg_api.c
 *
 *  Created on: Sep 20, 2012
 *      Author: XxXx
 */

#include <stdbool.h>

#ifdef HEADER_INCLUDE_C_FILES
#include "libmpeg2/convert/rgb.c"
#include "libmpeg2/convert/rgb_mmx.c"
#include "libmpeg2/convert/rgb_vis.c"
#include "libmpeg2/convert/uyvy.c"

#include "libmpeg2/alloc.c"
#include "libmpeg2/cpu_accel.c"
#include "libmpeg2/cpu_state.c"
#include "libmpeg2/decode.c"
#include "libmpeg2/header.c"
#include "libmpeg2/idct.c"
#include "libmpeg2/idct_alpha.c"
#include "libmpeg2/idct_altivec.c"
#include "libmpeg2/idct_mmx.c"
#include "libmpeg2/motion_comp_alpha.c"
#include "libmpeg2/motion_comp_altivec.c"
#include "libmpeg2/motion_comp_mmx.c"
#include "libmpeg2/motion_comp_neon.c"
#include "libmpeg2/motion_comp_vis.c"
#include "libmpeg2/motion_comp.c"
#include "libmpeg2/mpeg2_internal.h"
#include "libmpeg2/slice.c"

#include "libvo/video_out.c"
#include "libvo/video_out_dx.c"
#include "libvo/video_out_null.c"
#include "libvo/video_out_pgm.c"
#include "libvo/video_out_sdl.c"
#include "libvo/video_out_x11.c"
#endif


////#include "src/corrupt_mpeg2.c"
//#include "src/dump_state.c"
////#include "src/extract_mpeg2.c"
//#include "src/getopt.c"
//#include "src/gettimeofday.c"
////#include "src/mpeg2dec.c"

#include "interface/rtc_interface.h"
#include "mpeg12_api.h"

//#define Mpeg_Buffering

extern int optind;
extern int opterr;
extern int optopt;

#define _BUFFER_SIZE_ 4096

extern new_uart* DebugCom;
int _Fps;
static volatile unsigned int CntToDetermineTheFps = 0;

unsigned int read_from_buffer(unsigned char *buffer, unsigned int A, unsigned int size, FileInfo_t *file)
{
#ifndef Mpeg_Buffering
	unsigned int Size = _FatData_ReadSection(file, buffer, file->Offset, size);
	file->Offset += Size;
	return Size;
#else
	if(file->Offset > 0x2000000) return 0;
	unsigned char* Ptr = ((unsigned char*)0x80000000 + 0x2000000) + file->Offset;
	unsigned int Cnt = 0;
	for(Cnt = 0; Cnt < size; Cnt++)
	{
		buffer[Cnt] = *Ptr++;
	}
	file->Offset += size;
	return size;
#endif
}

int mpeg_instance_init(mpeg_struct_t *Mpeg_Struct, FileInfo_t *mpgfile)
{
#ifndef AVR32
    timer_interval(&Mpeg_Struct->FrameDisplay, 1000/24);
#endif
	mpgfile->Offset = 0;
	optind = 1;
	opterr = 0;
	optopt = '?';
	Mpeg_Struct->framenum = 0;

	Mpeg_Struct->decoder = mpeg2_init ();
	if (Mpeg_Struct->decoder == NULL) {
	return 0;
	}
	Mpeg_Struct->info = mpeg2_info (Mpeg_Struct->decoder);
	Mpeg_Struct->size = (unsigned int)-1;
	Mpeg_Struct->temporal_reference =(unsigned int)-1;
#ifdef Mpeg_Buffering
    RtcTimeCalDisplay();
	_FatData_ReadSection(mpgfile, (unsigned char*)0x80000000 + 0x2000000, 0, 0x2000000);
    RtcTimeCalDisplay();
#endif
	return 1;
}

unsigned int mpeg_idle(mpeg_struct_t *Mpeg_Struct, new_screen* ScreenBuff, FileInfo_t *mpgfile)
{
	if(!Mpeg_Struct) return 0;
	if(!Mpeg_Struct->FrameReady)
	{
		do
		{
		Mpeg_Struct->state = mpeg2_parse(Mpeg_Struct->decoder);
		switch (Mpeg_Struct->state)
		{
			case STATE_BUFFER:
				//size = fread (buffer, 1, _BUFFER_SIZE_, mpgfile);
				Mpeg_Struct->size = read_from_buffer(Mpeg_Struct->buffer, 1, _BUFFER_SIZE_, mpgfile);
				if(!Mpeg_Struct->size) break;
				mpeg2_buffer (Mpeg_Struct->decoder, Mpeg_Struct->buffer, Mpeg_Struct->buffer + Mpeg_Struct->size);
			break;
			case STATE_SEQUENCE:
				mpeg2_convert (Mpeg_Struct->decoder, Mpeg_Struct->mpeg_convert, NULL);
				break;
			case STATE_SLICE:
			case STATE_END:
			case STATE_INVALID_END:
				if (Mpeg_Struct->info->display_fbuf) Mpeg_Struct->FrameReady = 1;
				break;
			default:
				break;
		}
		}while(!Mpeg_Struct->FrameReady && Mpeg_Struct->size);
	}
#ifndef AVR32
	if(Mpeg_Struct->FrameReady == true && (timer_tick(&Mpeg_Struct->FrameDisplay) == true || Mpeg_Struct->EnableFrameLimit == false))
#else
	if(Mpeg_Struct->FrameReady == true)
#endif
	{
		Mpeg_Struct->FrameReady = false;
		/*save_ppm (ScreenBuff, Mpeg_Struct->info->sequence->width, Mpeg_Struct->info->sequence->height,
							Mpeg_Struct->info->display_fbuf->buf[0], Mpeg_Struct->framenum++);*/


		_Fps++;
		//Mpeg_Struct->->info
		if(Mpeg_Struct->info->current_picture->temporal_reference != Mpeg_Struct->temporal_reference)
		{
			Mpeg_Struct->temporal_reference = Mpeg_Struct->info->current_picture->temporal_reference;
			Mpeg_Struct->CallbackDisplayFrame((void*)Mpeg_Struct->CallbackDisplayFrameVariable, Mpeg_Struct->info->display_fbuf->buf[0], 0, 0, Mpeg_Struct->info->sequence->width, Mpeg_Struct->info->sequence->height);
		}
		if(CntToDetermineTheFps != rtcSecUpdate)
		{
			CntToDetermineTheFps = rtcSecUpdate;
			//UARTPuts(DebugCom, "Screen fill capability = ", -1);
			UARTPutNum(DebugCom, _Fps);
			UARTPuts(DebugCom, "Fps\n\r", -1);
	    	_Fps = 0;
		}
	}

	//if(Mpeg_Struct->size == 0) mpeg2_close (Mpeg_Struct->decoder);
	return Mpeg_Struct->size;
}

void mpeg_free(mpeg_struct_t *Mpeg_Struct)
{
	if(!Mpeg_Struct) return;
	if(Mpeg_Struct)
	{
		mpeg2_close (Mpeg_Struct->decoder);
		Mpeg_Struct = 0;
	}
}

/*void sample2 (new_screen* ScreenBuff, struct FileInfo_Struct *mpgfile)
{
    timer_interval(&FrameDisplay, 1000/24);
	mpgfile->Offset = 0;
	optind = 1;
	opterr = 0;
	optopt = '?';
	#define _BUFFER_SIZE_ 4096
	unsigned char buffer[_BUFFER_SIZE_];
	mpeg2dec_t * decoder;
	const mpeg2_info_t * info;
	mpeg2_state_t state;
	unsigned int size;
	int framenum = 0;

	decoder = mpeg2_init ();
	if (decoder == NULL) {
	return;
	}
	info = mpeg2_info (decoder);
	size = (unsigned int)-1;
	do {
	state = mpeg2_parse (decoder);
	switch (state) {
		case STATE_BUFFER:
			//size = fread (buffer, 1, _BUFFER_SIZE_, mpgfile);
			size = read_from_buffer(buffer, 1, _BUFFER_SIZE_, mpgfile);
			if(!size) break;
			mpeg2_buffer (decoder, buffer, buffer + size);
		break;
		case STATE_SEQUENCE:
			mpeg2_convert (decoder, mpeg2convert_rgb24, NULL);
			break;
		case STATE_SLICE:
		case STATE_END:
		case STATE_INVALID_END:
			if (info->display_fbuf)
			save_ppm (ScreenBuff, info->sequence->width, info->sequence->height,
				  info->display_fbuf->buf[0], framenum++);
			break;
		//case STATE_INVALID:
			//mpeg2_close (decoder);
			//return;
		default:
			break;
		}
	} while (size);
	mpeg2_close (decoder);
}
*/
