.syntax unified
.thumb
.global _start
.section .init
.type _start, %function
_start:

    // Initialize stack pointer
    ldr sp, =ld_stack_top

    .short 0x7059
    .short 0x7777

    @ mov r0, #1
    @ mrs r1, CONTROL
    @ msr CONTROL, r0

    mov r0, #0
    cmp r0, #0
    mov r1, #1
    cmp r0, #0

    mrs r0, xPSR

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

    // Copy rodata section
    ldr r0, =ld_rodata_vma
    ldr r1, =ld_rodata_lma
    ldr r2, =ld_rodata_size
    bl _memcopy

    // Fill bss section
    ldr r0, =ld_bss_vma
    ldr r1, =ld_bss_pattern
    ldr r2, =ld_bss_size
    bl _memfill

    mov r0, #0
    msr BASEPRI, r0
    cpsie fi
    b main



/// @brief: Copy a block of memory from a source address to a destination
/// address, given the number of bytes to copy.
/// @param[in]  dst(r0) Destination address (normally VMA).
/// @param[in]  src(r1) Source address (normally LMA).
/// @param[in]  size(r2) Number of bytes of the memory block to copy.
.thumb
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
.thumb
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
