/*
 * cache.c
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#include "cache.h"
#include "smc.h"
#include "utils.h"
//#if 0
//#if 0
void v7_outer_cache_enable(void)
{
	//set_pl310_ctrl_reg(1);
}

void v7_outer_cache_disable(void)
{
	//set_pl310_ctrl_reg(0);
}

//#include <linux/types.h>
//#include <common.h>
#include "armv7.h"
//#include <asm/utils.h>

#define ARMV7_DCACHE_INVAL_ALL		1
#define ARMV7_DCACHE_CLEAN_INVAL_ALL	2
#define ARMV7_DCACHE_INVAL_RANGE	3
#define ARMV7_DCACHE_CLEAN_INVAL_RANGE	4

#ifndef CONFIG_SYS_DCACHE_OFF
/*
 * Write the level and type you want to Cache Size Selection Register(CSSELR)
 * to get size details from Current Cache Size ID Register(CCSIDR)
 */
static void set_csselr(unsigned int level, unsigned int type)
{	unsigned int csselr = level << 1 | type;

	/* Write to Cache Size Selection Register(CSSELR) */
	asm volatile ("mcr p15, 2, %0, c0, c0, 0" : : "r" (csselr));
}

static unsigned int get_ccsidr(void)
{
	unsigned int ccsidr;

	/* Read current CP15 Cache Size ID Register */
	asm volatile ("mrc p15, 1, %0, c0, c0, 0" : "=r" (ccsidr));
	return ccsidr;
}

static unsigned int get_clidr(void)
{
	unsigned int clidr;

	/* Read current CP15 Cache Level ID Register */
	asm volatile ("mrc p15,1,%0,c0,c0,1" : "=r" (clidr));
	return clidr;
}

static void v7_inval_dcache_level_setway(unsigned int level, unsigned int num_sets,
					 unsigned int num_ways, unsigned int way_shift,
					 unsigned int log2_line_len)
{
	int way, set, setway;

	/*
	 * For optimal assembly code:
	 *	a. count down
	 *	b. have bigger loop inside
	 */
	for (way = num_ways - 1; way >= 0 ; way--) {
		for (set = num_sets - 1; set >= 0; set--) {
			setway = (level << 1) | (set << log2_line_len) |
				 (way << way_shift);
			/* Invalidate data/unified cache line by set/way */
			asm volatile ("	mcr p15, 0, %0, c7, c6, 2"
					: : "r" (setway));
		}
	}
	/* DSB to make sure the operation is complete */
	CP15DSB;
}

static void v7_clean_inval_dcache_level_setway(unsigned int level, unsigned int num_sets,
					       unsigned int num_ways, unsigned int way_shift,
					       unsigned int log2_line_len)
{
	int way, set, setway;

	/*
	 * For optimal assembly code:
	 *	a. count down
	 *	b. have bigger loop inside
	 */
	for (way = num_ways - 1; way >= 0 ; way--) {
		for (set = num_sets - 1; set >= 0; set--) {
			setway = (level << 1) | (set << log2_line_len) |
				 (way << way_shift);
			/*
			 * Clean & Invalidate data/unified
			 * cache line by set/way
			 */
			asm volatile ("	mcr p15, 0, %0, c7, c14, 2"
					: : "r" (setway));
		}
	}
	/* DSB to make sure the operation is complete */
	CP15DSB;
}

