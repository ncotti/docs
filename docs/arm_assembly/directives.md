# Assembly directives

Assembly directives serve the same purpose as C preprocessors directives[^1]: they tell [The GNU Assembler][gnu_assembler_manual] how to do its job. They all have names that begin with a period (`.`), e.g. `.section`, and define sections, variables, macros, etc.

[^1]: C preprocessors directives are the ones that start with a `#`, such as `#define`, `#ifndef`, etc. The directives are always interpreted before the compilation process.

The following sections are a summary of the most useful directives. A full list can be found at [The GNU Assembler: Assembler Directives][assembler_directives].

## Sections

A section is a set of machine instructions grouped together. The syntax for defining a section is as follows:

```arm
.section <name> [, <flags>, %<type>]
```

For example, the following directives create two sections named `._my_text` and `.my_data`, for storing instruction code and variables respectively.

```arm
.section .my_text, "ax", %progbits
    // Some code
.section .my_data, "aw", %progbits
    // Some variables
```

All instructions following the `.section` directive will be put inside that section. The `<flags>` indicate the properties of the section that will be used by the linker, and the `%<type>` indicate what's in the section. By default, new sections are only allocatable (`a`) and of type (`%nobits`):

* `a`: Allocatable (uninitialized memory region, same as .bss).

* `x`: Executable (same as .txt).

* `w`: Writable (same as .data).

* `S`: Contains null-terminated strings.

* `%progbits`: Contains code or initialized data (used for .data, .text)

* `%nobits`: Uninitialized data.

The C compiler generates the following default sections.

* `.text`: holds executable instructions. `("ax", %progbits)`.

* `.data`: holds initialized global variables and constants with labels. `("aw", %progbits)`.

* `.rodata`: holds initialized read only data. `("a", %progbits)`

* `.bss` and `COMMON`: short for "Block Started by Symbol", holds data storage areas that should be initialized to zero at the beginning of program execution, e.g. reserving 1000 bytes for a vector. The [COMMON section][common_section] is an alias that some compilers may use. `("aw", %nobits)`.

These special sections can be used as directives themselves, without specifying section, flags or types:

```arm
.text
.data
.bss
.rodata
```

Besides, an specific symbol can be given a type with the `.type` directive. Normally, it's not needed to define it, unless you want to call an assembly function from C (as we will see in the ABI section):

```arm
.type <symbol_name>, %<function | object>
```

```arm
// Declare a function in section .text
.text
.type function_symbol, %function
function_symbol:

// Declare a data object in section .data
.data
.type data_symbol, %object
data_symbol
```

## Variables and constants

This is a list of the possible types of constants that can be defined:

```arm
char:   .byte   0       // 1 byte variable i=0
short:  .hword  1       // 2 byte variable (half word) j=1
int:    .word   2       // 4 byte variable (word) k=2
asc:    .ascii  "hello" // Ascii string without NULL character
str:    .string "hello" // Ascii string with NULL character
float:  .float  1.2     // Single precision number
double: .double 4.4     // Double precision number
array:  .word 0,1,2,3,4 // Integer array array[5] = {0,1,2,3,4}
```

For example:

```arm
.global i,j,k,l

.data
i: .byte 0      // 0x00 -> 0x01
j: .hword 1     // 0x01 -> 0x03
k: .word 2,3    // 0x03 -> 0x0b
l: .word 4      // 0x0b -> 0x0f
```

## Filling and alignment

Since ARM is a 32-bit architecture, it is desired that variables are stored in memory positions multiples of 4. The alignment directives are as follows:

```arm
.align  x   // The next address will end with "x" zeros.
.balign x   // The next address will be multiple of "x"
.space size // Create a space of "size" bytes with zeros.
```

For example:

```arm
.global i,j,k,l

.data
i: .byte 0      // 0x00 -> 0x01
.align 2        // Force two zeros in next address -> 0x04
j: .hword 1     // 0x04 -> 0x06
.align 4        // Force four zeros -> 0x10
k: .word 2,3    // 0x10 -> 0x18
.balign 16      // Multiple of 16 -> 0x20
.space 33       // Fill 33 bytes with -> 0x41
l: .word 4      // 0x41 -> 0x45
```

## Preprocessor directives

Preprocessor directives are functionally equivalent to the ones that you would see in the C language.

### Setting and manipulating symbols

Giving an alias to a register helps with following the logic of the code.

```arm
<alias_name> .req <rx>      // Give "alias_name" to register "rx"
.unreq <alias_name>         // Undefine previous "alias_name"
```

The `.equ` and `.alias` directive are functionally equivalent to a `#define`.

```arm
.equ <symbol>, <expression>
alias <symbol>, <alias_name>
```

By default, all variables are static to the file they are created, unless the `.global` directive is used:

```arm
.global <symbol>             // Set visibility outside of the file
```

You can use weak aliases to symbols by first declaring the symbol as `.weak`, and then using `.set` to associate it to a **local** symbol or expression. Only **local** symbols can be weak. A strong symbol is defined as any symbol in other module, externally linked by the linker.

```arm
.weak <weak_symbol>
.set <weak_symbol>, <expression>
```

### Conditional assembly

```arm
.if expression || .ifdef symbol || .ifndef symbol
    // do stuff
.else
    // do other stuff
.endif
```

Three possible conditional are available:

* `.if <expression>`: the expression must be an absolute non-zero value to be evaluated as true.

* `.ifdef <symbol>`: the symbol must have been defined.

* `.ifndef <symbol>`: the symbol must not have been defined.

### Including files and symbols

The `.include` directive replaces the `#include` one. It copies the file contents in the position where it is called.

```arm
.include <file>
```

The `.extern` defines a symbol not in this file, whether it was declared in other file with `.global` or is a linker symbol.

```arm
.extern <symbol>
```

### Macros

Macros are a more powerful version of `#define`. They are text replacements that can have arguments and default values.

For example, the following code produces this disassembly of the object file:

```arm
.macro SHIFT arg1, arg2=5
    lsl \arg1, \arg1, #\arg2
.endm

.text
SHIFT r1, 3
SHIFT r2
```

```txt
$ arm-linux-gnueabihf-objdump -d a.out
00000000 <.text>:
   0:   e1a01181        lsl     r1, r1, #3
   4:   e1a02282        lsl     r2, r2, #5
```

### CPU mode

By declaring `.thumb` or `.arm`, you can force the code to be executed on either Thumb mode (16 bits) or ARM mode (32 bits).

```arm
.thumb || .code 16
.arm   || .code 32
```

## Recommended bibliography

[Modern Assembly Language Programming with the ARM Processor, Larry D Pyeatt][arm_book]

<!-- External links -->
[gnu_assembler_manual]: https://sourceware.org/binutils/docs/as/index.html
[assembler_directives]: https://sourceware.org/binutils/docs/as/Pseudo-Ops.html
[common_section]: https://sourceware.org/binutils/docs/ld/Input-Section-Common.html
[arm_book]: https://shop.elsevier.com/books/modern-assembly-language-programming-with-the-arm-processor/pyeatt/978-0-443-14114-0
