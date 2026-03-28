# U-Boot commands

The key to using [U-Boot][uboot_org] is to know exactly what you want it to do. There are three main use cases:

1. **Hardware probing**: checking that a hardware peripheral exists and is working as intended.

2. **Memory manipulation**: Writing to and from different memory addresses and devices.

3. **OS booting**: choosing which OS to boot and from where.

The following is a list of the most relevant commands. The not-so complete list of commands can be found in the [U-Boot docs][uboot_shell_commands].

## Environment variables

U-Boot relies heavily on the use of the environment variables. Environment variables are loaded and modified in RAM, and can optionally be later persistently stored in non-volatile storage.

There are three basic instructions:

1. Set a variable's value:

    ```bash
    setenv <variable_name> <variable_value>
    env set <variable_name> <variable_value>
    ```

    ```bash
    editenv <variable_name>
    env edit <variable_name>
    ```

2. Print the variable's value:

    ```bash
    printenv [variable_name]
    env print [variable_name]
    ```

3. Store the environment. The environment files are stored in a file called `uboot.env` in a predefined device and partition defined at build-time in the nconfig menu:

    ```bash
    env save
    saveenv
    ```

Also, you can execute commands from a variable. Commands must be semicolon-separated `;`:

```bash
env run <var_name>
run <var_name>
```

## Memory

Since the SPL loads U-Boot to the DRAM, all memory operations are performed from or to the DRAM's memory space.

`[.b|.w|.l]` specifies the byte size (1-byte, 2-byte, 4-byte) of the operation being performed.

`bdinfo` prints information about the board. When listing the memory space, it will show two reserved sections at the end of the DRAM, that correspond the U-Boot binary and the heap space:

```bash
=> bdinfo
lmb_dump_all:
 memory.count = 0x1
 memory[0] [0x80000000-0x9fffffff], 0x20000000 bytes, flags: none
 reserved.count = 0x2
 reserved[0] [0x9cf1f000-0x9cf27fff], 0x9000 bytes, flags: no-notify no-overwrite
 reserved[1] [0x9cf28b10-0x9fffffff], 0x30d74f0 bytes, flags: no-overwrite
```

`cmp` compares two DRAM's memory sections for equality:

```bash
cmp[.b|.w|.l] <0x_addr1> <0x_addr2> <0x_count>
```

Copy contents of DRAM's memory addresses:

```bash
cp[.b|.w|.l] <0x_source_addr> <0x_dest_addr> <0x_count>
loadm <0x_source_addr> <0x_dest_addr> <0x_len_in_bytes>
```

Memory dump:

```bash
md[.b|.w|.l] <0x_address> <0x_lenght>
```

`mw` writes to memory the specified value. `mm` modifies the memory in an incremental way, starting from the base address. It will show you the current value, prompt you to wrtie the new value, until a [Ctrl-C] is issued.

```bash
mw[.b|.w|.l] <0x_address> <0x_value> [count]
mm[.b|.w|.l] <0x_address>
```

## Block devices

### MMC

MMC controls both eMMC and SD cards.

The partition number start from "1". Partition number "0" refers to the whole raw device.

Display information:

```bash
mmc rescan                      # Update list of mmc devices. Useful if an SD card disconnects or reconnects.
mmc list                        # Lists available devices
mmc dev <device_number> [part]  # Change "current" mmc device
mmc dev                         # Shows current mmc device and partition
mmc info                        # Current MMC device info (block size, speed, capacity, manufacturer, etc)
mmc part                        # List of device's partitions
```

Read and write raw blocks of the mmc:

```bash
mmc read  <0x_ram_address_to_store> <0x_block_sector> <0x_number_of_blocks>
mmc write <0x_ram_address_from_where_to_get_data> <0x_block_sector> <0x_number_of_blocks>
mmc erase <0x_block_sector> <0x_number_of_blocks>
```

## Raw flash devices

### NAND FLASH

TODO: `nand` command.

### SPI FLASH

TODO: `sf` (SPI flash)

### NOR FLASH

TODO

## Filesystem manipulation

The following commands allow you to interact with a filesystem in a block device, for reference:

* `<interface>`: Represents the type of device (e.g mmc, sata, usb).
* `<dev[:part]>`: Device number (0,1,etc) and partition number (1,2,etc).

See the type of filesystem the device has on the partition:

```bash
fstype <interface> <dev>:<part>
# E.g
fstype mmc 0:1
```

For each filesystem supported by the builded U-Boot (`<fs> == fat | ext4 | ext2`, etc), the following commands may be applied:

