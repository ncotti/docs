# Embedded Linux toolchain

Provided that your host's OS is Linux, the toolchain required for embedded Linux behaves quite similarly.

When making the toolchain, only two things need to be taken into account:

1. The hardware architecture.
2. The choice of the C-library.

For the hardware architecture, the same compiler flags explained in the [Selecting the hardware architecture section][hardware_arch_section] for bare-metal apply. Therefore, it is encouraged to define:

```bash
-mcpu=<CPU>
-mfpu=<FPU>
-mfloat-abi=<soft | softfp | hard>
```

Regarding the C-library, there are three choices:

* [Musl][musl] is the preferred choice. It is a lightweight implementation of the C-library, and reduces greatly the size of statically linked binaries.
* [Glibc][glibc] is the de-facto standard. Is the most complete but quite heavy C-library. It is more suitable for full desktops rather than embedded.
* [uClibc-ng][uclibc]. Never used it, since it is not supported by Yocto. Very similar to Musl in spirit.

The application code can be statically or dynamically linked to the C-library. Unless the libraries weren't going to be included in the target, it is suggested to dynamically link the binaries.

Dynamic libraries point to the usual `/lib` or `/usr/lib` paths. However, these should not be mistaken with the libraries found at the host. The actual location for them can be found in your toolchain's path at `x-tools/<tuple>/sysroot/`. You can see the libraries a binary depends on with:

```bash
<tuple>-ldd --root <path_to_sysroot> <executable>
```

To execute the application in the target, the dynamic libraries must be present in the expected `/lib` or `/usr/lib` paths.

<!--External links-->
[glibc]: https://www.gnu.org/software/libc/
[uclibc]: https://uclibc-ng.org/
[musl]: https://musl.libc.org/
[hardware_arch_section]: /docs/embedded/toolchain_baremetal/#selecting-the-hardware-architecture
