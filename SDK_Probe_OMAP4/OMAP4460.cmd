/****************************************************************************/
/*  OMAP4460.cmd                                                            */
/*  Copyright (c) 2012  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an OMAP4460.          */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/
-stack  0x100                              /* SOFTWARE STACK SIZE           */
-heap   0x1000000                            /* HEAP AREA SIZE                */

-e Entry

/* Since we used 'Entry' as the entry-point symbol the compiler issues a    */
/* warning (#10063-D: entry-point symbol other than "_c_int00" specified:   */
/* "Entry"). The CCS Version (5.1.0.08000) stops building from command      */
/* line when there is a warning. So this warning is suppressed with the     */
/* below flag. */

--diag_suppress=10063



MEMORY
{
#ifndef M3_CORE     /* A9 memory map */

    L3_OCM_RAM      o = 0x40300000  l = 0x0000E000  /* 56kB L3 Internal memory */
    EMIF_CS0_SDRAM  o = 0x80000000  l = 0x10000000  /* 1GB external SDRAM memory on CS0 */
    EMIF_CS1_SDRAM  o = 0xC0000000  l = 0x10000000  /* 1GB external SDRAM memory on CS1 */

#else               /* M3 memory map */

    M3ROM           o = 0x55000000  l = 0x00004000  /* 16kB local ROM */
    M3RAM           o = 0x55020000  l = 0x00010000  /* 64kB local RAM */

#endif
}

SECTIONS
{
#ifndef M3_CORE    /* A9 memory map */

    .text          >  EMIF_CS0_SDRAM
    .stack         >  EMIF_CS0_SDRAM
    .bss           >  EMIF_CS0_SDRAM
                    RUN_START(bss_start)
                    RUN_END(bss_end)
    .cio           >  EMIF_CS0_SDRAM
    .const         >  EMIF_CS0_SDRAM
    .data          >  EMIF_CS0_SDRAM
    .switch        >  EMIF_CS0_SDRAM
    .sysmem        >  EMIF_CS0_SDRAM
    .far           >  EMIF_CS0_SDRAM
    .args          >  EMIF_CS0_SDRAM
    .ppinfo        >  EMIF_CS0_SDRAM
    .ppdata        >  EMIF_CS0_SDRAM
  
    /* TI-ABI sections */
    .pinit         >  EMIF_CS0_SDRAM
    .cinit         >  EMIF_CS0_SDRAM
  
    /* EABI sections */
    .binit         >  EMIF_CS0_SDRAM
    .init_array    >  EMIF_CS0_SDRAM
    .neardata      >  EMIF_CS0_SDRAM
    .fardata       >  EMIF_CS0_SDRAM
    .rodata        >  EMIF_CS0_SDRAM
    .c6xabi.exidx  >  EMIF_CS0_SDRAM
    .c6xabi.extab  >  EMIF_CS0_SDRAM

#else              /* M3 memory map */

    .text          >  M3RAM
    .stack         >  M3RAM
    .bss           >  M3RAM
    .cio           >  M3RAM
    .const         >  M3RAM
    .data          >  M3RAM
    .switch        >  M3RAM
    .sysmem        >  M3RAM
    .far           >  M3RAM
    .args          >  M3RAM
    .ppinfo        >  M3RAM
    .ppdata        >  M3RAM
  
    /* TI-ABI or COFF sections */
    .pinit         >  M3RAM
    .cinit         >  M3RAM
  
    /* EABI sections */
    .binit         >  M3RAM
    .init_array    >  M3RAM
    .neardata      >  M3RAM
    .fardata       >  M3RAM
    .rodata        >  M3RAM
    .c6xabi.exidx  >  M3RAM
    .c6xabi.extab  >  M3RAM

#endif
}