List files:

```bash
<fs>ls <interface> <dev>:<part>
```

Read/write files. The contents are stored to or written from the RAM:

```bash
<fs>load <interface> <dev>:<part> <ram_address> <filename>
<fs>write <interface> <dev>:<part> <ram_address> <filename> <bytes>
```

Check file contents as hex dump:

```bash
xxd <interface> <dev>:<part> <filename>
```

Some more filesystem operations:

```bash
[fs]mkdir <interface> <dev>:<part> <directory>
[fs]rm <interface> <dev>:<part> <filename>
[fs]mv <interface> <dev>:<part> <old_path> <new_path>
```

## Communicating with the PC

These commands read or write things from the host's PC to the board or vice-versa.

### Through USB interface

The `ums` (USB Mass Storage) allows to use U-Boot as a middle man between a block device in the target board and the host PC. It will create a device under `/dev` and mount the block device's partitions in the host PC, where they can be modified.

```bash
ums <usb_device> <interface> <dev[:part]>
# E.g
ums 0 mmc 1
```

This command is very useful for programming internal eMMC devices or NAND flashes using `cfdisk` and `mkfs.<fs>` commands.

As an example, to load U-Boot into an eMMC:

```bash
# Initiate the UMS server using the USB 0 and the MMC 1 (eMMC).
(u-boot) ums 0 mmc 1

# Erase any previous contents
sudo dd bs=1M count=1 if=/dev/zero of=/dev/sda

# Create an MBR partition table with a 64MB bootable fat filesystem
# and the remaining space ext4 for Linux
sudo cfdisk /dev/sda

# Devices "/dev/sda1" and "/dev/sda2" should have been created.
# Create filesystems for them
# See issue https://bootlin.com/blog/workaround-for-creating-bootable-fat-partition-for-beagle-bone-am335x-on-recent-distros/
sudo mkfs.fat -n UBOOT -a -F 32 /dev/sda1
sudo mkfs.ext4 -L LINUX /dev/sda2

# Copy U-boot SPL and U-Boot itself into the fat partition /dev/sda1
sudo mkdir /media/uboot
sudo mount -t vfat /dev/sda1 /media/uboot -o uid=$(id -u),gid=$(id -g),umask=$(umask)
cp <uboot_path>/MLO /media/uboot/MLO
cp <uboot_path>/u-boot.img /media/uboot/u-boot.img
sudo umount /media/uboot
sudo rmdir /media/uboot

# Restart the board and it should boot
```

The `dfu` ([Device Firmware Upgrade][uboot_dfu]) is an standard for upgrading a device's firmware at run time. Its main usage is to download or upload files.

First, you need to define the file names and locations in the variable `dfu_alt_info`. The contents of this variable depend on the type of device (mmc, ram, nand, etc). For example, to define two files `file1.txt` and `file2.txt` in the `fat` partition of an SD card:

```bash
# <filename> <part|fat|ext4> <dev_number> <partition>
setenv dfu_alt_info "file1.txt fat 0 1;file2.txt ext4 0 1"
```

Check that the device has been properly described with:

```bash
dfu <USB_controller> <interface> <dev_number> list
#E.g
dfu 0 mmc 0 list
```

Then, start the `dfu` backend.

```bash
dfu <USB_contoller> <interface> <dev_number>
```

From the host computer, connected through USB to the board, install `dfu-util` and check that you can see the device listed:

```bash
sudo apt install dfu-util
dfu-util -l
Found DFU: [0451:d022] ver=7ea1, devnum=49, cfg=1, intf=0, path="5-1.2.3.2", alt=1, name="file2.txt", serial="2151SBB35180"
Found DFU: [0451:d022] ver=7ea1, devnum=49, cfg=1, intf=0, path="5-1.2.3.2", alt=0, name="file1.txt", serial="2151SBB35180"
```

Finally, you can upload files to the board with (`<alt>` is either the `alt` number or the file name as it appears in the `dfu-util -l` command):

```bash
dfu-util -a <alt> -D <filename_in_host>
# E.g: a file called "file1.txt" will be created on the device, with the contents of the host's file "hello.txt"
dfu-util -a 0 -D hello.txt
```

And download files from the board with:

```bash
dfu-util -a <alt> -U <filename_in_host>
#E.g: the contents of the file "file2.txt" will be read from the device and stored in the host in the file "rx_hello.txt"
dfu-util -a 1 -U rx_hello.txt
```

### Through serial interface

