# Renode scripts: .resc

[**RE**node **SC**ripts][renode_scripts] are a compilation of monitor commands in a file, which launch and control the simulation.

Almost all Renode's scripts do these same steps:

1. Create a `machine`. A machine is defined as an empty board with a `sysbus`, i.e., a memory map, where all peripherals are connected.

2. Populate the machine with peripherals (usually by loading a `.repl` platform file).

3. Setup any logging or communication.

4. Start the simulation.

Whenever you launch `renode` from a terminal, the `command window` will popup. That's where all monitor commands are issued. To start a script right after opening this window add its path as an argument:

```bash
renode [script_path]
```

The following is a simple example that summarizes what a normal Renode script would look like:

```resc
:name: NRF52840
:description: This script runs Zephyr Shell demo on NRF52840.

using sysbus

// Create a machine
mach create "my_machine"

// Load platform
machine LoadPlatformDescription @platforms/cpus/nrf52840.repl

// Read and write through the UART0 (opens up a new window)
showAnalyzer uart0

// Load binary
$bin?=@https://dl.antmicro.com/projects/renode/renode-nrf52840-zephyr_shell_module.elf-gf8d05cf-s_1310072-c00fbffd6b65c6238877c4fe52e8228c2a38bf1f

sysbus LoadELF $bin
```

## Explaining some commands

There aren't a lot of Renode commands, but rather each peripheral brings to the table its own commands.

The `help <command>` only works for Renode commands, not peripheral ones, so you will find yourself going to the C# classes directly to understand what each command does.

When popping up Renode, you will start in the `(monitor)` environment. The only interesting thing you can do from here is to create a machine with `mach create`. This will switch the environment to `(machine-0)`, and you can go back with `Clear`.

Paths usually start with a `@` and can be URLs. After loading a platform like:

```resc
machine LoadPlatformDescription @platforms/cpus/nrf52840.repl
```

Now we can start seeing a lot of information.

With `peripherals` you can see a list of all buses and peripherals for the current machine.

TODO, keep adding as needed.

### UART

You can check if your UART has been configured correctly by inspecting its properties during the code execution.

For example, use `sysbus.uart0 BaudRate` to see if the BaudRate was correctly configured.

<!--External links-->
[renode_scripts]: https://renode.readthedocs.io/en/latest/basic/monitor-syntax.html
