# Cross Compiling

[Bootlin's Embedded Linux Training][bootlin_ppt]

Cross compilation is one of the four main tools that any embedded Linux developer needs.

![alt text](image.png)

![alt text](image-1.png)

The [Binutils][docs_binutils] is a set of tools to generate and manipulate binaries (usually with the ELF format) for a given CPU architecture. They include tools such as the assembler `as`, linker `ld`, `objcopy`, `objdump`, etc.

A toolchain is identified by a **tuple** like:

```txt
<arch>-<OS>-<ABI>
<arch>-<vendor>-<OS>-<ABI>
```

## Getting the toolchain

There are three approaches to getting a cross-compilation toolchain:

1. Getting a pre-compiled toolchain, such as the [Bootlin's toolchains][bootlin_toolchains] or [Linaro's toolchains][linaro_toolchains]. This is the simplest and most convenient solution, but you can't fine tune the toolchain to your needs.

2. Building the toolchain as a part of a build system, such as [Buildroot][buildroot] or [Yocto Project][yocto]. This approach has its own section.

3. Using [Crosstool-NG][crosstool]

## Crosstool-NG

[Crosstool-NG][crosstool] aims at building toolchains. Nothing more, nothing less. It is quite straightforward to install and use, provided that you know what you need from your toolchain.

I suggest reading the documentation, but here is a TL,DR:

### Installation

```bash
VERSION="1.28.0"
wget "http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-${VERSION}.tar.bz2"
tar -xf "crosstool-ng-${VERSION}.tar.bz2"
cd "crosstool-ng-${VERSION}"

sudo apt update && sudo apt install -y \
    gcc g++ gperf bison flex texinfo help2man make libncurses-dev \
    python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 \
    xz-utils unzip patch libstdc++6 rsync git meson ninja-build
./configure --enable-local
make
cat ./bash-completion/ct-ng >> "${HOME}/.bashrc"
```

### First step configuration

Instead of starting from scratch, the best way to start is to check a similar toolchain, and adapt it to your needs:

```bash
./ct-ng list-samples
```

```bash
./ct-ng show-<tuple>

./ct-ng show-arm-none-eabi
[L...]   arm-none-eabi
    Languages       : C,C++
    OS              : bare-metal
    Binutils        : binutils-2.45
    Compiler        : gcc-15.2.0
    Linkers         :
    C library       : newlib-4.5.0.20241231 picolibc-1.8.10
    Debug tools     :
    Companion libs  : gmp-6.3.0 isl-0.27 mpc-1.3.1 mpfr-4.2.2 newlib-nano-4.5.0.20241231 newlib-nano-4.5.0.20241231 zlib-1.3.1 zstd-1.5.7
    Companion tools :

./ct-ng show-mips-unknown-linux-gnu
[L...]   mips-unknown-linux-gnu
    Languages       : C,C++
    OS              : linux-6.16
    Binutils        : binutils-2.45
    Compiler        : gcc-15.2.0
    Linkers         :
    C library       : glibc-2.42
    Debug tools     : duma-2_5_21 gdb-16.3 strace-6.16
    Companion libs  : expat-2.7.1 gettext-0.26 gmp-6.3.0 isl-0.27 libiconv-1.18 mpc-1.3.1 mpfr-4.2.2 ncurses-6.5 zlib-1.3.1 zstd-1.5.7
    Companion tools :
```

Once a sample was decided as a base, run:

```bash
./ct-ng <tuple>
```

Then you can start making configurations.

### Second step configuration

## Some examples

### Realview pb a8

realview-pb-a8       ARM RealView Platform Baseboard for Cortex-A8

[RealView Platform Baseboard for Cortex-A8][https://developer.arm.com/documentation/dui0417/d]

When looking at the memory map of the device, we find that the address `0x7000_0000-0x7FFF_FFFF` is an SDRAM, that is also a mirror of the `0x0000_0000-0xFFFF_FFFF`, so writing to any of these two is equivalent.

Because we are dealing with a Cortex-A8, the architecture expects to see a vector table at address `0x0000_0000` like this:

```asm
.section .vector_table, "ax", %progbits
prv_vector_table:
    ldr pc, =RESET_handler
    ldr pc, =UND_handler
    ldr pc, =SVC_handler
    ldr pc, =PREF_handler
    ldr pc, =ABT_handler
    ldr pc, =0x00
    ldr pc, =IRQ_handler
    ldr pc, =FIQ_handler

/******************************************************************************
* Exception handlers
******************************************************************************/
/// @brief Reset handler. First instruction executed on power up.
.global RESET_handler
.section .init
.type RESET_handler, %function
RESET_handler:
    b _start
```

TODO

El problema acá es que no quiero escribir una aplicación baremetal en assembler, quiero probar mi toolchain con algo en C. Necesito un bootloader sencillito.

<!--External links -->
[bootlin_ppt]: https://bootlin.com/training/embedded-linux/
[bootlin_toolchains]: https://toolchains.bootlin.com/
[linaro_toolchains]: https://www.linaro.org/downloads
[yocto]: https://www.yoctoproject.org/
[buildroot]: https://buildroot.org
[crosstool]: https://crosstool-ng.github.io/

[realview_user_manual]

<!--Internal links-->
[docs_binutils]: /docs/arm_assembly/binutils.md
