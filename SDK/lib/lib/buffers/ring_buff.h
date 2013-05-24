#ifndef __RING_BUFF_H__
#define __RING_BUFF_H__
//###########################################################################
#include <stdbool.h>
#ifdef _UTIL_ATOMIC_H_
#include <util/atomic.h>
#endif
//###########################################################################
typedef struct
{
	char* Buffer;
	unsigned int buff_size;
	unsigned int push_ptr;
	unsigned int pop_ptr;
	unsigned int bytes_to_buff;
}fifo_settings_t;
//###########################################################################
typedef struct
{
	bool status;
	char character;
}fifo_pop_return_t;
//###########################################################################
bool fifo_push(fifo_settings_t* Settings, char Character);
fifo_pop_return_t fifo_pop(fifo_settings_t* Settings);
fifo_settings_t* fifo_open(unsigned int Size);
void fifo_free(fifo_settings_t* Settings);
//###########################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ring_buff.c"
#endif
//###########################################################################
#endif
