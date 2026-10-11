# Ares Romance GBA - Makefile for devkitARM
# Requires: devkitARM, tonc (included in devkitPro)

NAME := ares_romance
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include
GFX_DIR := $(SRC_DIR)/gfx

# Toolchain
PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
AS := $(PREFIX)as
LD := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
GBAFIX := gbafix

# Flags
ARCH := -mthumb -mthumb-interwork -mcpu=arm7tdmi
CFLAGS := $(ARCH) -O2 -Wall -Wextra -std=c99 -ffreestanding -fno-stack-protector
CFLAGS += -I$(INCLUDE_DIR) -I$(SRC_DIR) -D__GBA__
ASFLAGS := $(ARCH) -I$(INCLUDE_DIR)
LDFLAGS := $(ARCH) -specs=gba.specs -Wl,-Map,$(BUILD_DIR)/$(NAME).map
LIBDIRS := -L$(DEVKITPRO)/libgba/lib
LIBS := -lgba -lc -lgcc -lm

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(BUILD_DIR)/$(NAME).gba

# Link
$(BUILD_DIR)/$(NAME).elf: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBDIRS) $(LIBS)

# Convert to .gba
$(BUILD_DIR)/$(NAME).gba: $(BUILD_DIR)/$(NAME).elf
	$(OBJCOPY) -O binary $< $@
	$(GBAFIX) $@

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Run on emulator (optional)
run: $(BUILD_DIR)/$(NAME).gba
	mgba $<

.PHONY: all clean run