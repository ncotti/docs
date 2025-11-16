# Makefile: the first and last choice for build systems

The author has mixed feelings about [GNU Make][makefile]. On the one hand, it is a great tool, simple to setup and gives you full control of the compilation chain. On the other hand, it gets hard to maintain for large code bases, and the syntax is hard to read or follow. Nevertheless, understanding Makefiles is a required skill because of the sheer volume of projects that use it as their build system.

In my personal experience, you start writing a Makefile in the first steps of a project and, as the code base grows larger, you either end up maintaining a brittle Frankenstein of a build system or you migrate to something more powerful and standard.

[Makefile Tutorial][makefile_tutorial] is a great introduction to the topic. In the next section a more succinct explanation is given on the topic.

A [comprehensive Makefile example][makefile_example] can be found in this repository.

## What is a Makefile

A Makefile is a shell script with extra steps. Its most prominent feature is the fact that it will execute a list of commands conditionally based on the last modification date of the source files.

The building blocks of Makefiles are **recipes**, which have the syntax shown below. Both **targets** and **prerequisites** refer to actual files in your file system, with their path relative to the Makefile. All prerequisites must be existing files or must be other targets in the Makefile with instructions to generate those files.

```make
targets: prerequisites
    command
    command
```

The recipe will be executed based on the following conditions:

