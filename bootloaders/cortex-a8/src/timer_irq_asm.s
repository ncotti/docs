.text
.global timer_irq_asm
.type timer_irq_asm, %function
timer_irq_asm:
    ldr r0, =times_called   // r0 is caller saved
    ldr r10, [r0]
    add r10, r10, #1
    str r10, [r0]
    mov pc, lr

.data
times_called: .word 0
