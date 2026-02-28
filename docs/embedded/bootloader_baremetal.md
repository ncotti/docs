# Bare-metal bootloader

There is no way around it. For a bare-metal application, you MUST write your own assembly bootloader (or use a vendor IDE which should provide you with one by default).

## Bootloader's responsibilities

When a program is flashed into an embedded device, it's usually stored in a flash memory or an eMMC. Although possible, code execution is not suitable from neither of those because:

* Access times are considerably slower than an SRAM.
* They usually have fewer read/write cycles than an SRAM.
* They are sometimes read in blocks, not allowing single values reads.

Therefore, it is the bootloader's duty to copy all instructions (`.text`) and data (`.rodata`, `.data`, `.bss`, `.COMMON`) from the slower flash/eMMC to the faster SRAM/SDRAM.

Another things the bootloader must do is to initialize the stack pointer, enable interruptions and configure any architecture-specific registers (such as enable NEON's coprocessor for floating point).

After everything has been initialized, the last step is to concede execution to the C's `main()` function.

What your bootloader should not do is to initialize peripherals: that work is easier done from C code than assembler.

## Bootloaders by architecture

The following is the list of architectures that the author has successfully written a working project with a bootloader.

## Cortex-A bootloader

This bootloader implementation is heavily inspired on Chapter 14 of the book: [Modern Assembly Language Programming with the ARM Processor, Larry D Pyeatt][arm_book].

Check the implementation on the [Cortex-A8 example][cortex_a8_example]. The steps taken by the bootloader are:

* Initialize all stack pointer for different CPU modes.
* Copy all instruction and data from its LMA (flash) to its VMA (SRAM).
* Initialize all `.bss` data to zeros.
* Change to user mode, enable interruptions.
* Set a weak link to a default handler for all interrupt sources.
* Branch to `main()`.

## Cortex-M bootloader

TODO [Cortex-M4 example][cortex_m4_example]

<!--External links-->
[arm_book]: https://shop.elsevier.com/books/modern-assembly-language-programming-with-the-arm-processor/pyeatt/978-0-443-14114-0

<!--Internal links-->
[cortex_a8_example]: https://github.com/ncotti/docs/tree/main/bootloaders/cortex-a8/startup
[cortex_m4_example]: https://github.com/ncotti/docs/tree/main/bootloaders/cortex-m4/startup
