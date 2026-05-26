# Udev: Dynamic device management

[Udev][udev_arch_wiki] is a user space program that enforces **rules** on how to handle the connection of new devices.

This document is divided in two parts:

* The events that can trigger an udev-rule.
* The actions that can be taken in response to those events.

## Udev events: device identification

When a new device is connected to the computer, the first thing to be done is to identify what kind of device is. All devices' attributes are stored in a sub-directory inside `/sys/`, which is the [linux sysfs][sysfs] filesystem.

There are several ways to associate a path in `/sys/` with a device. The first approach is to check the kernel logs with `dmesg`. The following output was obtained after disconnecting and connecting an USB mouse, the `sysfs` path is the one starting from `/devices/...`:

```bash hl_lines="8"
$ sudo dmesg
[21383.608789] usb 1-1: USB disconnect, device number 6
[21388.767865] usb 1-1: new low-speed USB device number 7 using xhci_hcd
[21388.897705] usb 1-1: New USB device found, idVendor=1c4f, idProduct=0048, bcdDevice= 1.10
[21388.897711] usb 1-1: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[21388.897713] usb 1-1: Product: Usb Mouse
[21388.897715] usb 1-1: Manufacturer: SIGMACHIP
[21388.900928] input: SIGMACHIP Usb Mouse as /devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/0003:1C4F:0048.0003/input/input13
[21388.901030] hid-generic 0003:1C4F:0048.0003: input,hidraw0: USB HID v1.10 Mouse [SIGMACHIP Usb Mouse] on usb-0000:00:14.0-1/input0
```

Another way to get the `sysfs` path is by using the `udevadm monitor` command, which will listen to kernel events:

```bash
$ udevadm monitor
KERNEL[22994.196016] add      /devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/0003:1C4F:0048.0013/input/input29/mouse0 (input)
```

With the device's path in the sysfs, you can get all its info and its parents' info with the `udevadm info` command:

```bash
$ udevadm info -ap /devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/0003:1C4F:0048.0013/input/input29/mouse0
  [...]
  looking at device '/devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/0003:1C4F:0048.0013/input/input29/mouse0':
    KERNEL=="mouse0"
    SUBSYSTEM=="input"
    DRIVER==""
  [...]
  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-1':
    KERNELS=="1-1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{idProduct}=="0048"
    ATTRS{idVendor}=="1c4f"
    ATTRS{manufacturer}=="SIGMACHIP"
    ATTRS{product}=="Usb Mouse"
```

By convention, the device's attributes are singular `ATTR{}`, while their parents's are plural `ATTRS{}`.

The same information can be retrieved by using the device node name in the `/dev` directory, with:

```bash
udevadm info -an /input/mouse0
```

## Udev-rules: actions on devices

All udev-rules consist of two parts:

1. **The "match" part**: The conditions for the rule to be executed, i.e., a list of attributes that the device and/or its parents must have to be recognized as the target device.
2. **The "action" part**: The actions to be performed when the conditions are met.

The udev-rule syntax is a series of comma-separated keys with an operator and a value. Each newline in a file corresponds to a new udev-rule.

```udev
<KEY><OPERATOR><VALUE>, <KEY><OPERATOR><VALUE>
```

The following is an example udev-rule that detects the connection of the USB-mouse from the previous section and then creates a symbolic link in `/dev/mymouse`, and a log file in `tmp/udev_log.txt`.

```udev
ACTION=="add", \
KERNEL=="mouse*" \
SUBSYSTEMS=="usb", \
ATTRS{idProduct}=="0048", \
ATTRS{idVendor}=="1c4f", \
SYMLINK+="mymouse", \
RUN+="/bin/bash -c 'echo Mouse_connected >> /tmp/udev_log.txt'"
```

The full list of keys and operators can be found in the [Udev man page][udev]. The following is a summary of the most important keys:

| Key | Description |
| :---: | :-----------: |
| ACTION | Match the name of the event action. There are only two meaningful types: `add/remove` when the device node in `/dev` is created/removed, and `bind/unbind` when the driver is attached/detached. |
| KERNEL[S] | Match the name of the event device [or its parent's] |
| SUBSYSTEM[S] | Match the subsystem of the event device [or its parent's] |
| DRIVER[S] | Match the driver event of the event device [or its parent's] |
| ATTR[S]{filename} | Match sysfs attribute value of the event device [or its parent's]. "filename" is an actual file located at sysfs, the most common ones being `ATTRS{idVendor}` and `ATTRS{idProduct}`. |
| OWNER, GROUP, MODE | Set permissions for the device node. |
| RUN | Execute a program. It must be a named script with arguments. |
| SYMLINK | Creates a symbolic link in `/dev/`. This will only work if the device already creates a device node in `/dev/`, for example `/dev/ttyUSB0`. |

Any of the typical assignment (`=`, `+=`, `-=`) or comparison (`==`, `!=`) operators can be between keys and values. Values should always be quoted strings.

A very common udev-rule is the one that changes the permissions of a pluggable device so that the currently logged-in user can access it without root privileges:

```txt
# Original FT2232 VID:PID. Allows access to user without root privileges.
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010", MODE="660", GROUP="plugdev", TAG+="uaccess"
```

### Adding new rules

User-written rules are stored at `/etc/udev/rules.d`, while system-written at `/usr/lib/udev/rules.d`. Udev-rules filenames start with a number, such as `66-my-udev.rules`, and are processed in lexical order, meaning that the lower the number, the higher the priority.

Once the new udev-rule file was added at `/etc/udev/rules.d`, it will be automatically processed after each restart. However, you may reload Udev to detect new rules with:

```bash
sudo udevadm control --reload
```

<!-- Also, you may simulate the events generated by a device by using `udevadm test`. You can see that the "run" commands are prepared to execute, but they won't actually be executed.

```bash
udevadm test <device_path>
``` -->

## References

For reference, you may consult the [Arch Linux Wiki][udev_arch_wiki], the man pages for [udev][udev] and [udevadm][udevadm], or this [tutorial][udev_tuto].

<!-- External links -->
[udev]: https://man7.org/linux/man-pages/man7/udev.7.html
[udevadm]: https://man7.org/linux/man-pages/man8/udevadm.8.html
[udev_tuto]: https://linuxconfig.org/tutorial-on-how-to-write-basic-udev-rules-in-linux
[udev_arch_wiki]: https://wiki.archlinux.org/title/Udev
[sysfs]: https://man7.org/linux/man-pages/man5/sysfs.5.html
