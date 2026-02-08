#!/bin/bash
#------------------------------------------------------------------------------
# Recopilation of commands for using the GNU Binutils to compile,
# assemble and link binary files; as well as see information and manipulate
# them.
#------------------------------------------------------------------------------
file_name=main

mkdir -p build

while true; do
    echo "Choose what to do:"
    echo "1. Run C preprocessor (cpp)."
    echo "2. Run GNU C Compiler (gcc)."
    echo "3. Run Assembler (as)."
    echo "4. Run linker (ld)."
    echo "5. See size of assembly regions (size)."
    echo "6. See ELF headers (objdump -x)."
    echo "7. See ELF dissasembly (objdump -d)."
    echo "9. Erase all binaries."
    echo "q. Quit."

    read -r -s -n 1 option
    echo ""
    clear
    case "$option" in
        1) {
            arm-linux-gnueabihf-cpp main.c > "build/${file_name}.i"
            echo "File ${file_name}.i generated.";
        };;

        2) {
            arm-linux-gnueabihf-gcc -S -o build/main.s "build/${file_name}.i"
            echo "File ${file_name}.s generated.";
        };;

        3) {
            arm-linux-gnueabihf-as -o "build/${file_name}.o" "build/${file_name}.s"
            echo "File ${file_name}.o generated."
        };;

        4) {
            arm-linux-gnueabihf-ld -o "build/${file_name}.elf" -Map build/map_file.map "build/${file_name}.o"
            echo "Files ${file_name}.elf and map_fiile.map generated."
        };;

        5) {
            arm-linux-gnueabihf-size "build/${file_name}.o"
            arm-linux-gnueabihf-size "build/${file_name}.elf"
        };;

        6) {
            read -r -s -n 1 -p "Select file (1. ${file_name}.o; 2. ${file_name}.elf):" file_selected
            echo ""
            if [ "${file_selected}" = "1" ]; then
                arm-linux-gnueabihf-objdump -x "build/${file_name}.o"
            else
                arm-linux-gnueabihf-objdump -x "build/${file_name}.elf"
            fi
        };;

        7) {
            read -r -s -n 1 -p "Select file (1. ${file_name}.o; 2. ${file_name}.elf):" file_selected
            echo ""
            if [ "${file_selected}" = "1" ]; then
                arm-linux-gnueabihf-objdump -d "build/${file_name}.o"
            else
                arm-linux-gnueabihf-objdump -d "build/${file_name}.elf"
            fi
        };;

        9) rm build/* &> /dev/null;;

        *) break;;
    esac
    echo ""
done