/*
 * core_init.h
 *
 * Created: 2/8/2013 12:30:47 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef CORE_INIT_H_
#define CORE_INIT_H_
/*#####################################################*/
#include <avr/io.h>
/*#####################################################*/
#define malloc_set_start(start) __malloc_heap_start = (char *)start
#define malloc_set_end(end) __malloc_heap_end = (char *)end
#define malloc_set_margin(margin) __malloc_margin = (char *)margin
/*#####################################################*/

extern unsigned long CoreFreq;
//--------------------------------------
#define SRAM_SIZE						0x100000UL
#define MEMORY_BLOCK					1
#define SRAM_ADDR						(INTERNAL_SRAM_END + 1)//(SRAM_SIZE * MEMORY_BLOCK)
//*****************************************************************************
//
// The ASSERT macro, which does the actual assertion checking.  Typically, this
// will be for procedure arguments.
//
//*****************************************************************************
#ifdef DEBUG
#define ASSERT(expr) {                                      \
                         if(!(expr))                        \
                         {                                  \
                             __error__(__FILE__, __LINE__); \
                         }                                  \
                     }
#else
#define ASSERT(expr)
#endif
/*#####################################################*/
#define pi 3.14159265358979323846264338327950288419716939937510
/*#####################################################*/
void _core_init(void);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "core_init.c"
#endif
/*#####################################################*/
#endif /* CORE_INIT_H_ */
/*#####################################################*/
