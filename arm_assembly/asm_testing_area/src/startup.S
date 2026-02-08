#include "header.h"

.macro SHIFT arg1, arg2=5
    lsl \arg1, \arg1, #\arg2
.endm

.global _start
.type _start, %function
.text
_start:
    ldr sp, =_stack_addr

    // Put here your Assembly code
    mov r0, #0
    loop:
        add r0, r0, #1
        cmp r0, #5 
        blt loop

    b main
