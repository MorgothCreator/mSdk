/*
 * cache.h
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#ifndef _CACHE_H_
#define _CACHE_H_

/*****************************************************************************/
/*
** Macros which can be passed to CacheDisable/Enable APIs
*/
#define CACHE_ICACHE                 (0x01) /* Instruction cache */
#define CACHE_DCACHE                 (0x02) /* Data and Unified cache*/
#define CACHE_ALL                    (0x03) /* Instruction, Data and Unified*/

#if 0
/*
 * CP15 Barrier instructions
 * Please note that we have separate barrier instructions in ARMv7
 * However, we use the CP15 based instructtions because we use
 * -march=armv5 in U-Boot
 */
#define CP15ISB	asm volatile ("mcr     p15, 0, %0, c7, c5, 4" : : "r" (0))
#define CP15DSB	asm volatile ("mcr     p15, 0, %0, c7, c10, 4" : : "r" (0))
#define CP15DMB	asm volatile ("mcr     p15, 0, %0, c7, c10, 5" : : "r" (0))

void v7_outer_cache_enable(void);
void v7_outer_cache_disable(void);
void v7_outer_cache_flush_all(void);
void v7_outer_cache_inval_all(void);
void v7_outer_cache_flush_range(unsigned int start, unsigned int end);
void v7_outer_cache_inval_range(unsigned int start, unsigned int end);

/*****************************************************************************/
typedef volatile unsigned int _CMU_REG_;


typedef struct
{
	_CMU_REG_ CMU_REG_CMUCONFIGREG;//0x00
	_CMU_REG_ CMU_REG_CMUSTATUSREG;//0x04
	_CMU_REG_ CMU_REG_RESERVED_0x08;//0x08
	_CMU_REG_ CMU_REG_CMUINTRACK;//0x0C
	_CMU_REG_ CMU_REG_CMUALLOCATE;//0x10
	_CMU_REG_ CMU_REG_CMUDEALLOCATE;//0x14
	_CMU_REG_ CMU_REG_RESERVED_0x18;//0x18
	_CMU_REG_ CMU_REG_CMUDEBUG;//0x1C
}CMU_REG_t;

struct
{
	_CMU_REG_ CMU_REG_CMUOPERATION;//0x00
	_CMU_REG_ CMU_REG_CMUSTARTPA;//0x04
	_CMU_REG_ CMU_REG_CMULENGTH;//0x08
	_CMU_REG_ CMU_REG_CMURANGESTATE;//0x0C
}CMU_REG_I_t;

#define CMU_REG (*(CMU_REG_t *) 0x48290000)
#define CMU_REG_I(i) (*(CMU_REG_I_t *) (0x48290000 + 0x00001000 + (0x10 * i)))


#define CMU_REG_CMUCONFIGREG_LINESTEP_gp	(0)
#define CMU_REG_CMUCONFIGREG_LINESTEP_gm	(7)

typedef enum
{
	CMU_REG_CMUCONFIGREG_LINESTEP_4BYTES = 0,
	CMU_REG_CMUCONFIGREG_LINESTEP_8BYTES = 1,
	CMU_REG_CMUCONFIGREG_LINESTEP_16BYTES = 2,
	CMU_REG_CMUCONFIGREG_LINESTEP_32BYTES = 3,
	CMU_REG_CMUCONFIGREG_LINESTEP_64BYTES = 4,
	CMU_REG_CMUCONFIGREG_LINESTEP_128BYTES = 5,
	CMU_REG_CMUCONFIGREG_LINESTEP_256BYTES = 6,
	CMU_REG_CMUCONFIGREG_LINESTEP_512BYTES = 7,

	CMU_REG_CMUCONFIGREG_ENNSWR_NONSAFE = 0,
	CMU_REG_CMUCONFIGREG_ENNSWR_SAFE = 1,

	CMU_REG_CMUCONFIGREG_INT_DISABLE = 0,
	CMU_REG_CMUCONFIGREG_INT_ENABLE = 1,

	CMU_REG_CMUCONFIGREG_ENABLEPARALLEL = 0,
	CMU_REG_CMUCONFIGREG_DISABLEPARALLEL = 1,

	CMU_REG_CMUCONFIGREG_ENABLEPIPELINING = 0,
	CMU_REG_CMUCONFIGREG_DISABLEPIPELINING = 1
}CMU_REG_CMUCONFIGREG_LINESTEP_enum;

#define CMU_REG_CMUCONFIGREG_ENNSWR_gp	(3)
#define CMU_REG_CMUCONFIGREG_ENNSWR_gm	(1 << CMU_REG_CMUCONFIGREG_ENNSWR_gp)

