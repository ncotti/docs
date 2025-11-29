# Udev: Dynamic device management

[Udev][udev_arch_wiki] rules are a response from user space to new devices created by the kernel space.

There are two locations where Udev rules can be found:

* For user-written rules: `/etc/udev/rules.d`.
* For system-written rules: `/usr/lib/udev/rules.d`

This document is divided in two parts. First, we will define the possible events that can trigger an Udev rule. Then, the actions that can be taken and how to write the actual rules.

For reference, you may consult the [Arch Linux Wiki][udev_arch_wiki], the man pages for [udev][udev] and [udevadm][udevadm], or this [tutorial][udev_tuto].

## Udev events: Finding your device

To act upon a device, we first need to know its attributes, like its name, serial number, type, etc. All the properties of a device are stored in a directory inside the [linux sysfs][sysfs] at `/sys/`.

There are several ways to find this directory. The first approach is to check the kernel logs. The following output was obtained after disconnecting and connecting an USB mouse:

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

Another way to get the sysfs path is by using the `udevadm monitor` command, which will listen to kernel events:

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

## Udev rules: actions on devices

All Udev rules consist of two parts:

1. The "match" part: defines the conditions for the rule to be applied. These conditions are usually a list of attributes that the device and its parents must have to be recognized as the target device.
2. The "action" part: defines the actions to be performed when the conditions are met.

The syntax for the rule is a series of comma-separated keys with an operator and a value:

```udev
<KEY><OPERATOR><VALUE>, <KEY><OPERATOR><VALUE>
```

The following is an example Udev rule that detects when the USB mouse gets connected and creates a symbolic link in `/dev/mymouse`, and a log file in `tmp/udev_log.txt`.

```udev
ACTION=="add", \
KERNEL=="mouse*" \
SUBSYSTEMS=="usb", \
ATTRS{idProduct}=="0048", \
ATTRS{idVendor}=="1c4f", \
SYMLINK+="mymouse", \
RUN+="/bin/bash -c 'echo Mouse_connected >> /tmp/udev_log.txt'"
```

The operator can be any of the typical assignment (`=`, `+=`, `-=`) or comparison (`==`, `!=`) ones.

The full list of keys and operators can be found in the [Udev man page][udev]. The following is a summary of the most important keys:

| Key | Description |
|:---:|:-----------:|
| ACTION | Match the name of the event action. There are only four meaningful types: `add/remove` when the device node in `/dev` is created/removed, and `bind/unbind`, when the driver is attached/detached. |
| KERNEL[S]| Match the name of the event device [or of its parents] |
| SUBSYSTEM[S] | Match the subsystem of the event device [or of its parents] |
| DRIVER[S] | Match the driver event of the event device [or of its parents] |
| ATTR[S]{filename} | Match sysf attribute value of the event device [or of its parents]. "filename" is an actual file located in the sysfs. |
| OWNER, GROUP, MODE | Set the permissions for the device node. |
| RUN | Execute a program. It must be a named script with arguments. |
| SYMLINK | Creates a symbolic link in `/dev/`. This will only work if the device already creates a device node in `/dev/`, for example `/dev/ttyUSB0`.|

### Executing the rule

The rules start with a number, such as `66-my-udev.rules`, and are processed in lexical order, meaning that the lower the number, the higher the priority.

Create a file inside the `/etc/udev/rules.d` directory and reload Udev to detect your new rule:

```bash
sudo udevadm control --reload
```

After that, you may simulate the events generated by a device by using `udevadm test`. You can see that the "run" commands are prepared to execute, but they won't actually be executed.

```bash
udevadm test <device_path>
```

<!-- External links -->
[udev]: https://man7.org/linux/man-pages/man7/udev.7.html
[udevadm]: https://man7.org/linux/man-pages/man8/udevadm.8.html
[udev_tuto]: https://linuxconfig.org/tutorial-on-how-to-write-basic-udev-rules-in-linux
[udev_arch_wiki]: https://wiki.archlinux.org/title/Udev
[sysfs]: https://man7.org/linux/man-pages/man5/sysfs.5.html
