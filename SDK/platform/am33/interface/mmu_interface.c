/*
 * mmu_interface.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */

#include "board_properties.h"
#include "sys/mmu.h"

void MMUConfigAndEnable(void)
{
    /*
    ** Define DDR memory region of AM335x. DDR can be configured as Normal
    ** memory with R/W access in user/privileged modes. The cache attributes
    ** specified here are,
    ** Inner - Write through, No Write Allocate
    ** Outer - Write Back, Write Allocate
    */
    /*REGION regionDdr = {
                        MMU_PGTYPE_SECTION, START_ADDR_DDR, NUM_SECTIONS_DDR,
                        MMU_MEMTYPE_NORMAL_SHAREABLE(MMU_CACHE_WT_NOWA,
                                                     MMU_CACHE_WB_WA),
                        MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
                        (unsigned int*)pageTable
                       };*/
    REGION regionDdr = {
                            MMU_PGTYPE_SECTION, START_ADDR_DDR, NUM_SECTIONS_DDR,
                            MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA,
                                                             MMU_CACHE_WB_WA),
                            MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
                            (unsigned int*)pageTable
                           };
    /*
    ** Define OCMC RAM region of AM335x. Same Attributes of DDR region given.
    */
    REGION regionOcmc = {
                         MMU_PGTYPE_SECTION, START_ADDR_OCMC, NUM_SECTIONS_OCMC,
                         MMU_MEMTYPE_NORMAL_SHAREABLE(MMU_CACHE_WT_NOWA,
                                                      MMU_CACHE_WB_WA),
                         MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
                         (unsigned int*)pageTable
                        };

    /*
    ** Define Device Memory Region. The region between OCMC and DDR is
    ** configured as device memory, with R/W access in user/privileged modes.
    ** Also, the region is marked 'Execute Never'.
    */
    REGION regionDev = {
                        MMU_PGTYPE_SECTION, START_ADDR_DEV, NUM_SECTIONS_DEV,
                        MMU_MEMTYPE_DEVICE_SHAREABLE,
                        MMU_REGION_NON_SECURE,
                        MMU_AP_PRV_RW_USR_RW  | MMU_SECTION_EXEC_NEVER,
                        (unsigned int*)pageTable
                       };


    /* Initialize the page table and MMU */
    MMUInit((unsigned int*)pageTable);

    /* Map the defined regions */
    MMUMemRegionMap(&regionDdr);
    MMUMemRegionMap(&regionOcmc);
    MMUMemRegionMap(&regionDev);

    /* Now Safe to enable MMU */
    MMUEnable((unsigned int*)pageTable);
    /* Invalidate the TLB. */
}



