.extern ld_text_lma, ld_text_vma, ld_text_size
.extern ld_data_origin, ld_data_vma, ld_data_size
.extern ld_bss_vma, ld_bss_size, ld_bss_pattern
.extern ld_user_stack_top, ld_irq_stack_top, ld_fiq_stack_top
.extern ld_svc_stack_top, ld_abt_stack_top, ld_und_stack_top

.equ USR_MODE, 0x10
.equ FIQ_MODE, 0x11
.equ IRQ_MODE, 0x12
.equ SVC_MODE, 0x13
.equ ABT_MODE, 0x17
.equ UND_MODE, 0x1B
.equ SYS_MODE, 0x1F
.equ I_BIT, 0x80
.equ F_BIT, 0x40

/// @brief handles all the initialization steps before executing user code,
/// which include:
/// - Copy LMAs into VMAs for sections .text, .data, .bss, etc.
/// - Initialize the stack pointer of all operation modes.
/// - Change into user mode, activate interruptions.
.arm
.section .init
.global _start
.type _startup, %function
_start:
    // Change operation mode, disable IRQ and FIQ, set stack pointer for each mode
    msr CPSR, #IRQ_MODE|I_BIT|F_BIT // IRQ
    ldr sp, =ld_irq_stack_top

    msr CPSR, #FIQ_MODE|I_BIT|F_BIT // FIQ
    ldr sp, =ld_fiq_stack_top

    msr CPSR, #SVC_MODE|I_BIT|F_BIT // SVC
    ldr sp, =ld_svc_stack_top

    msr CPSR, #ABT_MODE|I_BIT|F_BIT // ABT
    ldr sp, =ld_abt_stack_top

    msr CPSR, #UND_MODE|I_BIT|F_BIT // UND
    ldr sp, =ld_und_stack_top

    msr CPSR, #SYS_MODE|I_BIT|F_BIT // For user mode, change to SYS instead
    ldr sp, =ld_user_stack_top

    // Copy text section
    ldr r0, =ld_text_vma
    ldr r1, =ld_text_lma
    ldr r2, =ld_text_size
    bl _memcopy

    // Copy data section
    ldr r0, =ld_data_vma
    ldr r1, =ld_data_lma
    ldr r2, =ld_data_size
    bl _memcopy

    // Fill bss section
    ldr r0, =ld_bss_vma
    ldr r1, =ld_bss_pattern
    ldr r2, =ld_bss_size
    bl _memfill

    // Change to user mode, enable interrupts
    cpsie aif, #USR_MODE

    b main

/// @brief: Copy a block of memory from a source address to a destination
/// address, given the number of bytes to copy.
/// @param[in]  dst(r0) Destination address (normally VMA).
/// @param[in]  src(r1) Source address (normally LMA).
/// @param[in]  size(r2) Number of bytes of the memory block to copy.
.section .init
.global _memcopy
.type _memcopy, %function
dst     .req r0
src     .req r1
size    .req r2
buffer  .req r3
_memcopy:
    cmp size, #0
    beq loop_memcopy_end
    loop_memcopy:
        ldrb buffer, [src], #1
        strb buffer, [dst], #1
        subs size, size, #1
        bne loop_memcopy
    loop_memcopy_end:
    mov pc, lr

.unreq dst
.unreq src
.unreq size
.unreq buffer

/// @brief Fill a memory address with an 8 bit pattern.
/// @param[in] dst(r0) Destination address.
/// @param[in] pattern(r1) The 8 LSB bits will be used to fill the memory space.
/// @param[in] size(r2) Number of bytes of the memory block to fill.
.section .init
.global _memfill
.type _memfill, %function
dst     .req r0
pattern .req r1
size    .req r2
_memfill:
    cmp size, #0
    beq loop_memfill_end
    loop_memfill:
        strb pattern, [dst], #1
        subs size, size, #1
        bne loop_memfill
    loop_memfill_end:
    mov pc, lr

.unreq dst
.unreq pattern
.unreq size