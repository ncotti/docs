# GDB Reference

In this document each of the most common GDB commands are detailed. For a full list of commands, please consult the [bibliography](#bibliography).

## Layout, configuration and startup commands

Start GDB with a compiled binary having used the `-g` flag with:

```bash
gdb [binary_file]
gdb -x [script.gdb] [binary_file]
```

GDB will always execute the commands in the `.gdbinit`, regardless of the usage of the `-x` flag.

**Quit** the debugger with

```gdb
(gdb) q[uit]
```

The **layout** of the debugger is configurable. You can see your source code or the CPU registers by typing the following commands, or by activating the **eXtra Awesome** mode (`Ctrl + x; a`).

```gdb
(gdb) lay[out] src
(gdb) lay[out] regs
```

**Refresh** the display or change the **focus** window (the focused window is the one that responds to the arrow keys for navigation).

```gdb
(gdb) ref[resh]
(gdb) foc[us] <layout_selected>
```

The program won't **run** until said so. To **set** command line arguments, type:

```gdb
(gdb) set args [arg_list ...]
(gdb) r[un]
```

**Help** with any command can be printed out with:

```gdb
(gdb) h[elp] [command]
```

## Flow control commands

**Continue** the program's normal operation. The flow of execution is usually stopped by a breakpoint or a watchpoint.

```gdb
(gdb) c[ontinue]
```

Go directly to the **next** line of code in the current file, passing over function calls.

```gdb
(gdb) n[ext]
```

**Step** into the next line of code to be executed, even entering function calls.

```gdb
(gdb) s[tep]
```

**Finish** execution of the current function, and go to the next line of code of the calling script.

```gdb
(gdb) fin[ish]
```

## Breakpoints and watchpoints

A **breakpoint** can be put at the beginning of a function call or in any line number in a file. All breakpoints get assigned a *breakpoint number* for future reference. If the file name is omitted, it will use the scope of the current file.

```gdb
(gdb) b[reak] [file_name:]function_name|line_number
```

Conditional breakpoints that use the program's variables and simple mathematical expressions can be set with an **if** statement:

```gdb
(gdb) break [file_name:]function_name|line_number [if condition]
```

A **watchpoint** can be put over a variable and it stops execution of the code every time the content of that memory address changes.

```gdb
(gdb) wat[ch] variable_name
```

List all the **info** of currently active breakpoints:

```gdb
(gdb) i[nfo] b[reak]

Num  Type        Disp  Enb  Address  What
1    breakpoint  keep  y    0xFFFF   in main at primec.c:34
```

**Delete** a breakpoint using the *breakpoint number*, or delete all breakpoints if no number is specified.

```gdb
(gdb) d[elete] [breakpoint_number ...]
```

**Disable** or **enable** a breakpoint:

```gdb
(gdb) dis[able] [breakpoint_number ...]
(gdb) en[able] [breakpoint_number ...]
```

**Ignore** a breakpoint until it has been crossed `x` times.

```gdb
(gdb) ign[ore] [breakpoint_number ...] [x]
```

Create a **temporary breakpoint** that will be deleted after being reached once.

```gdb
(gdb) tb[reak] [function_name]
```

## Displaying information

The **info** command displays all the information needed at the current step in execution. Some notable mentions are:

```gdb
(gdb) i[nfo] r[egisters]    # Print all registers, same info as in layout regs
(gdb) i[nfo] lo[cals]       # Print all local variables with their values
(gdb) i[nfo] ar[gs]         # Print arguments passed to function.
```

**Print** the variables' values in scope with:

```gdb
(gdb) p[rint] [variable_name]
(gdb) p vector[index]
(gdb) p *vector@[len]
```

Or print the content of memory addresses:

```gdb
(gdb) x[/amount] [hex_address]
```

**Display** a variable's value each time the execution stops with the `display` command.

```gdb
(gdb) display [variable_name]
(gdb) undisplay [variable_name]
```

Print the **backtrace** of the program, which includes all the function calls that needed to be made to be where you are, with its arguments and all the variables in scope.

```gdb
(gdb) bt full
```

Use a **printf** just like in C to obtain nicer outputs.

```gdb
(gdb) printf "some_text\n"
```

## GDB Scripting

Enable **logging** to a file:

```gdb
set logging enabled [on|off]
set logging file [file_name]
set logging overwrite [on|off]
```

Specify **commands** to be executed when a breakpoint is reached:

```gdb
break [file_name:][function_name|line_number]
    commands
    # actual commands, usually printing variables and memory values
end
```

Avoid downloading debug info form URLs:

```gdb
set debuginfod enabled off
```

## Bibliography

* [gdb QuickStart](https://web.eecs.umich.edu/~sugih/pointers/gdbQS.html#:~:text=If%20you%20want%20gdb%20to,or%20gdb%20encounters%20a%20breakpoint.).

* [gdb Cheatsheet](https://cs.brown.edu/courses/cs033/docs/guides/gdb.pdf).

* [GDB: The GNU Project Debugger](https://www.sourceware.org/gdb/documentation/).
