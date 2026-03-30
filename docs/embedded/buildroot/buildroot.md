# Buildroot

[Buildroot][buildroot] is a tool that can build the four needed components of any embedded Linux project:

1. Toolchain.
2. Bootloader.
3. Kernel.
4. Filesystem.

It uses a singular menuconfig configuration and builds the binaries required.

## Installation

Clone the repository and checkout to the latest stable branch:

```bash
VERSION="2026.02"
git clone https://gitlab.com/buildroot.org/buildroot.git
cd buildroot
git checkout ${VERSION}
```

## Default usage

In this section we will be describing how to build and use Buildroot as is, without any external package.

Run the typical menuconfig TUI and then compile.

```bash
make nconfig
make
```

In the menuconfig, we will see the configurations for each component. Let's review them one by one:

* **Target options**: CPU architecture details (ARM Cortex-A8, RISC-V, etc) EABI, floating point.

* **Toolchain options**: Two main options: to build a "buildroot" toolchain from scratch, or to use an "external" toolchain, such as Bootlin's, ARM's or one made with Crosstool-NG. The most critical options here are the C library and the linux header's version.

* **Build options**: Things related to Buildroot's compilation process.

* **System configuration**: Things related to user-space execution and the root filesystem (rootfs) assembly. Includes things like:
    * The rootfs structure.
    * The system hostname.
    * The `init` process (BusyBox init, systemd, etc).
    * How `/dev` is mounted.
    * Definition of rootfs overlays and post-build scripts.

* **Kernel**: Buildroot won't actually configure the kernel for you. You need to provide the `.config` configuration and the device tree. You can either use an in-tree defconfig, or use a `.config` generated from the kernel's own menuconfig. The same goes for the device tree blob.

* **Target packages**: It will always install Busybox for the basic shell utilities. You can point to a custom `.config` configuration or use the default one. After that, almost everything that could be installed with a `sudo apt install` can be included as a package in this menu.

* **Filesystem images**: The type of the target's filesystem (squshfs, ubifs, ext4). Unless you are generating a read-only filesystem image, it is best just to select the option to "tar the root filesystem". This is because you don't actually want Buildroot to generate the filesystem, but rather make it yourself in the boot device and copy the rootfs contents there.

* **Bootloaders**: Includes U-Boot. You must specify U-Boot's `.config` file, the device tree (including Make's argument `DEVICE_TREE=`) and the binary format, i.e., whether to generate a `u-boot.img`, `u-boot.elf`, `u-boot.bin`; and, if U-Boot is generating and SPL, its name.

* **Host utilities**: Tools to be installed in the host's computer.

Once the build finished, the `output` folder is created:

```bash
output
├── build
├── host
├── images
├── staging -> host/arm-buildroot-linux-gnueabihf/sysroot
└── target
```

Where:

* `build`: Where each package source tarballs are extracted and built.

* `host`: Contains all the host's tools, including the cross-compiler.

* `staging`: Staging area containing headers, libraries and binaries with symbols. Used for debugging and cross-compiling purposes, so that applications built for the target have access to the proper libraries and paths. It is a symbolic link to the sysroot.

* `target`: Skeleton of the target's filesystem. This directory is just for show, as to check that the contents of the rootfs are correct. You will see a file generated inside called `THIS_IS_NOT_YOUR_ROOT_FILESYSTEM` which explains that it lacks the proper permissions of the actual rootfs, and that you should actually extract it from the `rootfs.tar` image with: `sudo tar -C <dst_path> -xf images/rootfs.tar`.

* `images`: Where all the required outputs can be found: the bootloader image, the kernel image, the device tree blob and the rootfs tar image.

Once all images have been generated inside `output/images`, you can do the standard procedure of:

1. Flashing a block device with a boot and a Linux partition and creating their filesystems.

    ```bash
    sudo dd if=/dev/zero of=/dev/sda bs=1M count=16
    sudo cfdisk /dev/sda
    sudo mkfs.vfat -a -F 32 -n boot /dev/sda1
    sudo mkfs.ext4 -L rootfs -E nodiscard /dev/sda2
    ```

2. Copying the SPL, U-Boot binary, device tree blob and Kernel into the boot partition.

    ```bash
    cp output/images/u-boot.img output/images/MLO output/images/zImage output/images/am335x-boneblack-wireless.dtb /media/${USER}/boot/
    ```

