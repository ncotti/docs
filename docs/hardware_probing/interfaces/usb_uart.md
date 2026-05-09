# USB-UART interfaces: talk to me, board

The USB protocol is something that you need dedicated hardware for. If your chip doesn't have it, then you will need one of these chips for communicating from the board to the PC or vice versa.

In this section we will review the different alternatives to achieve data transfers from an embedded device to a computer, without directly using USB or Ethernet.

## Semihosting

There

## The Serial protocols

This serial protocols are meant to transmit UART signals outside of a PCB board, for distances greater than a meter.

Most serial protocols like UART, SPI, I2C use TTL levels, from 0V to either 1.8V, 3.3V or 5V. The main purpose of the standards shown below is to have greater voltage swings, between -15V and 15V, to defend against noise.

### RS-232

The original [RS-232][rs232] standard defines 9 pins, and was used with a DE-9 serial port. Nowadays, you only see either a 3-wire variation, using only Tx, RX and GND, or a 5-wire variation with RTS/CTS handshaking.

The meaning of the signals is as follows:

| Acronym | Signal | Direction | Functions |
|:-------:|:------:|:---------:|:---------:|
|TxD|Transmitted Data|Out|Carries data to other device.|
|RxD|Received Data|In|Carries data into the device.|
|GND|Common Ground|Common|Zero voltage reference.|
|RTS/RTR|Request To Send / Ready To Receive|Out|The device can receive data from the RxD pin. If RTS is "0", all data received through the "RxD" pin won't be read. |
|CTS|Clear To Send|In|Data can be sent to the other device through the TxD pin. If CTS is "0", all data sen't through the TxD pin won't be read by the other device. |

```css
        DEVICE A                                   DEVICE B
┌──────────────────────┐                   ┌──────────────────────┐
│      UART / MCU      │                   │      UART / MCU      │
│                      │                   │                      │
│   TX ────────────────┼──────────────────►│ RX                   │
│                      │                   │                      │
│   RX ◄───────────────┼───────────────────┤ TX                   │
│                      │                   │                      │
│  RTS ────────────────┼──────────────────►│ CTS                  │
│      (I can receive) │                   │ (You may send to B)  │
│                      │                   │                      │
│  CTS ◄───────────────┼───────────────────┤ RTS                  │
│ (You may send to A)  │                   │  (I can receive)     │
└──────────────────────┘                   └──────────────────────┘
```

The RTS/RTR pin may be confusing for its name. It shares two functions:

* The RTS (Request To Send) is assumed to be always asserted, that is, a device is always ready to send data. If it isn't ready, then it just won't send new data.
* The RTR (Ready To Receive) is the one that is electrically controlled. When asserted, the device can receive data through the "RX" pin.

![DE-9 male serial port](images/serial_port.png)

### RS-422

The [RS-422][rs422] standard only defines RX and TX as differential pairs. It supports one transmitter and up to 10 receivers.

The cable must be though like a transmission line. A parallel termination resistor of 100 or 120 Ohms is required for signal integrity, which matches the characteristic impedance (Z0) of most twisted pair cables.

![RS-422 diagram](images/rs422.png)

### RS-423

The [RS-423][rs423] is very similar to the RS-422, but it is single ended with a GND signal. This standard is seldom used.

### RS-485

The [RS-485][rs485] is fully compatible with the RS-422. It also uses differential signaling, but allows for 10 transmitters and 32 receivers. Transmitters have three-state logic, and only one at the time can transmit to avoid bus contention.

| **Specifications** | **RS-232** | **RS-423** | **RS-422** | **RS-485**|
|--------------------|------------|------------|------------|-----------|
| **Mode of Operation**| Single-Ended | Single-Ended | Differential | Differential |
| **Total Drivers/Receivers on One Line**| 1 Driver / 1 Receiver | 1 Driver / 10 Receivers | 1 Driver / 10 Receivers | 1 Driver / 32 Receivers |
| **Maximum Cable Length**| 15 m | 1200 m | 1200 m | 1200 m |
| **Maximum Data Rate** | 460 kb/s | 100 kb/s | 10 Mb/s | 30 Mb/s |
| **Maximum Driver Output Voltage** | ±25 V | ±6 V | –0.25 V to +6 V | –7 V to +12 V |
| **Driver Output Level (Loaded, Min.)** | ±5 V to ±15 V | ±3.6 V | ±2.0 V | ±1.5 V |
| **Driver Output Level (Unloaded, Max.)** | ±25 V | ±6 V | ±6 V | ±6 V |
| **Driver Load Impedance (Ω)**  | 3k–7k | ≥450 | 100 | 54 |
| **Max Driver Current (High-Z, Power On)** | N/A | N/A | N/A | ±100 µA |
| **Max Driver Current (High-Z, Power Off)** | ±6 mA @ ±2 V | ±100 µA | ±100 µA | ±100 µA |
| **Slew Rate (Max)** | 30 V/µs | Adjustable | N/A | N/A |
| **Receiver Input Voltage Range** | ±15 V | ±12 V | –10 V to +10 V | –7 V to +12 V |
| **Receiver Input Sensitivity** | ±3 V | ±200 mV | ±200 mV | ±200 mV |
| **Receiver Input Resistance (Ω)** | 3k–7k | ≥4k | ≥4k | ≥12k |

![RS-XXX standards' data rates](images/rs_datarates.png)

Sources: [OPTCORE][optcore_table] and [RS-423][rs423].

## THe USB-Serial devices in Linux

/dev/ttyUSBx
/dev/ttyACMx

HID USB
Virtual COM USB

RS232

RS484

## Chip vendors

There are basically three main chip vendors for UART-USB interfaces. They are ordered from most expensive and reliable to cheapest.

The FTDI

|Vendor|Chip number|Price|Comment|
|:----:|:---------:|:---:|:-----:|
|FTDI|

## CP21XX: only UART-USB

[CP21XX][cp_modules] from Silicon Labs

This is the one I have (check todo micro purchase).

## CH340

## FTDI chips

<!-- External links -->
[rs232]: https://en.wikipedia.org/wiki/RS-232
[rs422]: https://en.wikipedia.org/wiki/RS-422
[rs423]: https://en.wikipedia.org/wiki/RS-423
[rs485]: https://en.wikipedia.org/wiki/RS-485
[optcore_table]: https://www.optcore.net/difference-between-rs-232-rs-422-and-rs-485/
