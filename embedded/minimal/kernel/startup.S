/// @brief: Handles all the initialization steps before executing user code.
///   This function is executed by the kernel in SYSTEM mode, right after
///   a reset.
.syntax unified
.section .init
.global _start
.type _start, %function
_start:
    set_stack_pointers:
        ldr r0, =stack_top
        mov sp, r0

    bl copy_sections_to_ram
    b main

.section .stack, "aw", %nobits
.space 0x10000
.balign 8
stack_top:
