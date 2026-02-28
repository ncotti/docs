# Renode platforms: .repl

The [**RE**node **PL**atform description format][renode_repl] is a YAML file used to describe a "platform", i.e, the hardware for a SoC, a peripheral, a CPU, etc.

Each platform description file consists of solely the fundamental unit "**entry**", whose basic format is as follows:

```yaml
[local] <variable_name>: [type_name] [registration_info]
    [attribute0]
    [attribute1]
```

For example, a flash memory called `flash` is of type `Memory.MappedMemory`, registered at the system bus's address `0x08000000`, with a size of `0x200000` (2MB).

```yaml
flash: Memory.MappedMemory @ sysbus 0x08000000
    size: 0x200000
```

The type, the info and the attributes are optional, but at least one of them must be present.

It is heavily encouraged to start your work based upon one of the [example Renode platforms][renode_platforms_github].

The next sections review the entries' elements' characteristics.

## Variables

Variables can have any name. There can be multiple definitions of the same variable and, in that case, their properties are merged or overwritten by the last declaration.

You can import all variables in another `.repl`, with a given "prefix", with the `using` keyword:

```yaml
using "<path_to_other_repl_file>" [prefixed "prefix"]
```

However, variables declared with the `local` keyword can only be used inside the same file.

## Types

The type name indicates the class of the peripheral, let it be a CPU, memory, an UART, etc.

All peripherals are declared as C# classes, which can be found spread throughout the [renode-infrastructure repo][renode_infra_repo], under the `src/Emulator` subfolders.

For example, the type [CPU.CortexM][renode_cortexm_peripheral] and the type [Memory.MappedMemory][renode_mapped_memory] is declared as:

```c#
namespace Antmicro.Renode.Peripherals.CPU {
    public partial class CortexM : Arm, IPeripheralWithTransactionState }

namespace Antmicro.Renode.Peripherals.Memory {
    public sealed class MappedMemory : IBytePeripheral, ... }
```

When referencing this C# classes in a `.repl` file, the `Antmicro.Renode.Peripherals` part of the name can be can be omitted.

### Registration info

The registration info associates the peripheral type with a register at a given address:

```yaml
@ <register> [address]
```

The following examples show how a sensor is given the address `0x80` in a I2C bus, and how and UART module is mapped to the memory address `0x80000000` in the sysbus, i.e, the main memory.

```yaml
sensor: Sensors.SI70xx @ i2c0 0x80
uart0: UART.MiV_CoreUART @ sysbus 0x80000000
```

In the 99% percent of cases the register will be the `sysbus`, which represents the whole memory map. In the other 1%, you will either use `none` as the register or repeated declarations as:

```yaml
@ {
    <register1> [address];
    <register2> [address]
}
```

The address value can be defined with:

* An beginning address in hexadecimal.
* `<begin, end>`: The beginning and ending address.
* `<begin, +size>`: The beginning address and the size of the memory region.

## Attributes

Attributes define properties of the device. There are three kinds of attributes:

1. **Constructor attributes**: Map to arguments of the C# class's constructor.

    ```yaml
    <constructor_arg>: <value>
    ```

    ```yaml
    flash: Memory.MappedMemory @ sysbus 0x08000000
        size: 0x200000
    ```

2. **Interrupt attributes**: Map and interruption source to a receiver.

    ```yaml
        [irq_source] -> <irq_controller>@<irq_id>
    ```

    The C# class must have defined at least one property of type `GPIO`.

    ```c#
    [DefaultInterrupt]
    public GPIO IRQ { get; } = new GPIO();

    public GPIO DMARequest { get; } = new GPIO();
    ```

    The `[irq_source]` is optional. If none is provided, the one marked with the `[DefaultInterrupt]` decorator gets connected.
    `[irq_source]` can be the name of the property, or a list of names or a list of numbers (if `INumberedGPIOOutput` is implemented). The number of irq sources must match the amount of `<irq_ids>`.

    The `<irq_controller>` is any C# class that inherits from the `IIRQController` or the `IGPIOReceiver` interfaces, which must have several interrupt lines each represent by an `<irq_id>`.

    ```yaml
    usart1: UART.STM32_UART @ sysbus <0x40011000, +0x100>
        -> nvic@37
    ```

3. **Monitor attributes**: Execute monitor commands on the variable.

    ```yaml
    init [add]:
        <monitorStatementN>

    preinit [add]:
        <monitorStatementN>

    reset [add]:
        <monitorStatementN>
    ```

    The three types affect when the command is executed:

    * `init`: Executes the command on the variable.
    * `preinit`: Executes the command before the variable is created.
    * `reset`: Executes the command when the variable is being reset.

    If `[add]` is included, then the statements are appended. Otherwise, they are overwritten.

    A typical monitor statement is to initialize registers of the `sysbus` to a default value. This can be done with a [`SVD` file][cmsis_svd] or by using the sysbus's method `WriteDoubleWord`[^1].

    ```yaml
    sysbus:
        init:
            WriteDoubleWord 0xE000ED08 0x08000000
            ApplySVD @<path_to_svd_file>
    ```

<!--Footnotes-->

[^1]: In the Renode examples, you may find a `Tag` statement to initialize the registers instead of `WriteDoubleWord`. The first implementation was not working the last time I tested it.

<!--External links-->
[renode_repl]: https://renode.readthedocs.io/en/latest/advanced/platform_description_format.html
[renode_infra_repo]: https://github.com/renode/renode-infrastructure/tree/master/src/Emulator
[renode_platforms_github]: https://github.com/renode/renode/tree/master/platforms
[renode_cortexm_peripheral]: <https://github.com/renode/renode-infrastructure/blob/master/src/Emulator/Cores/Arm-M/CortexM.cs>
[renode_mapped_memory]: <https://github.com/renode/renode-infrastructure/blob/master/src/Emulator/Main/Peripherals/Memory/MappedMemory.cs>
[cmsis_svd]: <https://arm-software.github.io/CMSIS_5/SVD/html/index.html>
