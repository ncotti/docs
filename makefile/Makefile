#------------------------------------------------------------------------------
# Makefile Initialization
#------------------------------------------------------------------------------
SHELL=/bin/bash
.DELETE_ON_ERROR:
.SILENT:
.DEFAULT_GOAL := help

#------------------------------------------------------------------------------
# User modifiable variables
#------------------------------------------------------------------------------
# E.g.: arm-none-eabi-, arm-linux-gnueabihf-, (left empty), etc
TOOLCHAIN ?= arm-linux-gnueabihf-

SRC_DIRS ?= src src/sub_src
HEADER_DIRS ?= inc inc/sub_inc

CFLAGS ?= -Wall -g
ASFLAGS ?= -g
LDFLAGS ?= -g

# Linker script (can be empty)
LDSCRIPT ?= linker_script.ld

# Name of the final executable (without extension)
EXE ?= exe

# Name of the gdb script (can be empty)
GDB_SCRIPT ?= debug.gdb

#------------------------------------------------------------------------------
# Binutils
#------------------------------------------------------------------------------
ifeq ($(origin CC), default)
CC := $(TOOLCHAIN)gcc
else
CC ?= $(TOOLCHAIN)gcc
endif
ifeq ($(origin AS), default)
AS := $(TOOLCHAIN)as
else
AS ?= $(TOOLCHAIN)as
endif
LD 			:= $(TOOLCHAIN)ld
OBJDUMP 	:= $(TOOLCHAIN)objdump
OBJCOPY 	:= $(TOOLCHAIN)objcopy

#------------------------------------------------------------------------------
# Miscellaneous constants
#------------------------------------------------------------------------------
PRINT_CHECKMARK 	:= printf "\033[0;32m\342\234\224\n\033[0m"
PRINT_CROSS 		:= printf "\033[0;31m\342\235\214\n\033[0m"
PRINT_WARNING		:= printf "\033[0;33m\342\232\240\n\033[0m"

#------------------------------------------------------------------------------
# File location
#------------------------------------------------------------------------------
BUILD_DIR	:= build
INFO_DIR 	:= info
ELF 		:= $(BUILD_DIR)/$(EXE).elf
BIN 		:= $(BUILD_DIR)/$(EXE).bin
MAP			:= $(BUILD_DIR)/$(INFO_DIR)/memory.map

# If you use "ld" or "gcc" as linker, the memory map option is declared different
ifneq (,$(findstring -ld, $(LD)))
LDFLAGS += -Map $(MAP)
else
LDFLAGS += -Wl,-Map=$(MAP)
endif

# If you are using a custom linker script, don't use the default crt0.S files
ifneq (,$(LDSCRIPT))
LDFLAGS += -T $(LDSCRIPT)
endif

SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c) $(wildcard $(dir)/*.s))

HEADERS := $(foreach dir, $(HEADER_DIRS), $(wildcard $(dir)/*.h) $(wildcard $(dir)/*.s))
HEADER_FLAGS := $(addprefix -I , $(HEADER_DIRS))

OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS))
OBJS := $(patsubst %.c, %.o, $(OBJS))
OBJS := $(patsubst %.s, %.o, $(OBJS))

OBJ_HEADERS := $(patsubst %.o, %.header, $(OBJS) $(ELF))
OBJ_HEADERS := $(patsubst %.elf, %.header, $(OBJ_HEADERS))
OBJ_HEADERS := $(patsubst $(BUILD_DIR)%, $(BUILD_DIR)/$(INFO_DIR)%, $(OBJ_HEADERS))

DIS_ASM := $(patsubst %.header, %.d, $(OBJ_HEADERS))

BUILD_SRC_DIRS := $(addprefix $(BUILD_DIR)/, $(SRC_DIRS))
INFO_SRC_DIRS := $(addprefix $(BUILD_DIR)/$(INFO_DIR)/, $(SRC_DIRS))

#------------------------------------------------------------------------------
# User targets
#------------------------------------------------------------------------------
.PHONY: compile
compile: $(ELF) ## Compile all source code, generate ELF file.

.PHONY: help
help: ## Display this message.
	grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) \
	| sort \
	| awk 'BEGIN {FS = ":.*?## "}; \
	{printf "\033[36m%-10s\033[0m %s\n", $$1, $$2}'

.PHONY: binary
binary: $(BIN) ## Generate binary file, without ELF headers.

.PHONY: headers
headers: $(OBJ_HEADERS) ## Generate symbol table and section headers for all object files.

.PHONY: dasm
dasm: $(DIS_ASM) ## Generate disassemble for all object files and elf file.

.PHONY: clean
clean: ## Erase contents of build directory.
	rm -Rf $(BUILD_DIR)
	echo -n "All files successfully erased "; $(PRINT_CHECKMARK)

.PHONY: run
run: $(BIN) kill ## Execute compiled program (using QEMU)
	echo -n "Initiating Qemu... "
	qemu-system-arm --version &>/dev/null
	coproc qemu-system-arm \
		-M realview-pb-a8 \
		-m 32M \
		-no-reboot -nographic \
		-monitor telnet:127.0.0.1:1234,server,nowait \
		-S -gdb tcp::2159 \
		-kernel $(BIN) &>/dev/null
	$(PRINT_CHECKMARK)

.PHONY: kill
kill: ## Stop qemu process running on background
	if ps -e | grep "qemu" &>/dev/null; then \
		echo -n "Old qemu process running on background. Killing... "; \
		# Get the line where "qemu" is \
		qemu_line=$$( ps -e | grep "qemu" ); \
		# Get only the first numbers (PID) \
		qemu_pid=$$( echo "$${qemu_line}" | grep -P -o '^[^\d]*\d+'); \
		# Remove prefixing spaces or non digits \
		qemu_pid=$$( echo "$${qemu_pid}" | grep -P -o '\d+'); \
		kill "$${qemu_pid}"; \
		$(PRINT_CHECKMARK); \
	fi

.PHONY: debug
debug: run ## Debug the program (no need to "make run" first, compile with "-g")
	gdb-multiarch -q -x "$(GDB_SCRIPT)" "$(ELF)"

#------------------------------------------------------------------------------
# Compilation targets
#------------------------------------------------------------------------------
# Main executable linking
$(ELF): $(OBJS)
	echo -n "Linking everything together... "
	mkdir -p $(BUILD_DIR)/$(INFO_DIR)
	$(LD) $(LDFLAGS) -o $@ $^
	$(PRINT_CHECKMARK)
	echo "Executable file \"$@\" successfully created."

# Compiling object files from C sources
$(BUILD_DIR)/%.o: %.c $(HEADERS) Makefile $(LDSCRIPT) $(BUILD_SRC_DIRS)
	echo -n "Compiling $< --> $@... "
	$(CC) $(CFLAGS) $(HEADER_FLAGS) -o $@ -c $<
	$(PRINT_CHECKMARK)

# Compiling object files from asm sources
$(BUILD_DIR)/%.o: %.s $(HEADERS) Makefile $(LDSCRIPT) $(BUILD_SRC_DIRS)
	echo -n "Assembling $< --> $@... "
	$(AS) $(ASFLAGS) $(HEADER_FLAGS) -o $@ -c $<
	$(PRINT_CHECKMARK)

# Print object files' headers
$(BUILD_DIR)/$(INFO_DIR)/%.header: $(BUILD_DIR)/%.o $(INFO_SRC_DIRS)
	echo -n "Printing $< -> $@... "
	$(OBJDUMP) -x $< > $@
	$(PRINT_CHECKMARK)

# Print elf file's header
$(BUILD_DIR)/$(INFO_DIR)/%.header: $(BUILD_DIR)/%.elf $(INFO_SRC_DIRS)
	echo -n "Printing $< -> $@... "
	$(OBJDUMP) -x $< > $@
	$(PRINT_CHECKMARK)

# Print object files' disassembly
$(BUILD_DIR)/$(INFO_DIR)/%.d: $(BUILD_DIR)/%.o $(INFO_SRC_DIRS)
	echo -n "Disassembling $< -> $@... "
	$(OBJDUMP) -d $< > $@
	$(PRINT_CHECKMARK)

# Print elf file disassembly
$(BUILD_DIR)/$(INFO_DIR)/%.d: $(BUILD_DIR)/%.elf $(INFO_SRC_DIRS)
	echo -n "Disassembling $< -> $@... "
	$(OBJDUMP) -d $< > $@
	$(PRINT_CHECKMARK)

# Copy ELF file into BIN file
$(BIN): $(ELF)
	echo -n "Creating binary file $@... "
	$(OBJCOPY) -O binary $(ELF) $(BIN)
	$(PRINT_CHECKMARK)

# Folders
$(BUILD_SRC_DIRS) $(INFO_SRC_DIRS) $(BUILD_DIR)/$(INFO_DIR):
	mkdir -p $@
