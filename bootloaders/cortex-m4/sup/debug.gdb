set architecture arm
target remote localhost:2159
lay regs
b _reset_handler
b _start
b _default_handler
b main
