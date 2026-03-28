# Buildroot

[Buildroot][buildroot] is a tool that can build the four needed components of any embedded Linux project:

1. Toolchain.
2. Bootloader.
3. Kernel.
4. Filesystem.

It uses a singular menuconfig configuration and builds the binaries required.

## Installation

```bash
git clone https://gitlab.com/buildroot.org/buildroot.git
cd buildroot
git checkout 2026.02
```

```bash
make nconfig
make
make savedefconfig
```

In the menuconfig, we will see the configurations for each component. Let's review them one by one:

* Target options: CPU architecture details (ARM Cortex-A8, RISC-V, etc) EABI, floating point.

* Toolchain options: Two main options: build a "buildroot" toolchain from scratch, or using an "external" toolchain, such as Bootlin's, ARM's or one made with Crosstool-NG. The most critical options here are the C library and the linux header's version.

* Build options: Things related to Buildroot's compilation process.

* System configuration: Things related to the "core" of the user-space execution. Includes things like:
    * The rootfs structure.
    * The system's hostname.
    * The `init` process (BusyBox init, systemd, etc).
    * How `/dev` is mounted.
    Also, allows to define rootfs overlays and post-build scripts.

* Kernel: Buildroot won't actually configure the kernel for you. You need to provide the `.config` configuration and the device tree. You can either use an in-tree defconfig, or use a `.config` generated from the kernel's own menuconfig. The same goes for the device tree blob.

* Target packages: First, it will install Busybox. You can point to a custom `.config` (in which case, you should clone the busybox repo and get the configuration yourself), or use the default one. After that, everything that could be installed with a `sudo apt install` can be chosen to be placed in the target. Besides

* Filesystem images: The type of the target's filesystem (squshfs, ubifs, ext4).

* Bootloaders: Includes U-Boot. You must specify the U-boot `.config` file, the device tree (including the make argument `DEVICE_TREE=`) and the binary format, i.e., whether to generate a `u-boot.img`, `u-boot.elf`, `u-boot.bin`; and, if U-boot is generating and SPL, the name of the binary file.

* Host utilities: Tools to be installed in the host's computer.

## Basic usage

Once the build of Buildroot finished, prepare your SD card with two partitions:

```bash
sudo dd if=/dev/zero of=/dev/sda bs=1M count=16
sudo cfdisk /dev/sda
sudo mkfs.vfat -a -F 32 -n boot /dev/sda1
sudo mkfs.ext4 -L rootfs -E nodiscard /dev/sda2
```

