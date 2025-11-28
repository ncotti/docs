# STLINK

During this section I want to focus on the importance of the first communication with the board: How to flash your program into the board, how to debug it and how to communicate with a UART.

Most development boards come with an USB interface that iss plug-and-play, but let's review what's happening under the hood.

## SWD

## Arduino

Let's start with the simplest example: the [Arduino Nano][arduino_nano].

From the Datasheet[arduino_nano_datasheet] and the [Schematics][arduino_nano_schematic], we can see the following:

1. The TX/RX from the UART are connected to a FT232RL chip, and then to the USB bus.
2. It has an ATmega 328 Microcontroller.

![alt text](images/arduino_nano_block_diagram.png)

## STM32

## NXP

[Nucleo][nucleo]

[Manual][nucleo_user_manual]

<!-- External links -->
[nucleo]: https://www.st.com/en/evaluation-tools/nucleo-f401re.html
[nucleo_user_manual]: https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf
[arduino_nano]: https://docs.arduino.cc/hardware/nano/
[arduino_nano_schematic]: https://docs.arduino.cc/resources/schematics/A000005-schematics.pdf
[arduino_nano_datasheet]: https://docs.arduino.cc/resources/datasheets/A000005-datasheet.pdf
