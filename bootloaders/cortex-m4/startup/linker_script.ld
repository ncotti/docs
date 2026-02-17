/*-----------------------------------------------------------------------------
*   Output format
-----------------------------------------------------------------------------*/
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_reset_handler)

/*-----------------------------------------------------------------------------
*   Stack sizes
-----------------------------------------------------------------------------*/
HIDDEN(ld_stack_size  = 0x1000);   /* 1KB */

/*-----------------------------------------------------------------------------
*   Memory regions' origins and lengths
-----------------------------------------------------------------------------*/
/* Aliased from 0x0800_0000 to 0x0000_0000 */
HIDDEN(ld_flash_origin          = 0x08000000);
HIDDEN(ld_flash_length          = 0x00100000);   /* 1 MB */

HIDDEN(ld_ram_origin            = 0x10000000);
HIDDEN(ld_ram_length            = 0x00010000);  /* 64 KB */

/*-----------------------------------------------------------------------------
*   Memory sections
-----------------------------------------------------------------------------*/
MEMORY {
    flash   (rx)    :   ORIGIN = ld_flash_origin,   LENGTH = ld_flash_length
    ram     (wax)   :   ORIGIN = ld_ram_origin,     LENGTH = ld_ram_length
}

/*-----------------------------------------------------------------------------
*   Section definitions
-----------------------------------------------------------------------------*/
SECTIONS {

    /* The vector table should always be the first thing to store in memory.
    Must be identity mapped. */
    .vector_table : ALIGN(4) {
        *(.vector_table*)
    } >flash AT>flash
    PROVIDE(ld_vector_table_lma = LOADADDR(.vector_table));
    PROVIDE(ld_vector_table_vma = ADDR(.vector_table));
    PROVIDE(ld_vector_table_size = SIZEOF(.vector_table));

    /* Init section has all the boot code, including stack initialization and
    memcopys from flash to ram.
    Must be identity mapped. */
    .init : ALIGN(4) {
        *(.init*)
    } >flash AT>flash

    /* User code is copied from flash to RAM before executing */
    .text : ALIGN(4) {
        *(.text*)
    } >ram AT>flash
    PROVIDE(ld_text_lma = LOADADDR(.text));
    PROVIDE(ld_text_vma = ADDR(.text));
    PROVIDE(ld_text_size = SIZEOF(.text));

    /* Initialized data */
    .data : ALIGN(4) {
        *(.data*)
    } >ram AT>flash
    PROVIDE(ld_data_lma = LOADADDR(.data));
    PROVIDE(ld_data_vma = ADDR(.data));
    PROVIDE(ld_data_size = SIZEOF(.data));

    .rodata : ALIGN(4) {
        *(.rodata*)
    } >ram AT>flash
    PROVIDE(ld_rodata_lma = LOADADDR(.rodata));
    PROVIDE(ld_rodata_vma = ADDR(.rodata));
    PROVIDE(ld_rodata_size = SIZEOF(.rodata));

    /* Unitialized data */
    .bss (NOLOAD) : ALIGN(4) {
        *(.bss*)
        *(COMMON)
    } >ram
    PROVIDE(ld_bss_vma = ADDR(.bss));
    PROVIDE(ld_bss_size = SIZEOF(.bss));
    PROVIDE(ld_bss_pattern = 0x00);

    .stack (NOLOAD) : ALIGN(4) {
        . += ld_stack_size;
        PROVIDE(ld_stack_top = .);
    } >ram
    PROVIDE(ld_stack_size = SIZEOF(.stack));
}
