# OpenOCD: On-chip debugger

[OpenOCD][openocd]

Install with:

```bash
sudo apt install openocd
```

OpenOCD uses [TCL][tcl_in_y_minutes].

## Debug adapter hardware

A **dongle** is a small device that plugs into a computer and serves as an adapter from the PC's USB port to the electrical JTAG/SWD interface.

## Using OpenOCD

There are two elements:

* The dongle.
* Te target hardware.

Usually, openocd is executed by calling two configuration scripts:

```bash
openocd -f interface/ADAPTER.cfg -f board/MYBOARD.cfg
```

telnet localhost 4444

## Interfaces

Inside the interface folder, you can see 4 folders than stand out:

ftdi:

This driver is for adapters using the MPSSE (Multi-Protocol Synchronous Serial
Engine) mode built into many FTDI chips, such as the FT2232, FT4232 and FT232H.

microchip:

ft232r

This driver is implementing synchronous bitbang mode of an FTDI FT232R, FT230X,
FT231X and similar USB UART bridge ICs by reusing RS232 signals as GPIO.

<https://github.com/openocd-org/openocd/tree/master/tcl/interface>

<!-- External links -->
[openocd]: https://openocd.org/
[tcl_in_y_minutes]: https://learnxinyminutes.com/tcl/
