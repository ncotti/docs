# Bootloaders

[Das U-Boot][uboot_org].

Download Github. Checkout last release

```bash
git clone https://github.com/u-boot/u-boot.git
# release is v<year>.<month>
git checkout v2026.01
```

First we need to build U-Boot.

For that, look at the default configuration files for boards under the `configs/` directory. Find the most "similar" one and then run:

```bash
make <board_name>_defconfig
```

Dont' find your specific board? Try using the "qemu-*" ones.

For the realview board:

```bash
make qemu_arm_defconfig
```

Then, we are presented with the `make nconfig` GUI menu.

You need to build it with your cross-compiler.

<!-- External links -->
[uboot_org]: https://u-boot.org/
