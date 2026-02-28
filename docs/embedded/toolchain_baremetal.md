# Bare-metal toolchains

If you are writing a bare-metal application, then you are probably using a resource-constrained device with a Cortex-M or Cortex-R. In those scenarios, using a fully-fledged C library like [glibc][glibc], [musl][musl] or [uClibc-ng][uClibc] is prohibitive.

Therefore, there are three alternatives for the C-library: [Newlib][newlib], Newlib-nano or [Picolibc][picolibc].

It is recommended to choose Picolibc when possible. The story between these libraries goes like this:

1. [Newlib][newlib] is the original library. It bigger in size than the others.
2. Newlib-nano uses the same sources as Newlib, but optimized in size for embedded systems.
3. [Picolibc][picolibc] is a fork of Newlib, but further optimized and specifically designed for embedded.

As a summary, the recommended linker flags when compiling with a C library are as follows:

```txt
-Wl,-Map=<output_memory_map_file>
-nostartfiles
-specs=picolibc.specs
-Wl,--gc-sections
-T <linker_script>
```

In this repo there are some examples of bare-metal developments for the [Cortex-A8][cortex_a8_example] and the [Cortex-M4][cortex_m4_example] that were tested with these three libraries. Go check them out.

In the following sections we will be reviewing the linker flags from above and how to correctly setup a bare-metal project with a C library. But first, a comment about vendor IDE's and toolchains.

## The vendor IDE experience: why bother?

The vendor IDE the author has used the most is the [STM32Cube IDE][cube_ide], with development boards such as the [STM32F401 Nucleo Board][nucleo].

It's usage is as simple as writing your C code, maybe with the help of the HAL library, pressing the "run" button on the IDE, and then the code is compiled and flashed into the device.

The IDE already comes with an `arm-none-eabi` toolchain with Newlib and Newlib-nano, a default bootloader in an assembler file (`startup_stm32f401retx.s`) and a default linker script (`STM32F401RETX_FLASH.ld`).

Besides the point of "learning", there are only two disadvantages to using a vendor IDE:

1. You need to learn/have a new IDE for every manufacturer: SMT, NXP, TI, Nuvoton, etc.

2. The moment you deviate from the standard applications and board, you may run into problems.

Personally, I believe that it's just valuable to own your tools and understand what you are doing, because that will make problems easier to fix in the future.

## Compiler and linker flags for bare-metal development

### Selecting the hardware architecture

In Crosstool-NG's configuration menu, under "Target option --> Build a multilib toolchain", you are given the option to have support for all cores architectures or only a specific one. Choosing support for only one CPU and architecture reduces drastically the toolchain's build time.

Nonetheless, when compiling you should[^1] define the following flags so the compiler knows what it is compiling against [^2]:

* `-mcpu=<CPU>[+extension]`: Specifies the target's CPU processor, e.g `cortex-m4+nofp`. GCC uses the CPU to derive the architecture (`-march`) and the performance tuning (`-mtune`), so they are not strictly required.
    * `-march=<ARCH>[+extension]`: Specifies the target's CPU architecture, e.g. `armv7+fp`.
    * `-mtune=<CPU>`: Specifies the CPU for which to tune for performance, e.g `cortex-a8`.
* `-mfpu=<FPU>`: Specifies what floating-point hardware is available on the target, e.g. `neon` or `vfpv3`.
* `-mfloat-abi=<soft | softfp | hard>`: Specifies which floating-point ABI to use:
    * `soft` Causes GCC to generate output containing library calls for floating-point operations. You must link with a compatible set of libraries.
    * `softfp` Allows generation of code using hardware floating-point instructions, but still uses the soft-float calling instructions. You must link with a compatible set of libraries.
    * `hard`: Allows generation of floating-point instructions and uses FPU-specific calling conventions.
* `-marm | -mthumb`: Selects between generating code that executes in ARM or Thumb states by default. These declaration gets overwritten by using the "target("arm")" or "target("thumb")" attributes or pragmas.

### Selecting the bootloader

If you are writing a bare-metal application, you probably have your own linker script. Your command for linking should look something like this:

```bash
<toolchain_tuple>-gcc -T <linker_script> -o <output_elf_file> <list_of_object_files>
```

However, after executing that command, your linking stage will fail because of undefined references to linker sections (e.g `__bss_end__`) and specifically because of the inclusion of the `crt0.S` file [^3].

CRT stands for C Runtime, and it is a default bootloader provided by the C library. It initializes stack pointers, setups memory segments and then make a call to the `main()` function. You should explicitly exclude this file from being compiled by adding the flag:

```text
-nostartfiles
```

### Selecting the C library

