/* Linker script for STM32G070 */

ENTRY(Reset_Handler)

/* Memory layout for STM32G070 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 36K
}

/* Sections */
SECTIONS
{
  /* Vector table goes at the start of Flash */
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  /* Program code and read-only data */
  .text :
  {
    *(.text)           /* All normal code */
    *(.text*)          /* Any other code sections */
    *(.rodata)         /* Read-only data */
    *(.rodata*)        /* Any other read-only sections */
    KEEP(*(.init))     /* Init code */
    KEEP(*(.fini))     /* Finalization code */
  } > FLASH

  /* Constructor and destructor tables (for C++) */
  .preinit_array :
  {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } > FLASH

  .init_array :
  {
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
  } > FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } > FLASH

  /* Initialized global/static variables */
  .data : AT(ADDR(.text) + SIZEOF(.text))
  {
    _sdata = .;
    *(.data)
    *(.data*)
    _edata = .;
  } > RAM

  /* Uninitialized global/static variables (zero-init) */
  .bss :
  {
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  /* User stack (grows downward) */
  ._user_stack :
  {
    . = ALIGN(8);
    _estack = .;
  } > RAM
}

/* Symbols to help C startup code */
PROVIDE(_sidata = LOADADDR(.data));
PROVIDE(_sdata = ADDR(.data));
PROVIDE(_edata = ADDR(.data) + SIZEOF(.data));
PROVIDE(_sbss = ADDR(.bss));
PROVIDE(_ebss = ADDR(.bss));
PROVIDE(_estack = ORIGIN(RAM) + LENGTH(RAM));
PROVIDE(end = _ebss);
