/// @brief: Copy a block of memory from a source address to a destination
///     address, given the number of bytes to copy.
/// @param: dst (r0): Destination address (normally VMA).
/// @param: src (r1): Source address (normally LMA).
/// @param: size (r2): Number of bytes of the memory block to copy.
.syntax unified
.section .init
.global __memcopy
.type __memcopy, %function
dst     .req r0
src     .req r1
size    .req r2
buffer  .req r3
__memcopy:
    cmp size, #0
    ble memcopy_end
    loop_memcopy:       // Copy each byte into memory
        ldrb buffer, [src], #1
        strb buffer, [dst], #1
        subs size, size, #1
        bne loop_memcopy
    memcopy_end:
        mov pc, lr

.unreq dst
.unreq src
.unreq size
.unreq buffer

/// @brief: Fill a memory address with an 8 bit pattern.
/// @param: dst (r0): Destination address.
/// @param: pattern (r1): The 8 LSB bits will be used to fill the memory space.
/// @param: size (r2): Number of bytes of the memory block to fill.
.section .init
.global __memfill
.type __memfill, %function
dst     .req r0
pattern .req r1
size    .req r2
__memfill:
    cmp size, #0
    ble memfill_end
    loop_memfill:
        strb pattern, [dst], #1
        subs size, size, #1
        bne loop_memfill
    memfill_end:
        mov pc, lr

.unreq dst
.unreq pattern
.unreq size

/// @brief: This function copies all sections from the task1 into RAM:
///   * ".text", ".data" and ".rodata" get copied.
///   * ".bss" gets filled with "0".
.section .init
.type copy_sections_to_ram, %function
.global copy_sections_to_ram
copy_sections_to_ram:
    push {r12, lr}

    ldr r0, =ld_vector_table_vma
    ldr r1, =ld_vector_table_lma
    ldr r2, =ld_vector_table_size
    bl __memcopy

    ldr r0, =ld_text_vma
    ldr r1, =ld_text_lma
    ldr r2, =ld_text_size
    bl __memcopy

    ldr r0, =ld_data_vma
    ldr r1, =ld_data_lma
    ldr r2, =ld_data_size
    bl __memcopy

    ldr r0, =ld_rodata_vma
    ldr r1, =ld_rodata_lma
    ldr r2, =ld_rodata_size
    bl __memcopy

    ldr r0, =ld_bss_vma
    mov r1, #0x00
    ldr r2, =ld_bss_size
    bl __memfill

    pop {r12, lr}
    mov pc, lr
