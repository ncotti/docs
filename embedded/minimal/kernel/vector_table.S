/******************************************************************************
* Instructions to be loaded in the vector table
******************************************************************************/
.section .vector_table, "ax", %progbits
vector_table:
    .word =stack_top
    .word =RESET_handler
    .word =NMI_handler
    .word =HardFault_handler
    .word =MemManageFault_handler
    .word =BusFault_handler
    .word =UsageFault_handler
    .word 0x0
    .word 0x0
    .word 0x0
    .word 0x0
    .word 0x0
    .word =SVC_handler

/******************************************************************************
* Exception handlers
******************************************************************************/
/// @brief Reset handler. First instruction executed on power up.
.global RESET_handler
.section .init
.type RESET_handler, %function
RESET_handler:
    b _start

.text
.type NMI_handler, %function
NMI_handler:
    WFI

.text
.type HardFault_handler, %function
HardFault_handler:
    WFI

.text
.type MemManageFault_handler, %function
MemManageFault_handler:
    WFI

.text
.type BusFault_handler, %function
BusFault_handler:
    WFI

.text
.type UsageFault_handler, %function
UsageFault_handler:
    WFI

.text
.type SVC_handler, %function
SVC_handler:
    WFI
