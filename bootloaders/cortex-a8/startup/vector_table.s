/// @brief Interrupt Vector Table
.arm
.global _vector_table
.section .vector_table, "ax", %progbits
_vector_table:
    // Load contents of 24 bytes ahead, instead of 32, because the PC
    // advances two positions in the pipeline for the execution
    ldr pc, [pc, #24]
    ldr pc, [pc, #24]
    ldr pc, [pc, #24]
    ldr pc, [pc, #24]
    ldr pc, [pc, #24]
    ldr pc, [pc, #24]
    ldr pc, [pc, #24]
    ldr pc, [pc, #24]
    rh: .word _reset_handler
    uh: .word _undef_handler
    sh: .word _svc_handler
    ph: .word _prefetch_handler
    dh: .word _abort_handler
    vh: .word 0x00
    ih: .word _irq_handler
    fh: .word _fiq_handler

/// @brief Reset handler
.global _reset_handler
.section .init
.type _reset_handler, %function
_reset_handler:
    b _start

/// @brief Undefined instruction handler
.text
.type _undef_handler, %function
_undef_handler:
    push {r0-r3, r12, lr}
    bl undef_handler
    pop {r0-r3, r12, lr}
    movs pc, lr


/// @brief Software interrupt handler (Supervisor Call)
.text
.type _svc_handler, %function
_svc_handler:
    push {r0-r3, r12, lr}
    bl svc_handler
    pop {r0-r3, r12, lr}
    movs pc, lr

/// @brief Prefetch error handler
.text
.type _prefetch_handler, %function
_prefetch_handler:
    push {r0-r3, r12, lr}
    bl prefetch_handler
    pop {r0-r3, r12, lr}
    subs pc, lr, #4

/// @brief Data abort handler
.text
.type _abort_handler, %function
_abort_handler:
    push {r0-r3, r12, lr}
    bl abort_handler
    pop {r0-r3, r12, lr}
    subs pc, lr, #8

/// @brief Interrupt handler (IRQ)
.text
.type _irq_handler, %function
_irq_handler:
    push {r0-r3, r12, lr}
    bl irq_handler
    pop {r0-r3, r12, lr}
    subs pc, lr, #4

/// @brief Fast Interrupt handler (FIQ)
.text
.type _fiq_handler, %function
_fiq_handler:
    push {r0-r3, r12, lr}
    bl fiq_handler
    pop {r0-r3, r12, lr}
    subs pc, lr, #4

/// @brief Deafult Handler (weak symbol)
.text
.type _default_handler, %function
_default_handler:
    bl default_handler

/// Set weak aliases to the default handler
.weak undef_handler
.set undef_handler, _default_handler

.weak svc_handler
.set svc_handler, _default_handler

.weak prefetch_handler
.set prefetch_handler, _default_handler

.weak abort_handler
.set abort_handler, _default_handler

.weak irq_handler
.set irq_handler, _default_handler

.weak fiq_handler
.set fiq_handler, _default_handler