Waits for a file to be loaded through serial connection using the [Kermit][kermit] (preferred), the [XMODEM][xmodem] or [YMODEM][ymodem] protocols:

```bash
loadb <addr> <baudrate>
loadx <addr> <baudrate>
loady <addr> <baudrate>
# E.g
loadb 0x80001000 115200
```

If you are using picocom for your serial interface, you need to specify the script to execute (`--send-cmd "gkermit -i -X -s"`) when the send command `[Ctrl-a][Ctrl-s]` is issued.

```bash
sudo apt install gkermit
picocom -b 115200 --send-cmd "gkermit -i -X -s" /dev/ttyUSB0
    (picocom) [C-a][C-s]
    (picocom) *** file: <filename_to_send>
```

### Through Ethernet

#### TFTP

The [Trivial File Transfer Protocol][tftp_wikiarch] (TFTP) is a minimalistic protocol for transferring files.

```bash
sudo apt install tftpd-hpa
```

TODO

tftpput

tftpboot

BOOTP protocol

TFTP protocol

## Linux booting

The only command that actually boots the system is `bootz`:

```bash
bootz <ram_address_to_zImage> - <ram_address_to_dtb>
```

This command required three things[^2]:

1. A compressed Linux kernel stored in RAM, in `zImage` format.
2. A device tree blob stored in RAM, in `.dtb` format.
3. The boot arguments, defined in the `bootargs` env. variable.

You can get the `zImage` and the device tree blob from any device in any way you want, but they must be copied into memory. The next commands are some common options:

```bash
# Get it from an mmc device with a FAT filesystem
fatload mmc 0:1 0x81000000 zImage

# Get it through a serial connection (sent from host with Kermit protocol)
loadb 0x81000000 115200

# Get it through tftp protocol
tftp 0x81000000 zImage
```

