OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

HIDDEN (ram_origin = 0x40000000);
HIDDEN (ram_length = 0x10000000);
HIDDEN (rom_origin = 0x00000000);
HIDDEN (rom_length = 0x10000000);

MEMORY {
    ram	(!RX) : ORIGIN = ram_origin, LENGTH = ram_length
    rom (RX)  : ORIGIN = rom_origin, LENGTH = rom_length
}

SECTIONS {
    .text : AT(0x02000000) ALIGN(4) {
        *(.text*)
    } >ram

    .data : AT(0x03000000) ALIGN(256) {
         *(.data*)
    } >ram

    .bss 0x43000000 (NOLOAD) : ALIGN(4) {
        *(.bss*)
    }
}
