# Embedded Linux for the BeagleBone Black: Buildroot

This example uses [Buildroot][buildroot_section] to prepare an embedded Linux environment using the [BeagleBone Black board][bbb_board] (BBB), while including the [nInvaders][ninvaders] external package.

Install Buildroot:

```bash
VERSION="2026.02"
git clone https://gitlab.com/buildroot.org/buildroot.git
cd buildroot
git checkout ${VERSION}
```

We will cover two use cases: The one were you start from nothing, and the one where you are just importing and existing project (like this one).

## Starting from zero

Run menuconfig, but including this directory as an external dependency.

```bash
make BR2_EXTERNAL=<path> nconfig
```

Make the relevant changes for the BBB while pointing to the default configurations of each project, i.e.:

* ARM Cortex-A8 EABIhf VFPv3-D16 architecture.
* Bootlin's external toolchain with musl.
* Global patches, post-build script, post-install script paths with `$(BR2_EXTERNAL_COTTI_PATH)` variable.
* Linux kernel using `omap2plus` as defconfig and `ti/omap/am335x-boneblack` as device tree.
* U-Boot as bootloader, pointing to `am335x_evm` defconfig and `am335x-boneblack` device tree, and to `MLO` as the SPL.
* Include `External`

Run the build and save the log. As we already used `BR2_EXTERNAL=<path>`, the path has already been saved:

```bash
make 2>&1 | tee build.log
```

After the build is done, checkout the `.config` files generated for U-Boot, Linux and Busybox and copy them into your external package directory. Also, copy the device tree and device tree from the Linux project.

```bash
cp output/build/uboot-<version>/.config <BR2_EXTERNAL_COTTI_PATH>/board/cotti/cotti-board/uboot.config
cp output/build/busybox-<version>/.config <BR2_EXTERNAL_COTTI_PATH>/board/cotti/cotti-board/busybox.config
cp output/build/linux-<version>/.config <BR2_EXTERNAL_COTTI_PATH>/board/cotti/cotti-board/linux.config
```

Then, launch the menuconfig again, and point to these external configurations. Now, a new options enables us to change the full `.config` files for reduced `defconfig` files with only the changes from the default configuration. To do so, execute:

```bash
make nconfig
make uboot-update-defconfig
make linux-update-defconfig
make busybox-update-config
```

Then, you can compile with:

```bash
make 2>&1 | tee build.log
```

Finally, save the `defconfig` file in your external directory for future usage:

```bash
make savedefconfig
cp defconfig <BR2_EXTERNAL_COTTI_PATH>/configs/<boardname>_defconfig
```

## Using the files provided in the example

If you are starting from a fresh Buildroot repo, you just need to load the `<BR2_EXTERNAL_COTTI_PATH>/<boardname>_defconfig` with:

```bash
make <boardname>_defconfig
```

Given that the `.config` files are already pointing to the external files in the repo, whenever you issue the following commands to save the changes made, and they will be stored in the external repo:

```bash
# Buildroot
make nconfig
make savedefconfig

# U-Boot
make uboot-nconfig
make uboot-update-defconfig

# Linux
make linux-nconfig
make linux-update-defconfig

# Busybox
make busybox-menuconfig
make busybox-update-config
```

Whenever you do a menuconfig, those changes are stored in the locals `.config` and they do influence the next builds. However, changes will be lost after a cleanup or manually deleting the `.config` files.

## Flashing and checking the build

1. Flash an SD card with an MBR partition table with a FAT32 64MiB bootable partition and a Linux ext4 partition and create a filesystems.

    ```bash
    sudo dd if=/dev/zero of=/dev/sda bs=1M count=16
    sudo cfdisk /dev/sda
    sudo mkfs.vfat -a -F 32 -n boot /dev/sda1
    sudo mkfs.ext4 -L rootfs -E nodiscard /dev/sda2
    ```

2. Copy the SPL, U-Boot binary, device tree blob and Kernel into the boot partition.

    ```bash
    cp output/images/u-boot.img output/images/MLO output/images/zImage output/images/am335x-boneblack.dtb /media/${USER}/boot/
    ```

3. Copying the rootfs into the Linux partition.

    ```bash
    sudo tar -C /media/$USER/rootfs/ -xf output/images/rootfs.tar
    ```

4. Connect to the serial port with picocom, and prepare the boot commands:

    ```bash
    picocom -b 115200 /dev/ttyUSB0
    (u-boot) setenv bootcmd "fatload mmc 0:1 0x81000000 zImage; fatload mmc 0:1 0x82000000 am335x-boneblack.dtb; bootz 0x81000000 - 0x82000000"
    (u-boot) setenv bootargs "console=ttyS0,115200n8 root=/dev/mmcblk0p2 rootfstype=ext4 rw rootwait"
    (u-boot) saveenv
    (u-boot) boot
    [...] # Linux booting messages
    # Inside the Linux OS
    Cotti s BBB with Buildroot!
    cotti login: root
    $ ninvaders
        # The game starts to be played.
    ```

<!--Internal links-->
[buildroot_section]: /docs/embedded/buildroot/buildroot

<!--External links-->
[bbb_board]: https://www.beagleboard.org/boards/beaglebone-black
[ninvaders]: https://ninvaders.sourceforge.net/
