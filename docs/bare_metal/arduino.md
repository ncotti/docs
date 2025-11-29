# Arduino

Let's start with the simplest example: the [Arduino Nano][arduino_nano].

This example is very valuable because not only we will learn about the clone.

From the Datasheet[arduino_nano_datasheet] and the [Schematics][arduino_nano_schematic], we expect our chip to have the following:

Expected | actual
LM1117 | AMS1117
ATMega328P | ATMega328P
FT232RL | CP210x

![alt text](images/arduino_nano_block_diagram.png)

The UART-USB adapter's chip does not have any writing on top, is blank.

Programming the chip from the Arduino IDE is straight forward:

* Install the program.
* Use one of the example programs in File->Examples->01.Basics->Blink.
* Select the Arduino Nano in `/dev/ttyUSB0`.
* Configure Tools->Processor->"ATmega328P (Old Bootloader)".
* Press "Upload" button.

## Identifyng the USB-serial interface

Since we now that, when connecting the Arduino, a `/dev/ttyUSB0` is being created, let's get more info. Starting with `dmesg`:

```bash
$ sudo dmesg
[39612.052359] usb 1-3: new full-speed USB device number 30 using xhci_hcd
[39612.177275] usb 1-3: New USB device found, idVendor=1a86, idProduct=7523, bcdDevice= 2.54
[39612.177290] usb 1-3: New USB device strings: Mfr=0, Product=2, SerialNumber=0
[39612.177293] usb 1-3: Product: USB2.0-Serial
[39612.211320] usbcore: registered new interface driver ch341
[39612.211345] usbserial: USB Serial support registered for ch341-uart
[39612.211363] ch341 1-3:1.0: ch341-uart converter detected
[39612.212318] usb 1-3: ch341-uart converter now attached to ttyUSB0
```

The kernel logs already tells us that the chip used is a "ch341", but we lack device path.

```bash
$ udevadm info -an ttyUSB0
  looking at device '/devices/pci0000:00/0000:00:14.0/usb1/1-3/1-3:1.0/ttyUSB0/tty/ttyUSB0':
    KERNEL=="ttyUSB0"
    SUBSYSTEM=="tty"
    DRIVER==""

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-3/1-3:1.0/ttyUSB0':
    KERNELS=="ttyUSB0"
    SUBSYSTEMS=="usb-serial"
    DRIVERS=="ch341-uart"
    ATTRS{port_number}=="0"

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-3/1-3:1.0':
    KERNELS=="1-3:1.0"
    SUBSYSTEMS=="usb"
    DRIVERS=="ch341"

  looking at parent device '/devices/pci0000:00/0000:00:14.0/usb1/1-3':
    KERNELS=="1-3"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{idProduct}=="7523"
    ATTRS{idVendor}=="1a86"
    ATTRS{product}=="USB2.0-Serial"
    ATTRS{version}==" 1.10"
```

TODO check avrdude

<!-- External links  -->
[arduino_nano]: https://docs.arduino.cc/hardware/nano/
[arduino_nano_schematic]: https://docs.arduino.cc/resources/schematics/A000005-schematics.pdf
[arduino_nano_datasheet]: https://docs.arduino.cc/resources/datasheets/A000005-datasheet.pdf
