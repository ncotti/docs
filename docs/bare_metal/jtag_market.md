# Jtag market

This section is a market research on JTAG programmers and associated software tools.

A comparison table is provided below:

My two cents is that debuggers and probes are expensive because of ignorance. People pay for what they don't know.

## FT2232HL

TODO

<https://www.mercadolibre.com.ar/conversor-ft232h-usb-gpiospii2c-fifo-multifuncion/up/MLAU213998973#polycard_client=search-nordic&search_layout=grid&position=19&type=product&tracking_id=f1b605ec-717a-4ecc-bb76-8672dcfec577&wid=MLA1128744844&sid=search>

## CP2102

This is the one I have (check todo micro purchase).

## Black Magic Probe

The [Black Magic Probe (BMP)][black_magic_probe] goes for just $75 bucks. However, it

## ST-LINK

## SEGGER J-Link

Check: <https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/>

The old [J-Link][jlink_base] we all know and love. The J-Link BASE and the J-Link PLUS are the exact same hardware, the only difference being than the PLUS comes with extra software licenses, Ozone, the GUI debugger.

The BASE only gives you access to the [J-Link Commander][jlink_commander], a CLI tool that can only be used for flashing programs and debugging.

The hardware goes at $450, while the [Ozone software][ozone] has a free license for students and non commercial use, but for commercial goes at $1000.

With JLink Commander, you can:

* Connect to the target MCU via JTAG or SWD.
* Ran and write memory addresses.
* Load and program flash (.elf, .bin, .hex).
* Control execution with debugging instructions.
* Reset target.
* Get device information (detect core, flash size, etc).
* SWO/UART logging

JLink.exe -CommanderScript flash.jlink

For example, this jlink script programs the STM32F401:

```jlink
; Specify device and SWD interface speed
device STM32F401RE
if SWD
speed 4000

; Reset, halt and erase flash
r
h
erase

; Load binary into memory address
loadbin firmware.bin, 0x08000000
verifybin firmware.bin, 0x08000000

; Reset and run
r
go
exit
```

JLINK is primarly a flasher / debugger with a JTAG interface. For boundary-scan, you need to use external tools that talk to JLink.

The JTAG interface only specifies 5 pins, but the JLink device has a pinout of 20. Most of them are boilerplate GND and extra signals that aren't really needed.

![alt text](images/jlink_pinout.png)

## Lauterbach, XJTAG, JTAG Technologies

All these companies are really expensive. They are meant to be used for companies only, since the real value comes from the proprietary software and customer support.

Basically, by buying an using these products you are paying their engineers to solve your problems. You don't get any real experience or knowledge besides on how to use their products, since they purposefully abstract everything to make you reliant on their solutions.

I can only think of one specific use case for these devices, and that is if you absolutely a very high speed debugging interface with a large memory to store trace information.

<!-- External links -->

[jlink_base]: https://shop.segger.com/debug-trace-probes/debug-probes/j-link/j-link-base-classic
[jlink_commander]: https://www.segger.com/products/debug-probes/j-link/tools/j-link-commander/?mtm_campaign=kb&mtm_kwd=J-link-commander

[black_magic_probe]: https://black-magic.org/index.html
