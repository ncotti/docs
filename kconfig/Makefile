SHELL=/bin/bash
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:

CONFIG_FILE := $(wildcard .config)

include $(CONFIG_FILE)

#------------------------------------------------------------------------------
# Variables
#------------------------------------------------------------------------------
CFLAGS := -c
LDFLAGS :=

SRC_DIRS ?= src
HEADER_DIRS ?= inc

BUILD_DIR := build
EXE := exe
CC := gcc

#------------------------------------------------------------------------------
# Menuconfig processing
#------------------------------------------------------------------------------
# Include CFLAGS from menuconfig
ifdef CONFIG_USE_CUSTOM_CFLAGS

ifdef CONFIG_CFLAGS_OPT
CFLAGS += -O3
endif

ifdef CONFIG_CFLAGS_WALL
CFLAGS += -Wall
endif

endif

# Include linker script if one
ifdef CONFIG_GENERATE_MEMORY_MAP
LDFLAGS += -Wl,-Map=$(CONFIG_LDFLAGS_MEMORY_MAP)
endif

# Add all macros from menuconfig as "-D" symbols
MACROS := -DCONFIG_FIRST_PRINTF_TEXT='$(CONFIG_FIRST_PRINTF_TEXT)'

ifdef CONFIG_ENABLE_SECOND_PRINTF
MACROS += -DCONFIG_SECOND_PRINTF_TEXT='$(CONFIG_SECOND_PRINTF_TEXT)'
endif

MACROS += -DCONFIG_MATH_FIRST_OPERAND=$(CONFIG_MATH_FIRST_OPERAND)
MACROS += -DCONFIG_MATH_SECOND_OPERAND=$(CONFIG_MATH_SECOND_OPERAND)

ifdef CONFIG_MATH_OPERATION_ADD
MACROS += -DCONFIG_MATH_OPERATION_ADD
else ifdef CONFIG_MATH_OPERATION_SUB
MACROS += -DCONFIG_MATH_OPERATION_SUB
else ifdef CONFIG_MATH_OPERATION_MUL
MACROS += -DCONFIG_MATH_OPERATION_MUL
endif

#------------------------------------------------------------------------------
# Variable processing
#------------------------------------------------------------------------------

KCONFIG := Kconfig

ELF := $(BUILD_DIR)/$(EXE).elf

HEADER_INCLUDES := $(addprefix -I, $(HEADER_DIRS))

SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS))
OBJS := $(patsubst %.c, %.o, $(OBJS))

BUILD_SRC_DIRS := $(addprefix $(BUILD_DIR)/, $(SRC_DIRS))

#------------------------------------------------------------------------------
# User targets
#------------------------------------------------------------------------------
.PHONY: all
all: $(ELF)
	@echo -e "\nExecuting program $(ELF)..."
	./$(ELF)

.PHONY: clean
clean:
	rm -Rf $(BUILD_DIR)

.PHONY: clean_config
clean_config:
	rm -f .config .config.old

.PHONY: menuconfig
menuconfig:
	kconfig-mconf $(KCONFIG)

.PHONY: nconfig
nconfig:
	kconfig-nconf $(KCONFIG)

.PHONY: xconfig
xconfig:
	kconfig-qconf $(KCONFIG)

#------------------------------------------------------------------------------
# Compilation targets
#------------------------------------------------------------------------------

$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c $(BUILD_SRC_DIRS) Makefile .config
	$(CC) $(CFLAGS) $(MACROS) $(HEADER_INCLUDES) -o $@ $<

$(BUILD_SRC_DIRS):
	mkdir -p $@