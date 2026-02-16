set architecture arm
target remote localhost:2159
lay regs
#b _reset_handler
#b _start
b main

b _undef_handler
b _svc_handler
b _prefetch_handler
b _abort_handler
b _irq_handler
b _fiq_handler
b _default_handler
#b _write
#b _sbrk