* Any of the prerequisites have a modification date newer than the target.
* The target doesn't exist.
* It is a `.PHONY:` target (see [Special Targets](#special-targets-modify-makefiles-default-behavior)).

Prerequisites can also be targets, with their own commands associated. Order of execution goes from left to right for the prerequisites, and it is the same for the targets.

Targets work as menu entries. Executing `make` on the terminal will process the first target. Calling `make <target> [VAR=VALUE ...]` will execute the target `target`, and can be passed a list of environmental variables.

## How to write a Makefile

Under the hood, Makefiles are just a collection of Bash commands. However, `make` works as a preprocessor and therefore has its own syntax and functions.

It is strongly encouraged to use Make's functions and variables. If possible, try to avoid using pure Bash syntax and try to find a Make function to replace that functionality.

### Makefile variables

There are only two useful assignment operators for variables:

* Use `var := value` for imperative assignment.
* Use `var ?= value` for default value assignment (the variable `var` will only take the value `value` if it wasn't assigned before).

??? info
    There is a third type of assignment called *recursive*: `=`, which is confusing and not recommended. For example, if you have the variable `var = $(some_value)`, the actual value of `var` will get resolved at the moment the variable is used, not when it is defined.

    <!-- markdownlint-disable MD046 -->
    ```make
    y = "old_value"
    imperative_var := $(y)
    recursive_var = $(y)
    y = "new_value"

    all:
        @echo $(imperative_var) # prints old_value
        @echo $(recursive_var)  # prints new_value
    ```

They are used with `$(var)`. Their values are literal, and special characters such as quotes `"` or inline comments will be included in the value. Bash variables, on the other hand, must be used with a double dollar sign `$${var}`. All Makefile variables will be replaced with its value before executing any instruction, and can be used as targets or prerequisites.

You can `export` Make variables to use them in shell commands as such:

```make
export MAKE_VAR = some_value
all:
    @echo $${MAKE_VAR}  # Prints "some_value"
```

#### Automatic variables

Full list of [Automatic Variables][automatic_variables].

* `$@`: name of the target.
* `$^`: all the prerequisites, separated by spaces.
* `$?`: all the prerequisites newer than the target, separated by spaces.
* `$<`: the first prerequisite.

#### Implicit variables

The usage of [implicit rules][implicit_rules] won't be discussed. However, there is a [list of standard variables][implicit_variables] that is good to use, and you might have seen in other Makefiles like `CC, CFLAGS, CXX, CXXFLAGS` and so on.

The important thing to remember is that these variables should always be defined with a default value using `?=`, and allow the user to modify them from the command line. E.g., running `make CC=clang` will replace the compiler (1).
{.annotate}

1. Just writing `CC ?= <value>` would not resolve, because `CC` always has the default value `CC = cc`.

```make
ifeq ($(origin CC), default)
    CC := $(TOOLCHAIN)gcc
else
    CC ?= $(TOOLCHAIN)gcc
endif

CFLAGS ?= -Wall -g
```

### Makefile functions

[Make functions][makefile_functions] replace the most common operations needed for path and string manipulation. Here is a short list of the most useful ones:

```make
# Replace "pattern" with "replacement" in all
# whitespace-separated words in "text"
$(patsubst pattern, replacement, text)

# Add "prefix" to whitespace-separated "names"
$(addprefix prefix, names…)

# "list" is the iterable, "var" is the iterator's value (represented as
# a variable), and "text" is what it is done for each "var"
$(foreach var, list, text)
```

### Wildcards

Instead of using the `*` glob expansion for files, it is recommended to use the function `$(wildcard *.c)`[^1].

[^1]: If `*.c` is used, this will expand to all files ending in `.c`, but in the case that no file matches the pattern, it will print the literal `*.c`. In contrast, `$(wildcard *.c)` will print an empty string if the are no matches.

The **stem operator** `%` is used for [Static Pattern rules][static_pattern_rules]. It always appears in pairs: given a glob expansion from a source filename with `%`, the content that was inferred gets stored in the literal `%`, and is replicated to form a target filename.

Typical usages are to construct recipes, or in functions as shown below:

```make
SRCS ?= file1.c file2.c
OBJS ?= $(patsubst %.c, %.o, $(SRCS))

%.o: %.c
    command
```

## Special targets: modify Makefile's default behavior

The following [special targets][special_targets] modify the default behavior of the Makefile:

| Target       | Default behavior | When used |
|:------------------------:|-------------|-----------|
| `SHELL = <shell>` | Executes commands in `/bin/sh`. | Uses `<shell>` as the shell. |
| `.ONESHELL:` | Each command is executed in a separate shell. | All the commands in a recipe execute in the same shell session. |
| `.POSIX:` | Abort execution on the first shell session returning non zero (if `.ONESHELL:` is enabled, only the return value of the whole recipe is checked, intermediate commands are allowed to fail). | Abort execution on the first command returning non zero, even if `.ONESHELL` is used. |
| `EXPORT_ALL_VARIABLES:` | Makefile variables are not seen from shell commands, unless explicitly declared with `export`. | All Makefile variables are implicitly exported, and can be seen by shell commands. |
| `.DELETE_ON_ERROR:` | If an error occurs while compiling a target, and the namesake file was generated, it won't be deleted. | The target file is deleted on error. |
| `.SILENT:` | Prints to stdout the command being executed and its stdout. If the command starts with an `@`, then only stdout is printed. | Only stdout is printed (as if an implicit `@` were added before all commands). |
| `.DEFAULT_GOAL := <target>` | The first target in the file is executed when calling `make`. | The target set as the default goal is executed when calling `make`. |
| `.PHONY: <target>` | The target's commands will be run only if any of the prerequisites are older than the target. If there are no prerequisites, the target won't run if a namesake file exists. | The target will always run, regardless of prerequisites. |

<!-- External links -->
[makefile]: https://www.gnu.org/software/make/manual/make.html
[makefile_tutorial]: https://makefiletutorial.com/

<!-- Sections of the Make manual -->
[special_targets]: https://www.gnu.org/software/make/manual/html_node/Special-Targets.html
[automatic_variables]: https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
[static_pattern_rules]: https://www.gnu.org/software/make/manual/make.html#Static-Pattern
[makefile_functions]: https://www.gnu.org/software/make/manual/make.html#Functions
[implicit_rules]: https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
[implicit_variables]: https://www.gnu.org/software/make/manual/make.html#Implicit-Variables

<!-- Internal links -->
[makefile_example]: /makefile/Makefile
