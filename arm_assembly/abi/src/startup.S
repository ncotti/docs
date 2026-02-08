#include "header.h"

.global _start
.type _start, %function
.text
_start:
    ldr sp, =_stack_addr
    call_c_function_from_assembly:
        mov r0, #0
        mov r0, #1
        mov r1, #TWO    // Using C macro
        mov r2, #THREE
        mov r3, #4
        mov r4, #5
        mov r5, #6
        mov r6, #7
        mov r7, #8
        mov r8, #9
        push {r4-r8, r12} // Pushing arguments 5 to 9 into the stack
        blx add9    // r0 = 1 + 2*2 + 3*3 + 4*4 + 5*5 + 6*6 + 7*7 + 8*8 + 9*9 = 285
        pop {r4-r8, r12}

    using_c_macros_and_defines:
        mov r0, #15
        ADD_TO_R0(#20)  // r0 = 15 + 20 = 35

    access_C_variable:
        ldr r0, =var_from_c
        ldr r1, [r0]
        add r1, r1, #1
        str r1, [r0]

    b main


.data   // Variables used in C
.global var_from_asm
var_from_asm: .word 5

.global vector_from_asm
vector_from_asm: .word 0, 1, 2, 3, 4, 5


// Function defined in assembly called from C.
.text
.type add6_from_asm, %function // MANDATORY, or will not work
.global add6_from_asm
add6_from_asm:
    add r0, r0, r1
    add r0, r0, r2
    add r0, r0, r3
    ldr r1, [sp]        // Access argument 5
    add r0, r0, r1
    ldrb r1, [sp, #4]   // Access argument 6, it is a char, 1 byte
    add r0, r0, r1
    mov pc, lr
