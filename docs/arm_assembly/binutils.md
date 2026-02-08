# The GNU Binutils

When calling the GNU C Compiler (GCC), a lot of steps are done for us under the hood. Let's review all these steps, and see the information the binary files have to offer with the [GNU Binutils][gnu_binutils].

![All required elements to compile and flash a program to an embedded device](images/binutils.png)

All the steps described below can be conveniently executed from the script [buildutils.sh](/arm_assembly/binutils/buildutils.sh) provided in this repo.

## 1. C preprocessor (cpp)

The preprocessor handles all the so called "preprocessor directives", which include:

* `#define`: macro replacement.

* `#include`: copying the literal contents of the header file inside the source file.

* Remove all comments.

* `#ifdef | #endif`: Conditional compiling.

```bash
arm-linux-gnueabihf-cpp main.c > build/main.i
```

Check the file generated `main.i`, and see the changes made in the file.

## 2. The GNU C Compiler (gcc)

The gcc then parses the C code into Assembly code. In our case, it's ARM assembly.

```bash
arm-linux-gnueabihf-gcc -S -o build/main.s build/main.i
```

## 3. The Assembler (as)

It creates an object file based on the assembly code.

```bash
arm-linux-gnueabihf-as -o build/main.o build/main.s
$ file build/main.o
    build/main.o: ELF 32-bit LSB relocatable, ARM, EABI5 version 1 (SYSV), not stripped
```

See that the type of the file is ELF (Executable and Linkable Format), for 32-bit ARM processors.

## 4. The linker (ld)

It locates the code in the corresponding memory regions, and generates the final executable file.

```bash
arm-linux-gnueabihf-ld -o build/main.elf build/main.o
$ file build/main.elf
    build/main.elf: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), statically linked, not stripped
```

You may specify the option `-Map <map_file>` to print a "map file", which is a description of the memory addresses.

Normally, preprocessing, compiling, assembling and even linking can be done all together with a single call to the GCC.

## Binary files inspection and modification tools

So far, we have built two ELF files (Executable and Linkable Format): the object file (.o), and the executable file (.elf). Now, let's see what information they provide and how to obtain it.

### Display information of ELF files (objdump)

`objdump` is a powerful command that encompasses the functionality of various, more specific, commands, such as:

* `readelf`: for reading ELF headers.
* `size`: for reading assembly regions sizes (.text, .data, etc).
* `nm`:  for listing the symbol table.

