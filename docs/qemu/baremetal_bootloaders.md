# Baremetal bootloaders

After doing a lot of research on the topic, it seems that there are only two alternatives when it comes to simple bare-metal designs:

1. Write your own bootloader from scratch.

2. Use a vendor IDE that already has the bootloader written.

I will shortly demonstrate this approach coming from different vendors, but the core of this article is about writing your own bootloaders, to avoid being shackled to a vendor IDE.

TL;DR: vendor IDEs are "too generic". If you know your micro or board well, then yuo should be able to customize everything to your needs. When something is too generic, it is prone to faults and bad optimizations.

## Case studies of vendor IDEs

### STM32CUBEIDE

Files generated when created a project are in the folder.

### Arduino IDE

The case of the Arduino bootloader is quite special.

When writing code from the Arduino IDE, the bootloader is expected to already be installed by default. The Arduino boards have a USB-to-Serial chip, and the bootloader receives the code through the UART, writes those bits to the FLASH memory, and then executes.

Arduino won't share a copy of its bootloader and Linker script, but we can find an alternative:

<https://github.com/Optiboot/optiboot/tree/master>

### PlatformIO

From PlatformIO

<https://github.com/platformio/platform-ststm32/tree/develop/examples/stm32cube-hal-blink>

After running the steps, we can find the linker script and the bootloader files. They have been copied to the folder.

Toolchain:

~/.platformio/packages/toolchain-gccarmnoneeabi

Linker:

/home/nicolas.cotti/.platformio/packages/tool-ldscripts-ststm32/stm32f4/STM32F401RETX_FLASH.ld

Bootloader:

/home/nicolas.cotti/.platformio/packages/framework-stm32cubef4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xe.S
