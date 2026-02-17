.syntax unified
.thumb
.global _vector_table
.section .vector_table, "ax", %progbits
_vector_table:
    // Exception handlers
    .word ld_stack_top
    .word _reset_handler
    .word nmi_handler
    .word hard_fault_handler
    .word mem_manage_handler
    .word bus_fault_handler
    .word usage_fault_handler
    .word 0x00
    .word 0x00
    .word 0x00
    .word 0x00
    .word svcall_handler
    .word debug_monitor_handler
    .word 0x00
    .word pendsv_handler
    .word systick_handler
    // External interrupts handlers
    .word ext_int0_handler

.thumb
.global _reset_handler
.section .init
.type _reset_handler, %function
_reset_handler:
    b _start

.thumb
.section .init
.type _default_handler, %function
_default_handler:
    b default_handler

// Set weak aliases for default handlers
.weak nmi_handler
.set nmi_handler, _default_handler

.weak hard_fault_handler
.set hard_fault_handler, _default_handler

.weak mem_manage_handler
.thumb_set mem_manage_handler, _default_handler

.weak bus_fault_handler
.thumb_set bus_fault_handler, _default_handler

.weak usage_fault_handler
.thumb_set usage_fault_handler, _default_handler

.weak svcall_handler
.thumb_set svcall_handler, _default_handler

.weak debug_monitor_handler
.thumb_set debug_monitor_handler, _default_handler

.weak pendsv_handler
.thumb_set pendsv_handler, _default_handler

.weak systick_handler
.thumb_set systick_handler, _default_handler

.weak ext_int0_handler
.thumb_set ext_int0_handler, _default_handler