The [contents of an ELF file](https://linux-audit.com/elf-binaries-on-linux-understanding-and-analysis/) are:

* ELF file header: includes file type (ELF32), data endianness, machine (ARM), and operating system (UNIX).

* Program headers: they show the memory address where code will be loaded.

* Section headers: shows all the assembly sections (.text, .bss, .data, etc), with their address, size and offset.

* Symbol table: shows all symbols defined in the file. This is specially useful when the linker has to merge several files together, so it can search for a function or global variable in that object file.

```bash
$ arm-linux-gnueabihf-objdump -x build/main.elf

build/main.elf:     file format elf32-littlearm
build/main.elf
architecture: arm, flags 0x00000112:
EXEC_P, HAS_SYMS, D_PAGED
start address 0x00010094

Program Header:
    LOAD off    0x00000000 vaddr 0x00010000 paddr 0x00010000 align 2**16
         filesz 0x000000d0 memsz 0x000000d0 flags r-x
    LOAD off    0x000000d0 vaddr 0x000200d0 paddr 0x000200d0 align 2**16
         filesz 0x00000004 memsz 0x00000004 flags rw-
   STACK off    0x00000000 vaddr 0x00000000 paddr 0x00000000 align 2**4
         filesz 0x00000000 memsz 0x00000000 flags rw-
private flags = 5000400: [Version5 EABI] [hard-float ABI]

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         0000003c  00010094  00010094  00000094  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000004  000200d0  000200d0  000000d0  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .comment      00000024  00000000  00000000  000000d4  2**0
                  CONTENTS, READONLY
  3 .ARM.attributes 00000033  00000000  00000000  000000f8  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
00010094 l    d  .text  00000000 .text
000200d0 l    d  .data  00000000 .data
00000000 l    d  .comment       00000000 .comment
00000000 l    d  .ARM.attributes        00000000 .ARM.attributes
00000000 l    df *ABS*  00000000 main.c
000200d0 g     O .data  00000004 global_var
000200d4 g       .data  00000000 _bss_end__
000200d4 g       .data  00000000 __bss_start__
000100b4 g     F .text  0000001c add
000200d4 g       .data  00000000 __bss_end__
000200d4 g       .data  00000000 __bss_start
00010094 g     F .text  00000020 main
000200d4 g       .data  00000000 __end__
000200d4 g       .data  00000000 _edata
000200d4 g       .data  00000000 _end
```

This command can also act as a **disassembler**, showing the assembly instructions and their position in memory.

```bash
$ arm-linux-gnueabihf-objdump -d build/main.elf

build/main.elf:     file format elf32-littlearm
Disassembly of section .text:

00010094 <main>:
   10094:       b580            push    {r7, lr}
   10096:       b082            sub     sp, #8
   10098:       af00            add     r7, sp, #0
   1009a:       230a            movs    r3, #10
   1009c:       607b            str     r3, [r7, #4]
   1009e:       230f            movs    r3, #15
   100a0:       603b            str     r3, [r7, #0]
   100a2:       6839            ldr     r1, [r7, #0]
   100a4:       6878            ldr     r0, [r7, #4]
   100a6:       f000 f805       bl      100b4 <add>
   100aa:       2300            movs    r3, #0
   100ac:       4618            mov     r0, r3
   100ae:       3708            adds    r7, #8
   100b0:       46bd            mov     sp, r7
   100b2:       bd80            pop     {r7, pc}

000100b4 <add>:
   100b4:       b480            push    {r7}
   100b6:       b083            sub     sp, #12
   100b8:       af00            add     r7, sp, #0
   100ba:       6078            str     r0, [r7, #4]
   100bc:       6039            str     r1, [r7, #0]
   100be:       687a            ldr     r2, [r7, #4]
   100c0:       683b            ldr     r3, [r7, #0]
   100c2:       4413            add     r3, r2
   100c4:       4618            mov     r0, r3
   100c6:       370c            adds    r7, #12
   100c8:       46bd            mov     sp, r7
   100ca:       f85d 7b04       ldr.w   r7, [sp], #4
   100ce:       4770            bx      lr
```

### Discard symbols from object files (strip)

The `strip` command will erase the symbol table from the binary. This is used before shipping a binary to a client to reduce it's size and obscure information about the code (the symbol table is needed only for linking, not for execution).

```bash
# Before stripping, file size = 1064 and symbol table can be found
$ ls -l build/main.elf
    -rwxrwxr-x 1 cotti cotti 1064 abr 13 16:44 build/main.elf

$ arm-linux-gnueabihf-objdump -t build/main.elf
    build/main.elf:     file format elf32-littlearm
    SYMBOL TABLE:
    00010094 l    d  .text  00000000 .text
    000200d0 l    d  .data  00000000 .data
    ...
    000200d4 g       .data  00000000 _end

# After stripping, file size = 588, and no symbol table
arm-linux-gnueabihf-strip build/main.elf
$ ls -l build/main.elf
    -rwxrwxr-x 1 cotti cotti 588 abr 13 16:46 build/main.elf

$ arm-linux-gnueabihf-objdump -t build/main.elf
    build/main.elf:     file format elf32-littlearm
    SYMBOL TABLE:
    no symbols
```

### Get a binary file with no information (objcopy)

Files that have the `.elf` extension have all the headers and symbol tables, which might be useful for loading the code in hardware or execute it later. Files that have the `.bin` extension had all these information purposefully removed, as well as its code and data cramped together starting from the address 0x00.

```bash
arm-linux-gnueabihf-objcopy -O binary build/main.elf build/main.bin
$ file build/main.bin
    build/main.bin: data

$ ls -l build
-rwxrwxr-x 1 cotti cotti 65600 abr 13 17:23 main.bin
-rwxrwxr-x 1 cotti cotti  1064 abr 13 17:20 main.elf
```

The final `.bin` file is larger than it's `.elf` counterpart, even though header information was removed. Looking again at the section's header, we can see that the address (LMA) of the `.text` section is 0x10094, and the address of the `.data` is at 0x200d0, roughly 65500 KBytes apart. What we did was to move the address of the `.text` section to 0x00, and then keep the 65500 KBytes separation for the `.data`, filling the file with just "0".

```txt
Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         0000003c  00010094  00010094  00000094  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000004  000200d0  000200d0  000000d0  2**2
                  CONTENTS, ALLOC, LOAD, DATA
```

<!--External links-->
[gnu_binutils]: https://opensource.com/article/19/10/gnu-binutils
