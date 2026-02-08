OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

_stack_addr = 0x70020000;

MEMORY {
    ram (rx) : ORIGIN = 0x70010000, LENGTH = 0x1000
}

SECTIONS {
    .text : {
        *startup*(.text)
        *(.text)
    } >ram

    .data : {
        *(.data)
    } >ram

    .bss : {
        *(.bss)
        *(COMMON)
    } >ram
}
