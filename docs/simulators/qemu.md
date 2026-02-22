# QEMU

!!! warning
    It is not recommend to start new developments with QEMU. Go see the [Renode section][renode_section] for the preferred hardware simulator.

The **Q**uick **EMU**lator ([QEMU][qemu]) performs system emulation, i.e. allows you to run a completely different [CPU architecture][guest_archs] (such as ARM, MIPS, RISC-V, AVR, etc) with a different OS than those of the host machine.

QEMU is most suitable for running pre-configured boards, not custom hardware, as discussed in the [Why QEMU doesn't support custom boards section][qemu_custom_board_support_section].

## Running QEMU, TL;DR

Load an ELF file `exe.elf` into the board, open a serial connection through telnet on port 2222, wait for a GDB connection on port 2159.

```bash
ELF="exe.elf"

qemu-system-arm \
    -M realview-pb-a8  \
    -m 32M \
    -no-reboot -nographic \
    -monitor stdio \
    -serial telnet:localhost:2222,server,nowait \
    -S -gdb tcp:localhost:2159 \
    -device loader,file=${ELF}
```

On other terminal, connect to the telnet server for the serial communication:

```bash
telnet localhost 2222
```

Finally, connect the gdb session in other terminal:

```bash
gdb-multiarch exe.elf
(gdb) set architecture arm
(gdb) target remote localhost:2159
(gdb) lay regs
(gdb) b main
(gdb) c
```

## Invocation arguments

For each type of machine architecture, e.g ARM, RISC-V, AVR, etc; QEMU has its own [QEMU System Emulator Target][qemu_targets] or binary. For example, `qemu-system-arm`, `qemu-system-avr` or `qemu-system-riscv32`.

After choosing the architecture, the next step is to [choose the board model][qemu_choose_board]. Each board has its own set of simulated peripherals as can be seen, for example, for the [Raspberry Pi boards][qemu_raspi] or the [ARM Realview Boards][qemu_realview].

Check the full [list of command line arguments][qemu_args]. The most important ones are:

* `-machine|-M <board>`: Select which board to use. Consult board list with `qemu-system-arm -machine help`.

* `-m <mem_size><M|G>`: Set startup RAM size in Megabytes `M` or Gigabytes `G`.

* `-nographic`: Disable graphical output, redirecting the emulated serial port to the console.

* `-serial <dev>`: Redirects the virtual serial port to a host's character device `<dev>`. By default it goes to `stdio`. Some options are:

    * `stdio`: Standard input/output.
    * `/dev/XXX`: Connect to host tty, e.g. `/dev/ttyS0`.
    * `file:<filename>`: Write output to filename. Can't read input.
    * `tcp:[host_ip]:<host_port>[,server[,nowait]]`: Tries to connect to a TCP socket as client, aborts execution if no connection is established. If `server` is included, then QEMU will act as the server, and will halt until a connection from a client is established. If `server,nowait` is included, the server is created and the simulation proceeds, even without connections.
    * `telnet:[host_ip]:<host_port>[,server,[nowait]]` Same as tcp, but with the telnet protocol.
    * `null`: Redirect to `/dev/null`.

* `-monitor <dev>`: Redirect the QEMU monitor to host's device `<dev>`. Same options as with "serial". The QEMU's accepts command for controlling the simulation.

* `-S`: Do not start the CPU (you must type "c" in the monitor)

* `-S -gdb <dev>`: Accepts a gdb connection on device "dev". To halt execution until a gdb connection is established, use `-S` as well. Execution will start after the first `(gdb) c` command.

* `-no-reboot`: Exit instead of rebooting.

To [boot a kernel image][qemu_linux_boot], use the following flags:

* `-kernel <bzImage>`: Use bzImage as kernel image.
* `-append <cmdline>`: Use cmdline as kernel command line.
* `-initrd <file>`: Use file as initial ram disk.
* `-dtb <file>`: Use file as a device tree binary (dtb) image and pass it to the kernel on boot.

To [boot from a bare-metal elf file][qemu_generic_loader], use the following flag:

* `-device loader,file=<elf_file>`: Boots from an elf file. The LMA is used to determine where the binaries are loaded, and the linker's entrypoint marks where the code starts.

## Why QEMU doesn't support custom boards

As quoted from [QEMU's documentation][qemu_choose_board] "you must specify which board model you want to use with `-M` or `--machine`; there is no default"."check whether QEMU lists that machine [...]. If it is listed, then you can probably use that board model. If it is not listed, then unfortunately your image will almost certainly not boot on QEMU."

Therefore, the documentation is clearly stating that if your code can't run on any of the predefined board models, then it won't run on QEMU. The documentation does not provide any links to a section about modeling your own boards so, although technically possible, it is not encouraged.

<!--External links-->
[qemu]: https://www.qemu.org/
[guest_archs]: https://www.qemu.org/docs/master/about/emulation.html#emulation
[qemu_targets]: https://www.qemu.org/docs/master/system/targets.html
[qemu_choose_board]: https://www.qemu.org/docs/master/system/target-arm.html#choosing-a-board-model
[qemu_raspi]: https://www.qemu.org/docs/master/system/arm/raspi.html
[qemu_realview]: https://www.qemu.org/docs/master/system/arm/realview.html
[qemu_args]: https://www.qemu.org/docs/master/system/invocation.html#
[qemu_generic_loader]: https://www.qemu.org/docs/master/system/generic-loader.html
[qemu_linux_boot]: https://www.qemu.org/docs/master/system/linuxboot.html

<!--Internal links-->
[renode_section]: /docs/simulators/renode
[qemu_custom_board_support_section]: #why-qemu-doesnt-support-custom-boards
