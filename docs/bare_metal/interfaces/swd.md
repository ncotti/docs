# SWD: Serial Wire Debug

The SWD interface forms part of the [ARM Debug Interface Architecture Specification (ADIv5)][arm_adi]. SWD just represents the actual physical connection to the whole ADI architecture, which can also be accessed with a JTAG interface.

Everything that can be done with a JTAG can be done via SWD provided that the target ARM core supports it.

The application note [AN11553][an11553] from NXP is a great introduction to the whole architecture.

ISP (In-System Programming) over serial connections.

IAP (In-Application Programming)

<!-- External links -->

[an11553]: https://community.nxp.com/pwmxy87654/attachments/pwmxy87654/lpc/55224/1/SWD%20Programming%20AN11553.pdf

[arm_adi]: https://documentation-service.arm.com/static/622222b2e6f58973271ebc21?token=
