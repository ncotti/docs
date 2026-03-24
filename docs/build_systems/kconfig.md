# Kconfig: menuconfig

[Kconfig][kconfig_intro] is a language used to describe configuration options for a project. It won't build your project, but instead it will present a KConfig frontend (`menuconfig`, `nconfig`, `xconfig`), where the user selects all the configuration values; and will generate a `.config` file with Makefile variables to be used to conditionally compile the code.

The frontends need to be installed with:

```bash
sudo apt install kconfig-frontends
```

And executed depending on the TUI of your liking:

```bash
kconfig-mconf <KConfig_file>    # Launches menuconfig
kconfig-nconf <KConfig_file>    # Launches nconfig
kconfig-qconf <KConfig_file>    # Launches xconfig
```

KConfig files don't have a "standard" name, but the Linux kernel repo uses the name`Kconfig` in different paths.

## Using KConfig variables

All symbols declared within KConfig are prefixed with `CONFIG_`. Symbols that have not been set in the TUI are not declared. From the Makefile, include the `.config` file and use the variables as needed.

```bash
include .config

# Define your own CFLAGS, and append the ones provided within menuconfig
CFLAGS := -lc
CFLAGS += CONFIG_CFLAGS

ifdef CONFIG_SOME_SYMBOL
# Do something if the symbol was selected in menuconfig
endif

%.o: %.c
    gcc $(CFLAGS) -o $@ $<
```

## KConfig syntax

The syntax is divided in two parts:

* Defining a menu entry, i.e., the element that will be shown in the menu.
* The menu entry's properties.

Some definitions:

* `<prompt>`: What will be shown on the screen.
* `<symbol>`: The name of the configuration option.
* `<expr>`: A conditional expression based on symbols, using C-like arithmetic or logical operators (`=`, `!=`, `<`, `>=`, `!`, `&&`, `||`, etc).

An [example KConfig project][kconfig_example] can be found in this repo.

### Menu entries

Creates a single configuration option.

```kconfig
config <symbol>
    [config_options && dependency_options]
```

Creates a comment:

```bash
comment "<prompt>"
    [dependency_options]
```

Creates a menu to encompass symbols.

```bash
menu "<prompt>"
    [dependency_options]
    [visible if <expr>]

<configs_inside_menu>

endmenu
```

Creates a menu to encompass symbols. The menu itself is a boolean symbol and the symbols inside it will only be selectable if it has a `y` value.

```kconfig
menuconfig <symbol>
    bool <prompt>
    [config_options && dependency_options]
if <symbol>

<configs_inside_menuconfig>

endif
```

Creates a "radio button" list, where only one symbol can be selected. All symbols must be boolean.

```bash
choice
    bool <prompt>
    [optional]
    [config_options && dependency_options]

<configs_inside_choice>

endchoice
```

Includes a KConfig file:

```bash
source <filename>
```

Sets the title for the whole configuration window. Should be the first command:

```bash
mainmenu <title>
```

### Config options

Sets the symbol's type and the prompt to be shown in the GUI. An empty prompt means that the symbol won't be shown, but it will be defined.

```bash
<bool | tristate | string | hex | int> "<prompt>"
```

`bool` supports `y/n` for true or false values. `tristate` supports and additional `m` state, which represent the `module` value[^1]. The `tristate` type is not supported by default. To enable it, a symbol with `option modules` must be declared and set to `y` as follows:

```bash
config MODULES
    bool
    default y
    option modules
```

If the symbol is an integer or hexadecimal, you may define an allottable range:

```bash
range < ">=" this > < "<=" this >
```

Sets the default value for the symbol. If default is not specifically stated, a "false-like" value is assumed (`n`, `0`, empty string).

```bash
default <expr> [if <expr>]
```

Adds a help string to the symbol:

```bash
help
    <Help text>
    <Ends when a line with a lesser indentation is detected>
```

### Dependency options

When a symbol `depends on` another, it will not be set and it won't be shown in the menu unless expression evaluates to true.

```bash
depends on <expr>
```

When a symbol that `select`s another is set, it forcefully sets both. The `select`ed symbol is not shown in the menu, unless already visible, and its value can't be changed from the TUI, unless the parent symbol is de-selected first.

```bash
select <symbol> [if <expr>]
```

When a symbol that `imply`s another is set, it will set both ONLY if the latter is not visible in the menu. If its value can be directly selected with the TUI, then the user value takes precedence.

```bash
imply <symbol> [if <expr>]
```

<!--Footnotes-->
[^1]: In the Linux kernel context, an `m` value indicates that the feature should be compiled as a kernel module, instead of built-in.

<!--External links-->
[kconfig_intro]: https://docs.kernel.org/6.5/kbuild/kconfig-language.html

<!--Internal links-->
[kconfig_example]: https://github.com/ncotti/docs/tree/main/kconfig
