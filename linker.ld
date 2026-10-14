/* STM32F767 memory map - from reference manual */
ENTRY(Reset_Handler)

MEMORY
{
    /* 2MB of flash starting at 0x08000000 */
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K

    /* 512KB of RAM starting at 0x20000000 */
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 512K
}

SECTIONS
{
    /* .text - your actual program code goes in flash */
    .text :
    {
        KEEP(*(.isr_vector))    /* interrupt vector table must be first */
        *(.text*)               /* all code */
        *(.rodata*)             /* read only data - string literals, const variables */
    } > FLASH

    /* .data - initialized global variables
     * stored in flash but copied to RAM on startup */
    .data :
    {
        _sdata = .;             /* start of data section in RAM */
        *(.data*)
        _edata = .;             /* end of data section in RAM */
    } > RAM AT > FLASH          /* lives in RAM, loaded from FLASH */

    _sidata = LOADADDR(.data);  /* where .data lives in flash */

    /* .bss - uninitialized global variables
     * zeroed out in RAM on startup */
    .bss :
    {
        _sbss = .;              /* start of bss */
        *(.bss*)
        *(COMMON)
        _ebss = .;              /* end of bss */
    } > RAM

    PROVIDE(end = .);

    /* stack grows downward from end of RAM */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