static void v7_maint_dcache_level_setway(unsigned int level, unsigned int operation)
{
	unsigned int ccsidr;
	unsigned int num_sets, num_ways, log2_line_len, log2_num_ways;
	unsigned int way_shift;

	set_csselr(level, ARMV7_CSSELR_IND_DATA_UNIFIED);

	ccsidr = get_ccsidr();

	log2_line_len = ((ccsidr & CCSIDR_LINE_SIZE_MASK) >>
				CCSIDR_LINE_SIZE_OFFSET) + 2;
	/* Converting from words to bytes */
	log2_line_len += 2;

	num_ways  = ((ccsidr & CCSIDR_ASSOCIATIVITY_MASK) >>
			CCSIDR_ASSOCIATIVITY_OFFSET) + 1;
	num_sets  = ((ccsidr & CCSIDR_NUM_SETS_MASK) >>
			CCSIDR_NUM_SETS_OFFSET) + 1;
	/*
	 * According to ARMv7 ARM number of sets and number of ways need
	 * not be a power of 2
	 */
	log2_num_ways = log_2_n_round_up(num_ways);

	way_shift = (32 - log2_num_ways);
	if (operation == ARMV7_DCACHE_INVAL_ALL) {
		v7_inval_dcache_level_setway(level, num_sets, num_ways,
				      way_shift, log2_line_len);
	} else if (operation == ARMV7_DCACHE_CLEAN_INVAL_ALL) {
		v7_clean_inval_dcache_level_setway(level, num_sets, num_ways,
						   way_shift, log2_line_len);
	}
}

static void v7_maint_dcache_all(unsigned int operation)
{
	unsigned int level, cache_type, level_start_bit = 0;

	unsigned int clidr = get_clidr();

	for (level = 0; level < 7; level++) {
		cache_type = (clidr >> level_start_bit) & 0x7;
		if ((cache_type == ARMV7_CLIDR_CTYPE_DATA_ONLY) ||
		    (cache_type == ARMV7_CLIDR_CTYPE_INSTRUCTION_DATA) ||
		    (cache_type == ARMV7_CLIDR_CTYPE_UNIFIED))
			v7_maint_dcache_level_setway(level, operation);
		level_start_bit += 3;
	}
}

static void v7_dcache_clean_inval_range(unsigned int start,
					unsigned int stop, unsigned int line_len)
{
	unsigned int mva;

	/* Align start to cache line boundary */
	start &= ~(line_len - 1);
	for (mva = start; mva < stop; mva = mva + line_len) {
		/* DCCIMVAC - Clean & Invalidate data cache by MVA to PoC */
		asm volatile ("mcr p15, 0, %0, c7, c14, 1" : : "r" (mva));
	}
}

static void v7_dcache_inval_range(unsigned int start, unsigned int stop, unsigned int line_len)
{
	unsigned int mva;

	/*
	 * If start address is not aligned to cache-line do not
	 * invalidate the first cache-line
	 */
	if (start & (line_len - 1)) {
		//printf("ERROR: %s - start address is not aligned - 0x%08x\n", __func__, start);
		/* move to next cache line */
		start = (start + line_len - 1) & ~(line_len - 1);
	}

	/*
	 * If stop address is not aligned to cache-line do not
	 * invalidate the last cache-line
	 */
	if (stop & (line_len - 1)) {
		//printf("ERROR: %s - stop address is not aligned - 0x%08x\n", __func__, stop);
		/* align to the beginning of this cache line */
		stop &= ~(line_len - 1);
	}

	for (mva = start; mva < stop; mva = mva + line_len) {
		/* DCIMVAC - Invalidate data cache by MVA to PoC */
		asm volatile ("mcr p15, 0, %0, c7, c6, 1" : : "r" (mva));
	}
}

static void v7_dcache_maint_range(unsigned int start, unsigned int stop, unsigned int range_op)
{
	unsigned int line_len, ccsidr;

	ccsidr = get_ccsidr();
	line_len = ((ccsidr & CCSIDR_LINE_SIZE_MASK) >>
			CCSIDR_LINE_SIZE_OFFSET) + 2;
	/* Converting from words to bytes */
	line_len += 2;
	/* converting from log2(linelen) to linelen */
	line_len = 1 << line_len;

	switch (range_op) {
	case ARMV7_DCACHE_CLEAN_INVAL_RANGE:
		v7_dcache_clean_inval_range(start, stop, line_len);
		break;
	case ARMV7_DCACHE_INVAL_RANGE:
		v7_dcache_inval_range(start, stop, line_len);
		break;
	}

	/* DSB to make sure the operation is complete */
	CP15DSB;
}

