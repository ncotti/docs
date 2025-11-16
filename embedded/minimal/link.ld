/******************************************************************************
*   Output format
******************************************************************************/
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(RESET_handler)

/******************************************************************************
*   Memory sections
******************************************************************************/
HIDDEN(ld_rom_origin = 0x08000000);
ld_vector_table_vma = 0x00;

MEMORY {
    rom ( rx )      : ORIGIN = ld_rom_origin, LENGTH = 96K
    ram ( rxw )       : ORIGIN = 0x20000000, LENGTH = 512K
}

/******************************************************************************
*   Section definitions
******************************************************************************/
SECTIONS {

    .init : AT(ld_rom_origin) ALIGN(4) {
        *vector_table*(.init)  /* Make sure the reset handler is the first instruction run */
        *(.init)
    } >rom
    PROVIDE(ld_init_vma = ADDR(.init));
    PROVIDE(ld_init_size = SIZEOF(.init));

    .vector_table ld_vector_table_vma : ALIGN(4) {
        *(.vector_table)
    } AT>rom
    PROVIDE(ld_vector_table_lma = LOADADDR(.vector_table));
    PROVIDE(ld_vector_table_size = SIZEOF(.vector_table));

    .text : ALIGN(4) {
        *(.text)
    } >ram AT>rom
    PROVIDE(ld_text_lma = LOADADDR(.text));
    PROVIDE(ld_text_vma = ADDR(.text));
    PROVIDE(ld_text_size = SIZEOF(.text));

    .data : ALIGN(4) {
        *(.data)
    } >ram AT>rom
    PROVIDE(ld_data_lma = LOADADDR(.data));
    PROVIDE(ld_data_vma = ADDR(.data));
    PROVIDE(ld_data_size = SIZEOF(.data));

    .rodata : ALIGN(4) {
        *(.rodata)
    } >ram AT>rom
    PROVIDE(ld_rodata_lma = LOADADDR(.rodata));
    PROVIDE(ld_rodata_vma = ADDR(.rodata));
    PROVIDE(ld_rodata_size = SIZEOF(.rodata));

    .stack (NOLOAD) : ALIGN(8) {
        *(.stack)
    } >ram
    PROVIDE(ld_stack_vma = ADDR(.stack));
    PROVIDE(ld_stack_size = SIZEOF(.stack));

    .bss (NOLOAD) : ALIGN(4) {
        *(.bss)
        *(COMMON)
    } >ram
    PROVIDE(ld_bss_vma = ADDR(.bss));
    PROVIDE(ld_bss_size = SIZEOF(.bss));
}
