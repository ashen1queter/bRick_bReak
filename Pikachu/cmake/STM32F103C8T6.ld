MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 20K
}

ENTRY(Reset_Handler)

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))      /* Interrupt vector table */
    *(.text*)                 /* Code */
    *(.rodata*)               /* Read-only data */
    _etext = .;               /* End of text section */
  } > FLASH

  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  /* Stack top and heap boundaries */
  _estack = ORIGIN(RAM) + LENGTH(RAM);
  _Min_Heap_Size = 0x200; /* 512 bytes */
  _Min_Stack_Size = 0x400; /* 1 KB */
}
