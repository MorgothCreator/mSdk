/*
 * cache.h
 *
 * Created: 2/8/2013 10:46:42 PM
 *  Author: XxXx
 */ 


#ifndef CACHE_H_
#define CACHE_H_

/*****************************************************************************/
/*
** Macros which can be passed to CacheDisable/Enable APIs
*/
#define CACHE_ICACHE                 (0x01) /* Instruction cache */
#define CACHE_DCACHE                 (0x02) /* Data and Unified cache*/
#define CACHE_ALL                    (0x03) /* Instruction, Data and Unified
                                               Cache at all levels*/

/*****************************************************************************/
/*
** API prototypes
*/
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