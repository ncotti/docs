set architecture armv7e-m
target remote localhost:3333
lay regs
b _reset_handler
b _start
b _default_handler
b main
