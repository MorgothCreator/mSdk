/*
 * core_init.h
 *
 *  Created on: Aug 3, 2012
 *      Author: XxXx
 */

#ifndef CORE_INIT_H_
#define CORE_INIT_H_
#include "../clk/clk_core.h"
#include "api/gpio_api.h"
#include "../include/interrupt.h"
#include "../interface/mmu_interface.h"
#include "../include/gpio_v2.h"
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

#define pi 3.14159265358979323846264338327950288419716939937510

inline void VfpEnable(void);
void _core_init(void);

#ifdef HEADER_INCLUDE_C_FILES
#include "core_init.c"
#endif

#endif /* CORE_INIT_H_ */
