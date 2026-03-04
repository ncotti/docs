# Beaglebone example

## Cross-toolchain

The most important parameters for the toolchain are:

* CPU architecture: `-mcpu=cortex-a8`.
* Hardware FPU: `-mfpu=neon`.
* NEON floating point ABI: `-mfloat-abi=hard`.
* C-library: `musl`.

## Bootloader

For building U-Boot, there is no need to modify the default configuration file. Just import the defconfig and build:

```bash
export CROSS_COMPILE=<cross_compilation_toolchain->
make am335x_evm_defconfig
make DEVICE_TREE=am335x-boneblack
```

It produces the SPL (called `MLO` because of a Texas Instrument requirement), and the full U-Boot image `u-boot.img`.

[AM335X Sitara datasheet][am335x_sitara_datasheet]
[AM335X Technical Reference Manual][am335x_trm]

[am335x_sitara_datasheet]: https://www.ti.com/document-viewer/AM3358/datasheet
[am335x_trm]: https://www.ti.com/lit/ug/spruh73p/spruh73p.pdf
