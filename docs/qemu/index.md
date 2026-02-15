# QEMU

The **Q**uick **EMU**lator ([QEMU][qemu]) performs system emulation, i.e. allows you to run a completely different CPU architecture (such as ARM, MIPS, RISC-V, AVR, etc) with a different OS than those of the host machine.

Check the [list of supported guest architectures][guest_archs].

QEMU is the most suitable for running pre-configured boards, not custom hardware or boards. I will comment on this briefly but first, let's take a look on how to run a simple simulation for bare-metal and embedded Linux.

For each type of machine architecture, e.g ARM, RISC-V, AVR, etc; QEMU has its own [QEMU System Emulator Target][qemu_targets] or binary. For example, `qemu-system-arm`, `qemu-system-avr` or `qemu-system-riscv32`.

After choosing the architecture, the next step is to [choose the board model][qemu_choose_board]. Let's look at the example of ARM, since most architectures behave similarly, and we will see why QEMU does not support custom boards.

As quoted from the documentation "you must specify which board model you want to use with `-M` or `--machine`; there is no default". And then, it says: "check whether QEMU lists that machine [...]. If it is listed, then you can probably use that board model. If it is not listed, then unfortunately your image will almost certainly not boot on QEMU."

Therefore, the documentation is clearly stating that if your code can't run on any of the predefined board models, then it won't run on QEMU. The documentation does not provide any links to a section about modeling your own boards.

Each board has a set of predefined devices simulated, as can be seen, for example, for the [Raspberry Pi boards][qemu_raspi] or the [ARM Realview Boards][qemu_realview].

## Some example runs

The [list of command line arguments][qemu_args].

The most important ones are:

* `-machine|-M <board>`: Select which board to use. Consult list with `qemu-system-arm -machine help`.

* `-m <mem_size><M|G>`: Set startup RAM size in Megabytes `M` or Gigabytes `G`.

* `-nographic`: Disable graphical output, redirecting the emulated serial port to the console.

* `-serial <dev>`: Redirects the virtual serial port to host character device `dev`. By default it goes to `stdio`. Some options are:

  * `stdio`: Standard input/output.
  * `file:<filename>`: Write output to filename. No character can be read.
  * `tcp:<host_ip><host_port>[,server=on]`: Connect to a TCP socket. By default, it sends data to the specified IP and port, unless `server=on` is specified, where QEMU will halt until a connection is established from a client.
  * `telnet:<host_ip><host_port>[,server=on]` Same as tcp, but with the telnet protocol.
  * `null`: Equivalent to redirecting to `/dev/null`.

* `-monitor <dev>`: Redirect the QEMU monitor to host device "dev". Same options as with "serial". More on the QEMU monitor later.

* `-S`: Do not start CPU at startup (you must type "c" in the monitor)

* `-S -gdb <dev>`: Accepts a gdb connection on device "dev". To halt execution until a gdb connection is established, use `-S` as well.

* `-no-reboot`: Exit instead of rebooting.

* `-kernel <bzImage>`. Use bzImage as kernel image.

* `-append <cmdline>`: Use cmdline as kernel command line.

* `-initrd <file>`: Use file as initial ram disk.

* `-dtb <file>`: Use file as a device tree binary (dtb) image and pass it to the kernel on boot.

* `-device loader,addr=<addr>,data=<data>,data-len=<data_len>`: Load data in address.

TODO

<https://www.qemu.org/docs/master/system/generic-loader.html>

```bash
qemu-system-arm -machine realview-pb-a8
```

```bash
coproc qemu-system-arm \
    -M realview-pb-a8 \
    -m 32M \
    -no-reboot -nographic \
    -monitor telnet:127.0.0.1:1234,server,nowait \
    -S -gdb tcp::2159 \
    -kernel ${bin_file} &>/dev/null
```

<!--External links-->
[qemu]: https://www.qemu.org/
[guest_archs]: https://www.qemu.org/docs/master/about/emulation.html#emulation
[qemu_targets]: https://www.qemu.org/docs/master/system/targets.html
[qemu_choose_board]: https://www.qemu.org/docs/master/system/target-arm.html#choosing-a-board-model
[qemu_raspi]: https://www.qemu.org/docs/master/system/arm/raspi.html
[qemu_realview]: https://www.qemu.org/docs/master/system/arm/realview.html
[qemu_args]: https://www.qemu.org/docs/master/system/invocation.html#
