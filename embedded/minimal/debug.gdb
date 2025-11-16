set architecture arm
target extended-remote :3333
monitor arm semihosting enable
lay src
lay regs

b RESET_handler
load
c
