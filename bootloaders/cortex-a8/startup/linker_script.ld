/*-----------------------------------------------------------------------------
*   Output format
-----------------------------------------------------------------------------*/
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_reset_handler)

/*-----------------------------------------------------------------------------
*   Stack sizes
-----------------------------------------------------------------------------*/
/* They should be multiples of 8 */
HIDDEN(ld_user_stack_size  = 0x4000);   /* 16KB */
HIDDEN(ld_irq_stack_size   = 0x1000);   /* 4KB */
HIDDEN(ld_fiq_stack_size   = 0x1000);   /* 4KB */
HIDDEN(ld_svc_stack_size   = 0x1000);   /* 4KB */
HIDDEN(ld_abt_stack_size   = 0x1000);   /* 4KB */
HIDDEN(ld_und_stack_size   = 0x1000);   /* 4KB */

/*-----------------------------------------------------------------------------
*   Memory regions' origins and lengths
-----------------------------------------------------------------------------*/
/* NOR flash (SMC CS0) Remapped from 0x4000_0000 to 0x7000_0000 */
HIDDEN(ld_flash_origin          = 0x70000000);
HIDDEN(ld_flash_length          = 0x01000000);   /* 16 MB */

/* The stack is stored in SDRAM, after the flash section */
HIDDEN(ld_stack_origin = ld_flash_origin + ld_flash_length);
HIDDEN(ld_stack_length = ld_user_stack_size + ld_irq_stack_size +
    ld_fiq_stack_size + ld_svc_stack_size + ld_abt_stack_size +
    ld_und_stack_size);

PROVIDE(ld_heap_origin = ld_stack_origin + ld_stack_length);
PROVIDE(ld_heap_length = 0x00100000);     /* 1MB */
PROVIDE(ld_heap_end = ld_heap_origin + ld_heap_length);

/* Rest of user RAM */
HIDDEN(ld_ram_origin    = ld_heap_origin + ld_heap_length);

/* 256 MB - flash size - stack size */
HIDDEN(ld_ram_length    = 0x10000000 - ld_stack_length - ld_flash_length - ld_heap_length);


/*-----------------------------------------------------------------------------
*   Memory sections
-----------------------------------------------------------------------------*/
MEMORY {
    flash   (rx)    :   ORIGIN = ld_flash_origin,   LENGTH = ld_flash_length
    stack   (wa)    :   ORIGIN = ld_stack_origin,   LENGTH = ld_stack_length
    heap    (wa)    :   ORIGIN = ld_heap_origin,    LENGTH = ld_heap_length
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

    /* Stack for all operation modes */
    .stack (NOLOAD) : ALIGN(8) {
        . += ld_user_stack_size;
        . = ALIGN(8);
        PROVIDE(ld_user_stack_top = .);

        . += ld_irq_stack_size;
        . = ALIGN(8);
        PROVIDE(ld_irq_stack_top = .);

        . += ld_fiq_stack_size;
        . = ALIGN(8);
        PROVIDE(ld_fiq_stack_top = .);

        . += ld_svc_stack_size;
        . = ALIGN(8);
        PROVIDE(ld_svc_stack_top = .);

        . += ld_abt_stack_size;
        . = ALIGN(8);
        PROVIDE(ld_abt_stack_top = .);

        . += ld_und_stack_size;
        . = ALIGN(8);
        PROVIDE(ld_und_stack_top = .);
    } >stack
}