/* Invalidate TLB */
static void v7_inval_tlb(void)
{
	/* Invalidate entire unified TLB */
	asm volatile ("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
	/* Invalidate entire data TLB */
	asm volatile ("mcr p15, 0, %0, c8, c6, 0" : : "r" (0));
	/* Invalidate entire instruction TLB */
	asm volatile ("mcr p15, 0, %0, c8, c5, 0" : : "r" (0));
	/* Full system DSB - make sure that the invalidation is complete */
	CP15DSB;
	/* Full system ISB - make sure the instruction stream sees it */
	CP15ISB;
}

void invalidate_dcache_all(void)
{
	v7_maint_dcache_all(ARMV7_DCACHE_INVAL_ALL);

	v7_outer_cache_inval_all();
}

/*
 * Performs a clean & invalidation of the entire data cache
 * at all levels
 */
void flush_dcache_all(void)
{
	v7_maint_dcache_all(ARMV7_DCACHE_CLEAN_INVAL_ALL);

	v7_outer_cache_flush_all();
}

/*
 * Invalidates range in all levels of D-cache/unified cache used:
 * Affects the range [start, stop - 1]
 */
void invalidate_dcache_range(unsigned int start, unsigned int stop)
{

	v7_dcache_maint_range(start, stop, ARMV7_DCACHE_INVAL_RANGE);

	v7_outer_cache_inval_range(start, stop);
}

/*
 * Flush range(clean & invalidate) from all levels of D-cache/unified
 * cache used:
 * Affects the range [start, stop - 1]
 */
void flush_dcache_range(unsigned int start, unsigned int stop)
{
	v7_dcache_maint_range(start, stop, ARMV7_DCACHE_CLEAN_INVAL_RANGE);

	v7_outer_cache_flush_range(start, stop);
}

void arm_init_before_mmu(void)
{
	v7_outer_cache_enable();
	invalidate_dcache_all();
	v7_inval_tlb();
}

void mmu_page_table_flush(unsigned int start, unsigned int stop)
{
	flush_dcache_range(start, stop);
	v7_inval_tlb();
}

/*
 * Flush range from all levels of d-cache/unified-cache used:
 * Affects the range [start, start + size - 1]
 */
void  flush_cache(unsigned int start, unsigned int size)
{
	flush_dcache_range(start, start + size);
}


#else /* #ifndef CONFIG_SYS_DCACHE_OFF */
void invalidate_dcache_all(void)
{
}

void flush_dcache_all(void)
{
}

void invalidate_dcache_range(unsigned int start, unsigned int stop)
{
}

void flush_dcache_range(unsigned int start, unsigned int stop)
{
}

void arm_init_before_mmu(void)
{
}

void  flush_cache(unsigned int start, unsigned int size)
{
}

void mmu_page_table_flush(unsigned int start, unsigned int stop)
{
}

void arm_init_domains(void)
{
}
#endif /* #ifndef CONFIG_SYS_DCACHE_OFF */

#ifndef CONFIG_SYS_ICACHE_OFF
/* Invalidate entire I-cache and branch predictor array */
void invalidate_icache_all(void)
{
	/*
	 * Invalidate all instruction caches to PoU.
	 * Also flushes branch target cache.
	 */
	asm volatile ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));

	/* Invalidate entire branch predictor array */
	asm volatile ("mcr p15, 0, %0, c7, c5, 6" : : "r" (0));

	/* Full system DSB - make sure that the invalidation is complete */
	CP15DSB;

	/* ISB - make sure the instruction stream sees it */
	CP15ISB;
}
#else
void invalidate_icache_all(void)
{
}
#endif

/*  Stub implementations for outer cache operations */
/*__weak*/ //void v7_outer_cache_enable(void) {};
/*__weak*/ //void v7_outer_cache_disable(void) {};
/*__weak*/ //void v7_outer_cache_flush_all(void) {};
/*__weak*/ //void v7_outer_cache_inval_all(void) {};
/*__weak*/ //void v7_outer_cache_flush_range(unsigned int start, unsigned int end) {};
/*__weak*/ //void v7_outer_cache_inval_range(unsigned int start, unsigned int end) {};

