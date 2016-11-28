#ifndef __FiFo__
#define __FiFo__
//###########################################################################
#include <stdbool.h>
#include <stdlib.h>
#ifdef _UTIL_ATOMIC_H_
#include <util/atomic.h>
#endif
#include "ring_buff.h"
//###########################################################################
bool fifo_push(fifo_settings_t* Settings, char Character)
{
	volatile bool Return = false;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		unsigned int BuffSize = Settings->buff_size;
		unsigned int BytesToBuff = Settings->bytes_to_buff;
		if(BytesToBuff < BuffSize)
		{
			unsigned int PushPtr = Settings->push_ptr;
			Settings->Buffer[Settings->push_ptr] = Character;
			PushPtr++;
			if(PushPtr == BuffSize)
				PushPtr = 0;
			BytesToBuff++;
			Settings->push_ptr = PushPtr;
			Settings->bytes_to_buff = BytesToBuff;
			Return = true;
		}
#ifdef _UTIL_ATOMIC_H_
	}	
#endif
	return Return;
}
//###########################################################################
fifo_pop_return_t fifo_pop(fifo_settings_t* Settings)
{
	fifo_pop_return_t Result;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		unsigned int BytesToBuff = Settings->bytes_to_buff;
		if(BytesToBuff == 0)
			Result.status = false;
		else
		{
			unsigned int PopPtr = Settings->pop_ptr;
			Result.character = Settings->Buffer[PopPtr];
			PopPtr++;
			if(PopPtr == Settings->buff_size) PopPtr = 0;
			BytesToBuff--;
			Settings->bytes_to_buff = BytesToBuff;
			Settings->pop_ptr = PopPtr;
			Result.status = true;
		}
#ifdef _UTIL_ATOMIC_H_
	}	
#endif
	return Result;
}
//###########################################################################
fifo_settings_t* fifo_open(unsigned int Size)
{
	fifo_settings_t* Settings = (fifo_settings_t*)calloc(1, sizeof(fifo_settings_t));
	if(Settings == 0)
		return NULL;
	char* Buff;
	if((Buff = (char*)calloc(1,Size)) == 0) 
	{
		free(Settings);
		return NULL;
	}
	Settings->bytes_to_buff = 0;
	Settings->pop_ptr = 0;
	Settings->push_ptr = 0;
	Settings->Buffer = Buff;
	Settings->buff_size = Size;
	return Settings;
}
//###########################################################################
void fifo_free(fifo_settings_t* Settings)
{
	free((void*)Settings->Buffer);
	free(Settings);
}
//###########################################################################
#endif
