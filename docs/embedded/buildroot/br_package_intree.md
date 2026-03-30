# Buildroot in-tree package development

The requirements for a package named `<pkg>` are:

1. A directory created inside `package/<pkg>`
2. A `Config.in` file, written in *Kconfig* language, describing its configuration options.
3. A `<pkg>.mk` Makefile, describing where to fetch the source, how to build and install it, etc.
4. A `<pkg>.hash` file, providing hashes to check the integrity of the downloaded tarballs and license files.
5. Optional `.patch` files, that are applied on the package source code before building.

!!! info
    This section explains the requirements and parts of Buildroot's packages. However, if you want to build your own package, follow the steps in the [BR external packages section][br_external_section].

## Config.in

A [KConfig][kconfig_section] file called `Config.in`. The only mandatory configuration option is the one that enables/disables the package which must be called `BR2_PACKAGE_<PKG>`. It can be a `config` or a `menuconfig` that encompasses configs related to the package.

```bash
menuconfig BR2_PACKAGE_<PKG>
    bool "<pkg>"
    <config_options>
if BR2_PACKAGE_<PKG>
    <pacakge-related configs>
endif
```

After that, you must manually add a line to `source` this file in the `packages/Config.in` file, so that its options can be seen from Buildroot's menuconfig menu, e.g. "Target packages --> Miscellaneous --> <pkg\>".

Package dependencies can be dealt with either Kconfig's `depends on` or `select`:

* Use `depends on BR2_PACKAGE_<dependency_pkg>` to hide this package, unless the dependency has been selected. Usually used for bigger dependencies, such as the CPU architecture or the presence of threads or Python.

* Use `select BR2_PACKAGE_<dependency_pkg>` to forcefully select a dependency of this package in the menuconfig, enabling the necessary packages needed to build the current package.

## Package Makefile

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

The structure of the `<pkg>.mk` file is a succession of variables, ending with a call to the desired package infrastructure macro.

```bash
# Variable definition
<PKG>_FOO = "Some Variable"

# Package infrastructure call (use one)
$(eval $(generic-package))
$(eval $(autotools-package))
$(eval $(cmake-package))
[...]
```

The variables' names must be prefixed with the name of the package in uppercase. These variable control the actions taken in the different stages:

* ^^Download, extract and patching stages^^:

    The following variables act as the package's metadata.

    First, point to the source files. In the case of a tarball, the full URL from where is downloaded is `$(<PKG>_SITE)/$(<PKG>_SOURCE)`. TODO git repo

    ```bash
    <PKG>_SITE = <url>
    <PKG>_VERSION = <tarball version, commit hash, tag, etc>
    <PKG>_SOURCE = <tarball_filename>
    <PKG>_SITE_METHOD = <git | wget | scp | file | local | svn>
    ```

    Then, specify the license code in [SPDX][spdx] format and the name of the license file:

    ```bash
    <PKG>_LICENSE = <SPDX_license_format>
    <PKG>_LICENSE_FILES = <license_files>
    ```

    Also, you may specify the [Common Platform Enumeration][cpe] (CPE):

    ```bash
    <PKG>_CPE_VENDOR = <vendor>
    <PKG>_CPE_PRODUCT = <product>
    <PKG>_CPE_VERSION = <version>

    cpe:2.3:a:<VENDOR>:<PRODUCT>:<VERSION>:*:*:*:*:*:*:*
    cpe:2.3:a:google:chrome:9.0.597.7:*:*:*:*:*:*:*
    ```

    If you specify a list of patches, they will be automatically be downloaded and applied. It also searches for valid patches in `BR2_GLOBAL_PATCH_DIR` and int `package/<pkg>/*.patch` You may also download any extra files.

    ```bash
    <PKG>_PATCH = <patches>
    <PKG>_EXTRA_DOWNLOADS = <files>
    ```

    Package dependencies listed in the Makefile only guarantees that they will be installed before the package. In consequence, they must have been `select`ed as a dependency in the Kconfig `Config.in` file as well.

    ```bash
    <PKG>_DEPENDENCIES = <package_dependencies>
    ```

* ^^Configure stage^^:

    The configuration entails all things to be done before actually building the package. You may define a list of bash commands to be executed in this step as such:

    ```bash
    define <PKG>_CONFIGURE_CMDS
        <bash_commands>
    endef
    ```

    Also, Buildroot gives you access to a set of useful env. variables:

    * `$(@D)`: Path of the current package.

    * `$(TARGET_MAKE_ENV)`: Defined in `package/Makefile.in`. Overwrites the `$(PATH)` variable to point to Buildroot's host tools and toolchain.

    * `$(TARGET_CONFIGURE_OPTS)`: Defined in `package/Makefile.in`. Is a set of variables to be passed on any `$(MAKE)` call, that has this format:

        ```bash
        TARGET_CONFIGURE_OPTS = \
        [...]
        CC="$(TARGET_CC) \
        CFLAGS="$(TARGET_CFLAGS)" \
        LDFLAGS="$(TARGET_LDFLAGS) \
        [...]
        ```

        If you need to overwrite some of these variables,you should NOT modify the `$(TARGET_XXX)` variable, since it has a global effect for all packages. Instead, just add `XXX=$(TARGET_XXX) <your_value>` at the end of the `$(MAKE)` call, since variables are read from left to right in the command line and get overwritten.

* ^^Build stage^^:

    For a hand-written Makefile infrastructure, the command should always look like this, where you are replacing the `${PATH}` env. variable, calling make with the default flags and executing it in the directory where the package sources where downloaded.

    ```bash
    define <PKG>_BUILD_CMDS
        <bash_commands>
        $(TARGET_MAKE_ENV) $(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) <extra_arguments>
    endef
    ```

* ^^Install stage^^:

    The package can be installed to the target filesystem `$(TARGET_DIR)`, to the staging area `$(STAGING_DIR)` or as a binary image in `$(BINARIES_DIR)`. For either case, a variable to enable the installation and the commands used for it must be declared.

    ```bash
    # Installs to $(TARGET_DIR)
    <PKG>_INSTALL_TARGET = <YES|NO>
    define <PKG>_INSTALL_TARGET_CMDS
        $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) DESTDIR=$(TARGET_DIR) install
    endef

    # Installs to $(STAGING_DIR)
    <PKG>_INSTALL_STAGING = <YES|NO>
    define <PKG>_INSTALL_STAGING_CMDS
        $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) DESTDIR=$(STAGING_DIR) install
    endef

    # Installs to $(BINARIES_DIR)
    <PKG>_INSTALL_IMAGES = <YES|NO>
    define <PKG>_INSTALL_IMAGES_CMDS
        $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) DESTDIR=$(BINARIES_DIR) install
    endef
    ```

## Package hash file

For all downloaded files, a hash is required to validate the integrity of the downloaded files.

The file syntax is as follows, with TWO spaces between fields:

```bash
<hashtype>  <hash>  <file>
```

<!--External links-->
[spdx]: https://spdx.org/licenses/
[cpe]: https://en.wikipedia.org/wiki/Common_Platform_Enumeration

<!--Internal links-->
[kconfig_section]: /docs/build_systems/kconfig
[br_external_section]: /docs/embedded/buildroot/br_package_external