3. Copying the rootfs into the Linux partition.

    ```bash
    sudo tar -C /media/$USER/rootfs/ -xf output/images/rootfs.tar
    ```

4. Boot.

### Using other project's menuconfig menu

Since you are required to provide your own `.config` for U-Boot, Linux and BusyBox, Buildroot provides a way to generate these configuration files without having to individually clone each repo and copy their `.config` file.

First, go into Buildroot's menuconfig and point to some `.config` file, either in-tree or external.

```bash
make nconfig    # Set path the .config files
```

Then, all usual menuconfig-like commands for each package can be executed with the `<pkg>-<command>` syntax, e.g.:

```bash
make uboot-
make linux-nconfig
make busybox-menuconfig
```

!!! warning
    If you start with an in-tree configuration file, your changes will be stored in `output/build/<project>/.config`, which is prone to be erased. To avoid this, it is recommended to move the `.config` files to an external location and then point to them in Buildroot's menuconfig.

Do changes as needed. The changes are stored in `output/build/linux-<version>/.config`. Move that `.config` file to a custom location, and update Buildroot's menuconfig to point to that custom .config

## Root filesystem overlays

A root filesystem overlay is simply a directory whose contents will be copied over to the root filesystem, after all packages have been installed. Overwriting files is allowed.

In menuconfig, search for `BR2_ROOTFS_OVERLAY`, in `System configuration -> () Root filesystem overlay directories`. This is a whithespace-separated list of paths to be copied.

In the build logs, you should see something like this, and the changes in the rootfs should be visible in the `output/target` directory.

```bash
>>>   Copying overlay <overlay_path>
```

## Post build script

A post-build script, as it names implies, can be executed before the final filesystem image is created. It has access to the `${TARGET_DIR}` env. variable, which is equivalent to the `/` directory. Therefore, writing to `${TARGET_DIR}/etc`, is equivalent to writing `/etc` in the target.

This script is executed on the host to modify the soon-to-be filesystem, so it has access to the host's tools.

In menuconfig, search for `BR2_ROOTFS_POST_BUILD_SCRIPT` in `System configuration -> () Custom scripts to run before creating filesystem images` and provide a whitespace-separated list of scripts to execute.

In the build logs, you should see something like this, and the changes in the rootfs should be visible in the `output/target` directory.

```bash
>>>   Executing post-build script <script path>
```

Near this menuconfig option, you can opt to execute custom scripts in different stages of the build process:

* Before commencing the build.
* After the build, before creating the filesystem images.
* After creating filesystem images: especially useful to automatically flash or install the images.

They all have access to the environment variables pointing to the output paths, `${HOST_DIR}`, `${STAGING_DIR}`, `${TARGET_DIR}`, `${BUILD_DIR}` `${BINARIES_DIR}`, `${BASE_DIR}`; and one for Buildroot's `.config` file `${BR2_CONFIG}`. If you are working in an external directory, you also have access to `${BR2_EXTERNAL_<NAME>_PATH}`.

## Permission tables

By default, all files are owned by the `root` user.

To customize the ownership and permissions of installed files, one can create **permission tables**, both for devices and for users. They are found in the `system` path.

Look for the configurations `BR2_ROOTFS_DEVICE_TABLE` and `BR2_ROOTFS_USERS_TABLES` inside the `System configuration` menu.

## Adding new packages to Buildroot

A package is defined as a target or host application. There are two ways to add new packages:

1. [In-tree][br_intree_package_section]: Inserting your package inside Buildroot directory.
2. [External][br_external_package_section]: Using `BR2_EXTERNAL` external packages.

The latter approach is preferred, although the former is best for learning and understanding Buildroot's internal workings.

## Application development

Add `output/host/bin/bin` to your path, i.e., where the cross-compiler was built.

It is recommended to enable `BR2_PACKAGE_HOST_ENVIRONMENT_SETUP` in `Host utilities --> host environemnt-setup`. After a build, it will generate a bash script called `output/host/environment-setup`, which sets all environment variables such as the paths, the C flags, the linker flags, etc. Then, you can source it to cross-compile your application.

<!--Internal links-->
[br_intree_package_section]: /docs/embedded/buildroot/br_package_intree
[br_external_package_section]: /docs/embedded/buildroot/br_package_external

<!--External links-->
[buildroot]: https://buildroot.org/
