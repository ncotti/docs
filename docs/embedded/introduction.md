# The embedded Linux ecosystem

The development of an embedded Linux system requires four basic components:

1. A [cross-compilation toolchain][toolchain_section]: Compiler that runs on the development machine, but generates code for the target.
2. A [bootloader][bootloader_section]: started by the hardware, responsible for basic initialization, loading and executing the kernel.
3. The [Linux kernel][linux_kernel_section]: contains the process and memory management, network stack, device drivers and provides services to user space applications.
4. The target application.

![Architectural diagram of an embedded Linux system](images/embedded_linux_architectural_diagram.png)

First, we will review each component separately. Then, we will take a look a build systems like Buildroot and Yocto.

For reference, the [Bootlin's Embedded Linux Training][bootlin_ppt] is a great place to get a general idea of the workflow.

TODO: link to buildroot and yocto.

<!--External links-->
[bootlin_ppt]: https://bootlin.com/training/embedded-linux/

<!--Internal links-->
[toolchain_section]: /docs/embedded_linux/toolchain
[bootloader_section]: /docs/embedded_linux/bootloader
[linux_kernel_section]: /docs/embedded_linux/linux_kernel