After including a C library in your toolchain, you may find a `.specs` file in the output, somewhere in `<toolchain_tuple>/lib/`.

The `.specs` file sets the default configuration for the compiler. You can see the current default specs for your compiler with:

```bash
gcc -dumpspecs
```

When using C libraries, include their specs file to let the compiler know about their object files' and headers' paths, so when you include a header like `#include <stdio.h>`, it knows exactly where to look for. For example, these are the specs files for the Newlib, Newlib-nano and Picolibc libraries:

```bash
# No specs              # Newlib (if it was selected as the default C-library)
--specs=nano.specs      # Newlib-nano
--specs=picolibc.specs  # Picolibc
--specs=nosys.specs     # If the library has default syscalls, implement them as stubs.
```

## C-library implementation with syscalls

Since a bare-metal environment doesn't have an OS, you must define your own System calls (syscalls) to access the hardware resources.

For example, the `printf()` function does not care about whether the text gets written to a terminal in a monitor, an UART, a I2C bus, etc. It only wants to `write()` data, and the syscall defines how and where.

You can use a great subset of the C-library functions, provided that you implement the system calls for them.

### Newlib and Newlib-nano's syscalls

You can find the [full list of Newlib's syscalls][newlib] in chapter 2 of the Newlib's documentation, together with a minimal functionality template. The most relevant ones are `_write()`, `_read()`, for writing and reading data, and `_sbrk()` for using dynamic memory.

Each C-library function in the Newlib's documentation lists their required syscalls.

Check the [Cortex-A8 example][cortex_a8_example] that implements these syscalls.

### Picolibc syscalls

You can find the full [list of Picolibc's syscalls][picolib_syscalls] in the library's documentation.

Picolibc also requires the developer to provide definitions for `stdin`, `stdout` and `stderr`. Their declaration is done by creating a `my_putc()`, `my_getc()` and `my_flush()` functions for writing, reading and flushing a character respectively, and then calling the macro `FDEV_SETUP_STREAM`.

### Troubleshooting

1. If when including the Newlib-nano library with `--specs=nano.specs` you see a message like:

    ```bash
    fatal error: environment variable 'GCC_EXEC_PREFIX' not defined
    ```

    Then, you must move the toolchain to any other path than its default location.

2. If you see and error related to errno and TLS, that is because you are trying to execute Newlib-nano, when you have compiled Picolibc as the "main" C library. Newlib-nano should only be run with Newlib as the "main" library.

    ```bash
    errno: TLS reference in [...] mismatches non-TLS reference in [...]
    ```

3. If, when defining `stdout` for Picolibc, you see and error like:

    ```bash
    error: implicit declaration of function 'FDEV_SETUP_STREAM'
    ```

    This is because your toolchain was built for support for both Newlib and Picolibc. If you analyze the paths of the header files, you will notice that they don't point to the picolibc path, but rather the "sys-include" path, which means Newlib.

    This error is fixed by recompiling the toolchain without the Newlib library.

4. If you see either of these errors:

    ```bash
    dangerous relocation: unsupported relocation
    Unknown destination type (ARM/Thumb)
    ```

    That's because you compiled the library with "multilib" support. The compiler probably built the library with the "ARM" instruction set, and you are trying to link it in a device that only supports "Thumb" instructions. The solution is to rebuild the toolchain and specify the CPU architecture )(`-mcpu=XXX`) yourself.

<!--Footnotes-->

[^1]: If you chose only one architecture when building the toolchain, the compiler flags that define the CPU are not mandatory. However, if you built a multilib toolchain, then you **must** define them.

[^2]: All possible values for these flags can be found in the [GCC man's pages][gcc_man].

[^3]: The `crt0.S` or its variants like `crtbegin.S` or `crtend.S` are only present in Crosstool-NG's build folder, not in the generated toolchain's path. There, only the object file `crt0.o` can be found.

<!-- External links -->
[picolibc]: https://github.com/picolibc/picolibc
[newlib]: https://sourceware.org/newlib/
[glibc]: https://www.gnu.org/software/libc/
[musl]: https://musl.libc.org/
[uClibc]: https://uclibc-ng.org/
[picolib_syscalls]: https://github.com/picolibc/picolibc/blob/main/doc/os.md
[cube_ide]: https://www.st.com/en/development-tools/stm32cubeide.html
[nucleo]: https://www.st.com/en/evaluation-tools/nucleo-f401re.html
[gcc_man]: https://man7.org/linux/man-pages/man1/gcc.1.html

<!--Internal links-->
[cortex_a8_example]: https://github.com/ncotti/docs/tree/main/bootloaders/cortex-a8
[cortex_m4_example]: https://github.com/ncotti/docs/tree/main/bootloaders/cortex-m4
