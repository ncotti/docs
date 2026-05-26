# FTDI 4232H

When you connect the probe:

```bash
$ sudo dmesg | tail -n 17
[34744.298835] usb 1-2: new high-speed USB device number 10 using xhci_hcd
[34744.425877] usb 1-2: New USB device found, idVendor=0403, idProduct=6011, bcdDevice= 8.00
[34744.425897] usb 1-2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[34744.425903] usb 1-2: Product: Quad RS232-HS
[34744.425909] usb 1-2: Manufacturer: FTDI
[34744.428196] ftdi_sio 1-2:1.0: FTDI USB Serial Device converter detected
[34744.428274] usb 1-2: Detected FT4232H
[34744.428851] usb 1-2: FTDI USB Serial Device converter now attached to ttyUSB0
[34744.429263] ftdi_sio 1-2:1.1: FTDI USB Serial Device converter detected
[34744.429298] usb 1-2: Detected FT4232H
[34744.429963] usb 1-2: FTDI USB Serial Device converter now attached to ttyUSB1
[34744.430403] ftdi_sio 1-2:1.2: FTDI USB Serial Device converter detected
[34744.430441] usb 1-2: Detected FT4232H
[34744.431088] usb 1-2: FTDI USB Serial Device converter now attached to ttyUSB2
[34744.431516] ftdi_sio 1-2:1.3: FTDI USB Serial Device converter detected
[34744.431554] usb 1-2: Detected FT4232H
[34744.432212] usb 1-2: FTDI USB Serial Device converter now attached to ttyUSB3
```

```bash
udevadm monitor
KERNEL[34943.056606] add      /devices/pci0000:00/0000:00:08.1/0000:c3:00.3/usb1/1-2/1-2:1.0/ttyUSB0 (usb-serial)
KERNEL[34943.058040] add      /devices/pci0000:00/0000:00:08.1/0000:c3:00.3/usb1/1-2/1-2:1.1/ttyUSB1 (usb-serial)
KERNEL[34943.059457] add      /devices/pci0000:00/0000:00:08.1/0000:c3:00.3/usb1/1-2/1-2:1.2/ttyUSB2 (usb-serial)
KERNEL[34943.060840] add      /devices/pci0000:00/0000:00:08.1/0000:c3:00.3/usb1/1-2/1-2:1.3/ttyUSB3 (usb-serial)
```

```bash
udevadm info -an /ttyUSB0
```

```txt
# Number of channel in the board, not related to the number of "ttyUSB0".
ATTRS{bInterfaceNumber}=="00"
ATTRS{idProduct}=="6011"
ATTRS{idVendor}=="0403"
ATTRS{manufacturer}=="FTDI"
ATTRS{interface}=="Quad RS232-HS"
```

## EEPROM

```bash
sudo apt install ftdi-eeprom libftdi1

sudo echo "blacklist ftdi_sio" > /etc/modprobe.d/blacklist-ftdi.conf
sudo modprobe -r ftdi_sio

ftdi_eeprom --flash-eeprom eeprom.cfg
```

<http://developer.intra2net.com/git/?p=libftdi;a=blob;f=ftdi_eeprom/example.conf;h=545703e77bb0edbe65f09368652d349396b23c32;hb=HEAD>

## Drivers: libftdi

There are two drivers for FTDI devices:

* The official [D2XX][d2xx]  driversfrom FTDI themselves. They are proprietary, distributed as a compiled library and bypass the standard Linux drivers, accessing the USB device via what they call a "DLL interface".
* The [libftdi][libftdi] drivers. This is an open-source library, installable via *apt* packages, that implements basic functions to communicate with FTDI chips. The only caveat is its lack of documentation and robustness compared to D2XX.

Because of the open-source nature, the remainder of this guide will focus solely on [libftdi][libftdi]. To avoid annoying mans-in-the-middle, we will be using the C library directly and not the Python wrappers such as [pylibftdi][pylibftdi] or [PyFtdi][pyftdi].

```bash
sudo apt install libftdi1-dev
```

##

<!-- External links -->
[d2xx]: https://ftdichip.com/drivers/
[libftdi]: https://www.intra2net.com/en/developer/libftdi/
[pylibftdi]: https://pypi.org/project/pylibftdi/
[pyftdi]: https://eblot.github.io/pyftdi/
