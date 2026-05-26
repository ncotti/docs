# Hardware probing

Hardware probing is the action of reading or writing a device's memory from an **external** interface. They are normally used for flashing code into a device or to access the debug interfaces.

It should not be mistaken with what bootloaders do. Having a bootloader on a device means that code is being executed and that memory is being accessed from the MCU in its normal/expected fashion, from **internal** interfaces. A hardware probe is a third-party intruder, completely separated from the board, that reads/writes memory in a disruptive fashion.
