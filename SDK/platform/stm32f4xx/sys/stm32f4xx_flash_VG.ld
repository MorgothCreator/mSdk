
/******************************************************************************
 *based on linker file at https://launchpadlibrarian.net/99003318/readme.txt
 ******************************************************************************/
 
/* Entry Point */
ENTRY(Reset_Handler)
 
/* Specify the memory areas */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 0x00100000 /*1M*/
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 0x00020000 /*128K*/
  RAM1 (xrw) 	: ORIGIN = 0x10000000, LENGTH = 0x00010000 /*64K*/
}
 


/* define stack size and heap size here */
stack_size = 1024;
heap_size = 10;
 
/* define beginning and ending of stack */
_stack_end = ORIGIN(RAM)+LENGTH(RAM);
_stack_begin = _stack_end - stack_size;
 
 /* higher address of the user mode stack (end of 128K RAM on AHB bus)*/
_estack = _stack_end;
 
/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH
 
  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)
 
    KEEP (*(.init))
    KEEP (*(.fini))
 
    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH
 
  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array*))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH
 
  /* used by the startup to initialize data */
  _sidata = .;
 
  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : AT ( _sidata )
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
 
    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM
 
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
 
    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );


 
. = ALIGN(4);
.heap :
{
_heap_start = .;
. = . + heap_size;
} > RAM
 
. = ALIGN(4);
. = _stack_begin;
.stack :
{
 
. = . + stack_size;
} > RAM
 
 
}