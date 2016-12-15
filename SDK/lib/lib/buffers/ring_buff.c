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
bool fifo_push(fifo_settings_t* param, char Character)
{
	volatile bool Return = false;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		unsigned int BuffSize = param->buff_size;
		unsigned int BytesToBuff = param->bytes_to_buff;
		if(BytesToBuff < BuffSize)
		{
			unsigned int PushPtr = param->push_ptr;
			param->Buffer[param->push_ptr] = Character;
			PushPtr++;
			if(PushPtr == BuffSize)
				PushPtr = 0;
			BytesToBuff++;
			param->push_ptr = PushPtr;
			param->bytes_to_buff = BytesToBuff;
			Return = true;
		}
#ifdef _UTIL_ATOMIC_H_
	}	
#endif
	return Return;
}
//###########################################################################
unsigned int fifo_push_buff(fifo_settings_t* param, unsigned char *buff, unsigned int len)
{
	unsigned int cnt = 0;
	for(; cnt < len; cnt++)
	{
		if(!fifo_push(param, buff[cnt]))
		{
			return cnt;
		}
	}
	return cnt;
}
//###########################################################################
fifo_pop_return_t fifo_pop(fifo_settings_t* param)
{
	fifo_pop_return_t Result;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		unsigned int BytesToBuff = param->bytes_to_buff;
		if(BytesToBuff == 0)
			Result.status = false;
		else
		{
			unsigned int PopPtr = param->pop_ptr;
			Result.character = param->Buffer[PopPtr];
			PopPtr++;
			if(PopPtr == param->buff_size)
				PopPtr = 0;
			BytesToBuff--;
			param->bytes_to_buff = BytesToBuff;
			param->pop_ptr = PopPtr;
			Result.status = true;
		}
#ifdef _UTIL_ATOMIC_H_
	}	
#endif
	return Result;
}
//###########################################################################
unsigned int fifo_pop_buff(fifo_settings_t* param, unsigned char *buff, unsigned int len)
{
	unsigned int cnt = 0;
	for(; cnt < len; cnt++)
	{
		fifo_pop_return_t tmp = fifo_pop(param);
		if(!tmp.status)
			return cnt;
		buff[cnt] = tmp.character;
	}
	return cnt;
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
int fifo_reinit(fifo_settings_t *param, unsigned int Size)
{
	if(param == 0)
		return -2;
	fifo_deinit(param);
	char* Buff;
	if((Buff = (char*)calloc(1 ,Size)) == 0)
	{
		return -1;
	}
	param->bytes_to_buff = 0;
	param->pop_ptr = 0;
	param->push_ptr = 0;
	param->Buffer = Buff;
	param->buff_size = Size;
	return 0;
}
//###########################################################################
void fifo_free(fifo_settings_t* param)
{
	free((void*)param->Buffer);
	free(param);
}
//###########################################################################
void fifo_deinit(fifo_settings_t* param)
{
	if(param->Buffer)
		free((void*)param->Buffer);
}
//###########################################################################
#endif