Then, copy the U-boot image, the SPL, the dtb and the kernel `zImage` into the fat partition from `output/images`. Then extract the rootfs (if "tarring" the rootfs was chosen in Buildroot's build options) into the SD ext4 partition from `output/`.

```bash
sudo tar -C /media/$USER/rootfs/ -xf output/images/rootfs.tar
```

ANd that's it. Buildroot didn't actually do anything you didn't know. Even, you have to provide the configuration for all the tools. But it is useful to have
everything neatly wrapped in one place.

## Useful hacks

You can use modify the kernel menuconfig inside buildroot. No need to download the linux repo.

In Buildroot's menuconfig, point to a default in-tree defconfig file.

```bash
make nconfig                    # Start with a default defconfig
make linux-nconfig              # Update Linux menuconfig
```

Do changes as needed. The changes are stored in `output/build/linux-<version>/.config`. Move that `.config` file to a custom location, and update Buildroot's menuconfig to point to that custom .config

```bash
make nconfig
make linux-update-defconfig
```

Configuration will be saved at `output/build/linux-<version>/defconfig`.

## Buildroot internals

Once the build folder is created, it looks like this:

```bash
output
├── build
├── host
├── images
├── staging -> host/arm-buildroot-linux-gnueabihf/sysroot
└── target
```

Where:

* `build`: Whee all source tarballs are extracted and build of each package takes place.
* `host`: Contains all the tools for the host, including the cross-compiler.
* `staging`: Staging area containing headers and binaries with symbols. Used debugging and cross-compiling purposes. Symbolic link to the sysroot.
* `target`: Skeleton of the target's filesystem. This directory is just for show. You will see a file generated inside called `THIS_IS_NOT_YOUR_ROOT_FILESYSTEM`, that explains that it lacks the proper permissions of the actual rootfs (the owner of the files should not be the UID of the host machine).
* `images`: This is where you will find all the required outputs: The bootloader image, the kernel image, the device tree blob and the rootfs image.

## Root filesystem overlay

A root filesystem overlay is simply a directory whose contents will be copied over the root filesystem, after all packages have been installed. Overwriting files is allowed.

In menuconfig, search for `BR2_ROOTFS_OVERLAY`, in `System configuration -> () Root filesystem overlay directories`. This is a whithespace-separated list of paths to be copied.

Log:

```bash
>>>   Copying overlay <overlay_path>
```

Changes should be visible in the `output/target` directory.

## Post build script

A post-build script, as it names implies, can be executed before the final filesystem image is created. They are called with the path to the target directory `${TARGET_DIR}` as their first argument (also accessible as an environment variable), which is equivalent to the `/` directory. Therefore, writing to `${TARGET_DIR}/etc`, is the equivalent of writing to `/etc` in the target.

This script is executed on the host to modify the soon-to-be filesystem.

In menuconfig, search for `BR2_ROOTFS_POST_BUILD_SCRIPT` in `System configuration -> () Custom scripts to run before creating filesystem images` and provide a whitespace-separated list of scripts to execute.

Log:

```bash
>>>   Executing post-build script <script path>
```

Changes should be visible in the `output/target` directory.

Near this menuconfig option, you can opt to execute custom scripts in different stages of the build process:

* Before commencing the build
* After the build, before creating the filesystem images
* Inside the fakeroot environment: Not recommended
* After creating filesystem images: especially useful to automatically flash or install the images.

## Permission tables

By default, all files are owned by the `root` user.

To customize the ownership of the permission of installed files, one can create **permission tables**, both for devices and for users. They are found in `system` path.

Look for the configurations `BR2_ROOTFS_DEVICE_TABLE` and `BR2_ROOTFS_USERS_TABLES` inside the `System configuration` menu.

## Adding new packages to Buildroot

A package is defined as a target or host application. The requirements for a package are:

1. A directory `package/<pkg>`
2. A `Config.in` file, written in *Kconfig* language, describing the configuration options for the package.
3. A `<pkg>.mk` Makefile, describing where to fetch the source, how to build and install it, etc.
4. A `<pkg>.hash` file, providing hashes to check the integrity of the downloaded tarballs and license files.
5. Optional `.patch` files, that are applied on the package source code before building.

### Config.in

A KConfig file called `Config.in`. The only mandatory configuration option is the one that enables/disables the package. It can be a `config` or a `menuconfig` that encompasses configs related to the package.

```bash
menuconfig BR2_PACKAGE_<PKG>
    bool "<pkg>"
    <config_options>
if BR2_PACKAGE_<PKG>
    <pacakge-related configs>
endif
```

After that, you must manually add a line to `source` this file in the `packages/Config.in` file, so that its options can be seen from the right Buildroot's menuconfig menu, e.g "Target packages --> Miscellaneous --> <pkg>".

Package dependencies can be dealt with using Kconfig's `depends on` and `select`:

Use `depends on BR2_PACKAGE_<dependency_pkg>` to hide this package, unless the dependency has been selected. (For example, only present this package for ARM architecture, or systems with threads or an MMU, or if it has python installed).

Use `select BR2_PACKAGE_<dependency_pkg>` to forcefully select a dependency of this package in the menuconfig, enabling the necessary packages needed to build the current package.

### Package Makefile

Buildroot does not rewrite the build steps for each package, but rather leverages their existing build system. Buildroot will use a different **package infrastructure** depending on the type of build system: hand-written Makefile, autotools, Meson, Cmake, etc.

Nonetheless, each package infrastructure is divided in six stages. Depending on the infrastructure, some stages are already defined, or must be defined by the user.

```mermaid
flowchart LR
    Download --> Extract
    Extract --> Patch
    Patch --> Configure
    Configure --> Build
    Build --> Install
```

The structure of the `.mk` file is a succession of variables, ending with a call to the desired package infrastructure macro. The variables' name must be prefixed with the name of the package in uppercase:

```bash
# Variable definition
<PKG>_FOO = "Some Variable"

# Package infrastructure call
$(eval $(generic-package))
$(eval $(autotools-package))
$(eval $(cmake-package))
[...]
```

These variable control the actions taken in the different stages:

* Download, extract and patching stages:

    These variables act as the "metadata" for the package.

    First, point to the source files. In the case of a tarball, the full URL from where is downloaded is `$(<PKG>_SITE)/$(<PKG>_SOURCE)`

    ```bash
    <PKG>_SITE = <url_to_download_package>
    <PKG>_VERSION = <tarball version, commit hash, tag, etc>
    <PKG>_SOURCE = <tarball_filename>
    <PKG>_SITE_METHOD = <git | wget | scp | file | local | svn>
    ```

    Then, specify the license code in [SPDX][spdx] format and the name of the license file:

    ```bash
    <PKG>_LICENSE = <SPDX license format>
    <PKG>_LICENSE_FILES = <license_filenames_relative_to_top-level_source_directory>
    ```

    Also, you may specify the [Common Platform Enumeration][cpe] (CPE):

    ```bash
    <PKG>_CPE_VENDOR = <vendor>
    <PKG>_CPE_PRODUCT = <product>
    <PKG>_CPE_VERSION = <version>

    cpe:2.3:a:<VENDOR>:<PRODUCT>:<VERSION>:*:*:*:*:*:*:*
    cpe:2.3:a:google:chrome:9.0.597.7:*:*:*:*:*:*:*
    ```

    If you specify a list of patches, they will be automatically be downloaded and applied. It also searches for valid patches in `BR2_GLOBAL_PATCH_DIR` and int `package/<pkg>/*.patch` You may aswell download any extra files:

    ```bash
    <PKG>_PATCH = <list of patches to download and automatically apply>
    <PKG>_EXTRA_DOWNLOADS = <List of additional files>
    ```

    Package dependencies listed in the Makefile only guaranties that they will be installed before the package. However, they must have been `select`ed for installation in the menuconfig.

    ```bash
    <PKG>_DEPENDENCIES = <Packages names of dependencies>
    ```

* Configure stage:

    `$(@D)` represents the path of the current package.

    The variable `$(TARGET_MAKE_ENV)` is defined in the `package/Makefile.in`, and updates the value of the $(PATH) variable to point to Buildroot's host tools.

    The variable `$(TARGET_CONFIGURE_OPTS)` is defined in the `package/Makefile.in`, and is a collection of variables to be passed on any `$(MAKE)` call.

    ```bash
    TARGET_CONFIGURE_OPTS = \
    [...]
    CC="$(TARGET_CC) \
    CFLAGS="$(TARGET_CFLAGS)" \
    LDFLAGS="$(TARGET_LDFLAGS) \
    [...]
    ```

    Your package may need to modify some CFLAGS or whatever, in that case you should NOT modify the "TARGET_CFLAGS" variable, but instead overwrite it last. In Make, variables are left from left to right, so writing first TARGET_CONFIGURE_OPTS and then CFLAGS=X, the second one overwrites the first.

    ```bash
    define <PKG>_CONFIGURE_CMDS
        <bash_commands>
    endef
    ```

* Build stage:

    ```bash
    define <PKG>_BUILD_CMDS
        <bash_commands>
        $(TARGET_MAKE_ENV) $(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
    endef
    ```

* Install stage:

    The package can be installed to the target filesystem, to the staging area or as a binary image. For either case, a variable to enable the installation and the commands used must be declared. Also, and env. variable that points to each directory is provided.

    ```bash
    # Installs to $(TARGET_DIR)
    <PKG>_INSTALL_TARGET = <YES|NO>
    define <PKG>_INSTALL_TARGET_CMDS
        $(TARGET_MAKE_ENV) $(MAKE1) -C $(@D) DESTDIR=$(TARGET_DIR) install
    endef

    # Installs to $(STAGING_DIR)
    <PKG>_INSTALL_STAGING = <YES|NO>
    define <PKG>_INSTALL_STAGING_CMDS
        $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) DESTDIR=$(STAGING_DIR) install
    endef

    # Installs to $(BINARIES_DIR)
    <PKG>_INSTALL_IMAGES = <YES|NO>
    define <PKG>_INSTALL_IMAGES_CMDS
        $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) DESTDIR=$(STAGING_DIR) install
    endef
    ```

#### Packages hash file

For all downloaded files, a hash is required to validate the integrity of the downloaded files and licenses.

The file syntax is as follows, with TWO spaces between fields:

```bash
<hashtype>  <hash>  <file>
```

## BR2_EXTERNAL

```bash
make BR2_EXTERNAL=/path/to/external1:/path/to/external2
make BR2_EXTERNAL=/home/nicolas.cotti/cotti/docs/buildroot
```

## Application development

Add `output/host/bin/bin` to your path, i.e., where the cross-compiler was built.

It is recommended to enable `BR2_PACKAGE_HOST_ENVIRONMENT_SETUP` in `Host utilities --> host environemnt-setup`. After a build, it will generate a bash script called `output/host/environment-setup`, which sets all environment variables such as the paths, the C flags, the linker flags, etc. Then, you can source it to cross-compile your application.

<!--External links-->
[buildroot]: https://buildroot.org/
[spdx]: https://spdx.org/licenses/
[cpe]: https://en.wikipedia.org/wiki/Common_Platform_Enumeration
