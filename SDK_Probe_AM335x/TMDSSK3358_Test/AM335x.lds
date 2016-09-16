/****************************************************************************/
/*  AM335x.lds                                                              */
/*  Copyright (c) 2014  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the GCC compiler    */
/*                 and running the resulting .out file on an AM335x device. */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/


/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory region DDR0.
 * It references following symbols, which must be defined in code:
 *   Entry : Entry of reset handler
 * 
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __HeapBase        - To be compatible with Linaro's semihosting support 
 *   __StackLimit
 *   __StackTop
 *   __StackBase    - To be compatible with Linaro's semihosting support 
 *   __stack
 */
ENTRY(Entry)

/*
** The stack is kept at end of the image, and its size is 128 MB.
** The heap section is placed above the stack to support I/O
** operations using semihosting. The size of the section is 2KB.
*/

MEMORY
{
    OCMC :   o = 0x40300000,  l = 0xFFFF      /* 64k L3 OCMC SRAM */
    DDR0 :   o = 0x80000000,  l = 0x10000000  /* 256MB external DDR Bank 0 */
}

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS
{

        .rsthand :
        {
			. = ALIGN(4);
        	*init.o      (.text)
        } >DDR0


        .text :
        {
            . = ALIGN(4);
            *(.text*)
            *(.rodata*)
        } >DDR0

        .data :
        {
            . = ALIGN(4);
            *(.data*)
        } >DDR0

        /*iram_start = (ADDR (.data) + SIZEOF (.data));
        .reloc_iram :
        AT (iram_start)
        {
            . = ALIGN(4);
            relocstart = . ;
                *("IRAM_CODE")
                *("IRAM_DATA")
            relocend = . ;
            . = ALIGN(4);
        } >OCMC*/

        .bss :
        {
            . = ALIGN(4);
            _bss_start = .;
            __bss_start__ = .;
            *(.bss*)
            *(COMMON)
            _bss_end = .;
            __bss_end__ = .;
        } >DDR0

        .heap :
        {
            . = ALIGN(4);
            __end__ = .;
            end = __end__;
            __HeapBase = __end__;
            *(.heap*)
            . = . + HEAPSIZE;
            __HeapLimit = .;
        } >DDR0

        .stack :
        {
            . = ALIGN(4);
            __StackLimit = . ;
            *(.stack*)
            . = . + 0x7FFFFF8;
            __StackTop = .;
        _stack = __StackTop;
        __stack = __StackTop;
        } >DDR0

}