Regarding the `bootargs`, these are a list of whitespace-separated [kernel's command-line parameters][kernel_cmd_line_parameters], the most important being:

For specifying the root filesystem:

* `root=<dev>`: The device and partition from where the root filesystem (the one mounted at `/` is stored). E.g: `root=/dev/sdXY`, `root=/dev/mmcblkXpY`, `root=/dev/mtdblockX`, `/dev/nfs`; where "X" is the device and "Y" is the partition.

* `rootfstype=<ext4 | ubifs | squashfs | nfs>`: Root filesystem type. Normally it is autodetected.

* `rootwait`: Wait indefinitely for the root device to show up.

* `rootdelay=<seconds>`: Delay before mounting the root filesystem.

* `ro` or `rw`: Mount the root filesystem as read-only or read-write.

For console and logging:

* `console=ttyS<n>[,bbbbpnf]`: [Linux Serial Console][linux_serial_console]. From this console you will be able to see all `printk` kernel messages. Multiple consoles are possible. The options `bbbbpnf` refer to:
    * `bbbb`: baudrate.
    * `p`: parity, where `e` is even, `o` is odd, and `n` is none.
    * `n`: number of bits.
    * `f`: flow control, where `r` enables the `CTS/RTS` signals.

* `earlyprintk=serial,ttyS0,<baudrate>`: Enables very eraly kernel debugging output, even before the normal drivers are loaded.

* `loglevel=<level_number>`: Print all kernel messages with a loglevel smaller. `7=DEBUG`, `6=INFO`, `4=WARNING`, `0=EMERGENCY`.

* `ignore_loglevel`: Prints all log messages, regardless of log level.

For initialization:

* `init=<script>`: Program to run as PID 1. By default, it searches in order: `/sbin/init`, `/etc/init`, `/bin/init`, `/bin/sh`.

Ideally, you would store these commands in the `bootcmd` env. variable, so you can use the `boot` command, which are equivalent to execute `run bootcmd`, i.e., to execute the command stored in `bootcmd`.

```bash
setenv bootcmd "fatload mmc 0:1 0x81000000 zImage; fatload mmc 0:1 0x82000000 device_tree.dtb; bootz 0x81000000 - 0x82000000"
setenv bootargs "console=ttyS0,115200n8 root=/dev/mmcblk0p2 rootfstype=ext4 rw rootwait"
saveenv
boot
```

### Booting from nfs

Network File System (nfs) TODO

## Hardware probing

U-boot extracts most of the hardware information from the device tree (DTS). However, in the following sections no previous knowledge of it will be assumed.

### GPIO

Show GPIOs current status:

```bash
gpio status -a [bank] # Shows all gpios
gpio status <pin>

#E.g
gpio status -a
    Bank gpio@481ae000_:
    gpio@481ae000_0: input: 0 [ ]

gpio status gpio@481ae000_0
    gpio@481ae000_0: input: 0 [ ]
```

The general naming convention for the status is:

```bash
gpio status -a
    Bank <bank>:
    <pin>: <function>: <electrical_value> [x] <label>
```

To set as input, or write a value as output, use:

```bash
gpio input <pin>    # Set to input
gpio set <pin>      # Set to output and "1"
gpio clear <pin>    # Set to output and "0"
gpio toggle <pin>   # Toggle output value

#E.g Turning on BBB leds (GPIO1_21|22|23|24)
gpio set gpio@4804c000_21
gpio set gpio@4804c000_22
gpio set gpio@4804c000_23
gpio set gpio@4804c000_24
```

#### Button

Buttons are just a wrapper to the gpio driver.

TODO

```bash
button list
button <button_name>
setenv <var_name> $?    # Store button value in var
```

#### Leds

Leds are just a wrapper to the gpio driver.

```bash
```

### I2C buses

Display information about the I2C driver and connected devices:

```bash
i2c bus                 # Shows i2c buses from the DTS
i2c dev <bus_number>    # Set current i2c bus
i2c speed               # Get speed of i2c bus
i2c probe               # Show i2c bus addresses where chips are connected
```

Read from I2C device (memory dump). Here, `[.0|.1.|.2]` refer to the amount of bytes used for the registers' address in the I2C transaction.

```bash
i2c md <0x_i2c_bus_addr> <0x_target_address_offset>[.0|.1|.2] <bytes_to_read>
i2c read <0x_i2c_bus_addr> <0x_target_address_offset>[.0|.1|.2] <bytes_to_read> <ram_address_to_store>

# E.g: read from i2c device @50, starting from address 0x04, 8 bytes. In this example, the device is an EEPROM with 32KiB (2 bytes for addressing).
i2c md 0x50 0x04.2 8
```

Memory write `i2c mw` writes a single value to the I2C device, while memory modify `mm`[^1] reads the current value presents the option to modify it and immediately following ones until a Ctrl-C is issued:

```bash
i2c mw <0x_i2c_bus_addr> <0x_target_address_offset>[.0|.1|.2] <value_to_write>
i2c mm <0x_i2c_bus_addr> <0x_target_address_offset>[.0|.1|.2]
i2c write <ram_address_to_write_from> <0x_i2c_bus_addr> <0x_target_address_offset>[.0|.1|.2] <bytes_to_write>

# E.g
i2c mw 0x50 0x200.2 0xaa
```

### SPI buses

`sspi`

TODO

### EEPROM

The EEPROM commands are a wrapper on top of the I2C or the SPI drivers.

It only has functions for reading and writing, as it expects that the I2C or SPI buses had already been initialized.

For I2C EEPROM:

```bash
eeprom read <i2c/spi_bus_number> <i2c_bus_addr> <ram_address_to_store> <target_address_offset> <bytes_to_read>
eeprom write <i2c_bus_number> <i2c_bus_addr> <ram_address_to_write_from> <target_address_offset> <bytes_to_write>

# E.g Read from I2C bus 0@50, to the RAM address 0x80001000,
# the 8 bytes starting from address 0x200 of the I2C EEPROM.
eeprom read 0 0x50 0x80001000 0x200 8
```

mbr

part (partitions)

dfu

### Fastboot

TODO: seems like a protocol to communicate from/to U-boot through USB. Might be useful.

<!--Footnotes-->
[^1]: I personally ran into problems when trying to use `mm` because of a lack of "ACK".

[^2]: There is actually a fourth thing, the initial RAM disk `initrd` (a.k.a. initramfs), but I personally have never used them, so can't tell.

<!-- Internal links -->

<!-- External links -->
[uboot_org]: https://u-boot.org/
[uboot_shell_commands]: https://docs.u-boot.org/en/latest/usage/index.html#shell-commands

[kermit]: https://www.kermitproject.org/index.html
[xmodem]: https://en.wikipedia.org/wiki/XMODEM
[ymodem]: https://en.wikipedia.org/wiki/YMODEM

[kernel_cmd_line_parameters]: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html
[linux_serial_console]: https://docs.kernel.org/admin-guide/serial-console.html

[uboot_dfu]: https://docs.u-boot.org/en/latest/usage/dfu.html
[tftp_wikiarch]: https://wiki.archlinux.org/title/TFTP