//#else
/**
 * \brief   Disables Cache. The levels/type of Cache to be disabled
 *          is passed as parameter.
 *
 * \param   disFlag   Caches to be disabled.
 *            'disFlag' can take one of the below values. \n
 *                CACHE_ICACHE - To disable Instruction Cache \n
 *                CACHE_DCACHE - To disable Data/Unified Cache \n
 *                CACHE_ALL - To disable all levels of Cache
 *
 * \return  None.
 *
 * \Note    Disabling Data Cache disables Unified cache also, if present.
 **/
void CacheDisable(unsigned int disFlag)
{

}

/**
 * \brief   Enables Cache. The levels/type of Cache to be enabled
 *          is passed as parameter.
 *
 * \param   enFlag   Caches to be enabled.
 *            'enFlag' can take one of the below values. \n
 *                CACHE_ICACHE - To enable Instruction Cache \n
 *                CACHE_DCACHE - To enable Data/Unified Cache \n
 *                CACHE_ALL - To enable all levels of Cache
 *
 * \return  None.
 *
 * \Note    Enabling Data Cache enables Unified cache also, if present.
 **/

void CacheEnable(unsigned int enFlag)
{

}

/**
 * \brief   This API invalidates the entire I-Cache
 *
 * \param   None
 *
 * \return  None.
 *
 **/
void CacheInstInvalidateAll(void)
{

}

/**
 * \brief   This API invalidates a section of I-Cache.
 *
 * \param   startAddr    Starting address to be invalidated
 * \param   numBytes     The number of bytes to be invalidated
 *
 * \return  None.
 *
 **/
void CacheInstInvalidateBuff(unsigned int startAddr, unsigned int numBytes)
{

}

/**
 * \brief   This API Cleans and Invalidates the entire Data Cache.
 *
 * \param   None
 *
 * \return  None.
 *
 **/
void CacheDataCleanInvalidateAll(void)
{

}

/**
 * \brief   This API Cleans the entire Data Cache.
 *
 * \param   None
 *
 * \return  None.
 *
 **/
void CacheDataCleanAll(void)
{

}

/**
 * \brief   This API Invalidates the entire Data Cache.
 *
 * \param   None
 *
 * \return  None.
 *
 **/
void CacheDataInvalidateAll(void)
{

}

/**
 * \brief   This API clean a section of D-Cache, upto PoC. This API
 *          can be used to make a buffer in D-Cache to be coherent
 *          with the memory. For example, If DMA engine has to access
 *          a memory area for transmitting, to make sure that the
 *          D-Cache values for the corresponding buffer is written to
 *          memory, this API can be used.
 *
 * \param   startAddr    Starting address of the buffer to be cleaned
 * \param   numBytes     The number of bytes to be cleaned.
 *
 * \return  None.
 *
 **/
void CacheDataCleanBuff(unsigned int startAddr, unsigned int numBytes)
{

}

/**
 * \brief   This API invalidates a section of D-Cache till PoC. With this
 *          API, we can make sure that the next read of the buffer happens
 *          from memory. This is required if any DMA engine has updated
 *          the memory area with any data, other than from the D-Cache.
 *
 * \param   startAddr    Starting address of the buffer to be invalidated
 * \param   numBytes     The number of bytes to be invalidated
 *
 * \return  None.
 *
 **/
void CacheDataInvalidateBuff(unsigned int startAddr, unsigned int numBytes)
{

}

/**
 * \brief   This API cleans and invalidates a section of D-Cache to PoC.
 *
 * \param   startAddr    Starting address of the buffer to be cleaned
 *                       and invalidated
 * \param   numBytes     The number of bytes to be cleaned and invalidated
 *
 * \return  None.
 *
 **/
void CacheDataCleanInvalidateBuff(unsigned int startAddr, unsigned int numBytes)
{

}

//#endif

