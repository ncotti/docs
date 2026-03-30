# Buildroot external package development

An external package is the more stylistic and clean way of including packages into Buildroot. However, they require some extra steps and configurations compared to in-tree packages.

Buildroot places packages in external directories. The latter must contain three files:

* `external.desc`: Name and description of the external directory. Only two lines are required with its name `<NAME>` and description.

    ```txt
    name: <NAME>
    desc: <description>
    ```

* `Config.in`: A Kconfig file to be added in Buildroot's menuconfig. Its options will appear under the "External options ---> " submenu. It usually just contains `source` statements to the package's `Config.in` such as:

    ```kconfig
    source "$(BR2_EXTERNAL_<PKG>_PATH)/package/<pkg1>/Config.in"
    source "$(BR2_EXTERNAL_<PKG>_PATH)/package/<pkg2>/Config.in"
    ```

* `external.mk`: A Makefile which should only include each package's Makefile with the following instruction:

    ```make
    include $(sort $(wildcard $(BR2_EXTERNAL_<NAME>_PATH)/package/*/*.mk))
    ```

A typical file structure looks like this, which imitates Buildroot's internal file structure:

```bash
external_dir
├── board/
│   └── <company>/
│       └── <boardname>/
│           ├── linux.config
│           ├── busybox.config
│           ├── <other .config files>
│           ├── post_build.sh
│           ├── post_image.sh
│           ├── rootfs_overlay/
│           │   ├── etc/
│           │   └── <other files>
│           └── patches/
|               └── <pkgX>
|                   └── <.patch file>
├── configs
│   └── <boardname>_defconfig
├── package
│   └── <pkg1>
│       ├── Config.in
│       ├── <pkg1>.hash
│       └── <pkg1>.mk
├── Config.in
├── external.desc
└── external.mk
```

The contents of your package in `package/<pkg1>/` are the same ones described in [BR in-tree packages section][br_internal_section].

## Setting Buildroot's configuration for external packages

For your external packages to be recognized, you need to call `make` at least once with the `BR2_EXTERNAL` variable equal to a list of colon-separated paths of the external directories, either absolute or relative to Buildroot's source. The value of `BR2_EXTERNAL` gets stored for future calls unless explicitly reassigned in a future `make` call.

```bash
make BR2_EXTERNAL=<path/to/external1>:</path/to/external2> nconfig
```

Inside the menuconfig, you can reference the contents of the external directory with the env. variable `BR2_EXTERNAL_<NAME>_PATH`, where `<NAME>` is defined in the `external.desc` file.

Following the sample file tree from before, the configuration values would look like this, where you just point to the rootfs overlay, post-build and post-image scripts and the custom kernel configuration inside the external directory, just to name a few.

```bash
BR2_GLOBAL_PATCH_DIR="$(BR2_EXTERNAL_<NAME>_PATH)/board/<company>/<boardname>/patches/"
BR2_ROOTFS_OVERLAY="$(BR2_EXTERNAL_<NAME>_PATH)/board/<company>/<boardname>/rootfs_overlay/"
BR2_ROOTFS_POST_BUILD_SCRIPT="$(BR2_EXTERNAL_<NAME>_PATH)/board/<company>/<boardname>/post_build.sh"
BR2_ROOTFS_POST_IMAGE_SCRIPT="$(BR2_EXTERNAL_<NAME>_PATH)/board/<company>/<boardname>/post_image.sh"
BR2_LINUX_KERNEL_USE_CUSTOM_CONFIG=y
BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE="$(BR2_EXTERNAL_<NAME>_PATH)/board/<company>/<boardname>/linux.config"
```

It is also worth noting that external directories hold Buildroot's configuration in the `${BR2_EXTERNAL_<NAME>_PATH}/configs/<boardname>_defconfig` file.

To save the `defconfig` file, run this command and then move the generated file into your external package.

```bash
make savedefonfig
cp defconfig <BR2_EXTERNAL_<NAME>_PATH>/configs/<boardname>_defconfig
```

Since the external package mimics the source tree from Buildroot, all `defconfig` files inside this directory are treated as if they were in Buildroot's tree itself. Therefore, you can load this default configuration by just writing its name:

```bash
make <boardname>_defconfig
```

### How to setup a custom device tree

If, inside your external repository you have your own out-of-tree device tree, it is not so straight-forward to include it in your build.

This is because the device tree is required for both Linux and U-Boot and, normally, these two programs have their own `.dts` files and build their own `.dtb`.

Therefore, the right way of doing this is to:

1. Have a copy of the device tree sources in `${BR2_EXTERNAL_<NAME>_PATH}/board/<company>/<boardname>/dts/`.

2. Create a pre-build script that compiles the `.dts` file into an `.dtb` file.

    ```bash
    DT_PATH="${BR2_EXTERNAL_<NAME>_PATH}/board/<company>/<boardname>/dts"
    DTS="${DT_PATH}/<dts.dts>"
    DTB="${DT_PATH}/<dtb.dtb>"

    cpp -nostdinc -undef -x assembler-with-cpp -Idts \
        "${DTS}" > "${DTS}.preprocessed"

    dtc -I dts -o "${DTB}" "${DTS}.preprocessed"

    rm -f "${DTS}.preprocessed>"
    ```

3. Tell U-Boot to use an external device tree blob with the command argument:

    ```bash
    EXT_DTB=$(BR2_EXTERNAL_<NAME>_PATH)/board/<company>/<boardname>/dts/<dtb.dtb>
    ```

4. Tell Linux not to build a device tree blob.

5. Create a post-image script that copies the DTB into the binaries folder:

    ```bash
    cp "${BR2_EXTERNAL_<NAME>_PATH}/board/<company>/<boardname>/dts/<dtb.dtb>" \
    "${BINARIES_DIR}/<dtb.dtb>"
    ```

## Examples of BR2_EXTERNAL trees

[buildroot-external-st][external_st].

[buildroot-external-microchip][external_microchip].

[buildroot-external-nxp][external_nxp].

<!--Internal links-->
[br_internal_section]: /docs/embedded/buildroot/br_package_intree

<!--External links-->
[external_st]: https://github.com/bootlin/buildroot-external-st
[external_microchip]: https://github.com/linux4sam/buildroot-external-microchip
[external_nxp]: https://github.com/boundarydevices/buildroot-external-boundary