#define CMU_REG_CMUCONFIGREG_INTEN_bp	(8)
#define CMU_REG_CMUCONFIGREG_INTEN_bm	(1 << CMU_REG_CMUCONFIGREG_INTEN_bp)

#define CMU_REG_CMUCONFIGREG_DISABLEPARALLEL_bp	(9)
#define CMU_REG_CMUCONFIGREG_DISABLEPARALLEL_bm	(1 << CMU_REG_CMUCONFIGREG_DISABLEPARALLEL_bp)

#define CMU_REG_CMUCONFIGREG_DISABLEPIPELINING_bp	(10)
#define CMU_REG_CMUCONFIGREG_DISABLEPIPELINING_bm	(1 << CMU_REG_CMUCONFIGREG_DISABLEPIPELINING_bp)

#define CMU_REG_CMUCONFIGREG_QENTRIES_gp	(16)
#define CMU_REG_CMUCONFIGREG_QENTRIES_gm	(0x7FF << CMU_REG_CMUCONFIGREG_QENTRIES_gp)

#define CMU_REG_CMUCONFIGREG_THREADS_gp	(16)
#define CMU_REG_CMUCONFIGREG_THREADS_gm	(0x0F << CMU_REG_CMUCONFIGREG_THREADS_gp)






#define CMU_REG_CMUSTATUSREG_BUSY_bp	(0)
#define CMU_REG_CMUSTATUSREG_BUSY_bm	(1 << CMU_REG_CMUSTATUSREG_BUSY_bp)






#define CMU_REG_CMUINTRACK_ERROR_bp	(15)
#define CMU_REG_CMUINTRACK_ERROR_bm	(1 << CMU_REG_CMUINTRACK_ERROR_bp)

#define CMU_REG_CMUINTRACK_VALID_bp	(31)
#define CMU_REG_CMUINTRACK_VALID_bm	(1 << CMU_REG_CMUINTRACK_VALID_bp)





#define CMU_REG_CMUALLOCATE_INDEX_gp	(0)
#define CMU_REG_CMUALLOCATE_INDEX_gm	(0xFF << CMU_REG_CMUALLOCATE_INDEX_gp)

#define CMU_REG_CMUALLOCATE_NONE_bp	(31)
#define CMU_REG_CMUALLOCATE_NONE_bm	(1 << CMU_REG_CMUALLOCATE_NONE_bp)





#define CMU_REG_CMUDEALLOCATE_INDEX_gp	(0)
#define CMU_REG_CMUDEALLOCATE_INDEX_gm	(0xFF << CMU_REG_CMUDEALLOCATE_INDEX_gp)




#define CMU_REG_CMUDEBUG_CPU_MSUSPEND_bp	(0)
#define CMU_REG_CMUDEBUG_CPU_MSUSPEND_bm	(1 << CMU_REG_CMUDEBUG_CPU_MSUSPEND_bp)

#define CMU_REG_CMUDEBUG_SPEC_MSUSPEND_bp	(1)
#define CMU_REG_CMUDEBUG_SPEC_MSUSPEND_bm	(1 << CMU_REG_CMUDEBUG_SPEC_MSUSPEND_bp)

#define CMU_REG_CMUDEBUG_OR_MSUSPEND_bp	(2)
#define CMU_REG_CMUDEBUG_OR_MSUSPEND_bm	(1 << CMU_REG_CMUDEBUG_OR_MSUSPEND_bp)

#define CMU_REG_CMUDEBUG_AND_MSUSPEND_bp	(3)
#define CMU_REG_CMUDEBUG_AND_MSUSPEND_bm	(1 << CMU_REG_CMUDEBUG_AND_MSUSPEND_bp)



/*****************************************************************************/
#endif
void CacheEnable(unsigned int enFlag);
void CacheDisable(unsigned int disFlag);
void CacheInstInvalidateAll(void);
void CacheInstInvalidateBuff(unsigned int startAddr,
                                    unsigned int numBytes);
void CacheDataCleanInvalidateAll(void);
void CacheDataCleanAll(void);
void CacheDataInvalidateAll(void);
void CacheDataCleanBuff(unsigned int startAddr,
                               unsigned int numBytes);

void CacheDataInvalidateBuff(unsigned int startAddr,
                                    unsigned int numBytes);

void CacheDataCleanInvalidateBuff(unsigned int startAddr,
                                         unsigned int numBytes);

#ifdef HEADER_INCLUDE_C_FILES
#include "cache.c"
#endif

#endif /* CACHE_H_ */
