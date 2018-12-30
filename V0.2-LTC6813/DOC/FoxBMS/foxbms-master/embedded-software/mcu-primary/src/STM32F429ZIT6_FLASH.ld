/*
*****************************************************************************
**

**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32F429ZITx Device with
**                2048KByte FLASH, 192KByte RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** <h2><center>&copy; COPYRIGHT(c) 2014 Ac6</center></h2>
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**   1. Redistributions of source code must retain the above copyright notice,
**      this list of conditions and the following disclaimer.
**   2. Redistributions in binary form must reproduce the above copyright notice,
**      this list of conditions and the following disclaimer in the documentation
**      and/or other materials provided with the distribution.
**   3. Neither the name of Ac6 nor the names of its contributors
**      may be used to endorse or promote products derived from this software
**      without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x2002FFFF;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;;      /* required amount of heap  */
_Min_Stack_Size = 0x400;; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
/* FLASH (rx)   : ORIGIN   = 0x8000000, LENGTH = 2048K */
FLASH (rx)         : ORIGIN = 0x08000000, LENGTH = 1024K-256
FLASH_HEADER (rx)  : ORIGIN = 0x080FFF00, LENGTH = 256
FLASH_2 (rx)       : ORIGIN = 0x08100000, LENGTH = 1024K
RAM (xrw)          : ORIGIN = 0x20000000, LENGTH = 192K
CCMRAM (rw)        : ORIGIN = 0x10000000, LENGTH = 64K
BKP_RAM(rw)        : ORIGIN = 0x40024000, LENGTH = 4K
EXT_SDRAM (xrw)    : ORIGIN = 0xD0000000, LENGTH = 8192K
}

/* Define output sections */
SECTIONS
{
  .flashheader :
  {
    . = ALIGN(4);
      *(.FLASH_HEADERSection*)
    . = ALIGN(4);
  } >FLASH_HEADER
 
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(16);	 /* note: this must be done as the next section is aligned to 16*/
  } >FLASH = 0xFF	 /* and the Fill command (0xFF) works only within the output section*/
                     /* this means the space between the output sections on the target memory could be differently handled */ 
                     /* by the programming tool or algorithm */
                     /* flash checksum verification feature needs initialized values of 0xFF in all unused memory areas !!!*/

 _foxBMS_FLASH_end   = ORIGIN(FLASH)   + LENGTH(FLASH) + LENGTH(FLASH_HEADER);
 _foxBMS_FLASH_2_end = ORIGIN(FLASH_2) + LENGTH(FLASH_2);

_sflash1 = ORIGIN(FLASH);
_jumpadressvectable = ORIGIN(FLASH) + 4;

  .text :
  {
/* input path: "...engine...",  section type: "text.<symbolname>"
   input path: "...general...", section type: "text.<symbolname>"
*/   
  
    . = ALIGN(16);
      *arm-none-eabi*(.text*)
      *libgcc.a:(.text*)
      *libc*.a:(.text*)
      *libg*.a:(.text*)
      *limc*.a:(.text*)
 

      /**** HAL  ****/
      *src/hal/*HAL_Driver*(.text*)
      *libHAL.a:(.text*)
      
      *hal.a:(.text*)


      /**** CMSIS  ****/
      *src/hal/CMSIS*(.text*)
      
      *CMSIS.a:(.text*)


      /**** os  ****/
      */os/*(.text*)
      *libOS.a:(.text*)
      
      *os.a:(.text*)
      

      /**** engine  ****/      
      *src/engine*(.text*)
      
      *engine*.a:(.text*)

      
      /**** general  ****/
      *src/general*(.text*)
      
      *src\\general*(.text*)


      /**** module  ****/    
      *src/module*(.text*)
      
      *module.a:(.text*)

      
      /**** application  ****/
      *src/application*(.text*)
      
      *application.a:(.text*)


/*    *src/test*(.text*)
      *test.a:(.text*)
*/
    
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(16);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH = 0xFF

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH = 0xFF

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH = 0xFF

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH = 0xFF
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH = 0xFF
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH = 0xFF

  /* used by the startup to initialize data */
  _sidata_ext = LOADADDR(.ext_sdramsect_data);     /* define a global symbol at load address in Flash region for SDRAM data section*/

  /* Initialized external SDRAM section of type data*/
  .ext_sdramsect_data :
  {
    . = ALIGN(4);          /* define a global symbol at data start of external SDRAM */
    _s_extdata = .;
    *sdram_cfg*(.data*)
    . = ALIGN(4);
    _e_extdata = .;        /* define a global symbol at data end of external SDRAM */
  } >EXT_SDRAM AT> FLASH = 0xFF

  
  _sidata = LOADADDR(.data); /* define a global symbol at load address in Flash region for internal RAM data section */

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH = 0xFF

  
  _siccmram = LOADADDR(.ccmram);

  /* CCM-RAM section 
  * 
  * IMPORTANT NOTE! 
  * If initialized variables will be placed in this section,
  * the startup code needs to be modified to copy the init-values.  
  */
  .ccmram :
  {
    . = ALIGN(4);
    _sccmram = .;       /* create a global symbol at ccmram start */
    *(.ccmram)
    *(.ccmram*)
    . = ALIGN(4);
    _eccmram = .;       /* create a global symbol at ccmram end */
  } >CCMRAM AT> FLASH = 0xFF
  
  _eflash1 = _siccmram + (_eccmram - _sccmram);

  /* Uninitialized Backup SRAM section of type bss*/
  . = ALIGN(4);
  .bkp_ramsect (NOLOAD) :
  {
 /*   *(.BKP_RAMSection) */
    *(.BKP_RAMSection*)
 /*   *(BKP_RAMSection) */

  } >BKP_RAM

  /* Uninitialized external SDRAM section of type bss*/
  . = ALIGN(4);
  .ext_sdramsect_bss :
  {
      *sdram_cfg*(.bss*)
      *(.EXT_SDRAMSection*)
  } >EXT_SDRAM
  
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

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(4);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(4);
  } >RAM

  

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}


