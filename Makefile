# MIT License, Copyright (c) 2021 Marvin Borner

CC = $(PWD)/cross/opt/bin/i686-elf-gcc
LD = $(PWD)/cross/opt/bin/i686-elf-ld
OC = $(PWD)/cross/opt/bin/i686-elf-objcopy
ST = $(PWD)/cross/opt/bin/i686-elf-strip
AS = nasm

BUILD = $(PWD)/build/

CFLAGS = -Wall -Wextra -Werror -std=c99 -m32 -nostdlib -nostdinc -fno-builtin -fno-profile-generate -fno-omit-frame-pointer -fno-common -ffreestanding -Ofast

ASFLAGS = -f elf32

all: bootloader example

bootloader:
	@mkdir -p $(BUILD)
	@$(CC) -c $(CFLAGS) load.c -o $(BUILD)/load.o
	@$(LD) -N -emain -Ttext 0x00009000 -o $(BUILD)/load.bin $(BUILD)/load.o --oformat binary
	@$(AS) -f bin entry.asm -o $(BUILD)/boot.bin

example:
	@mkdir -p $(BUILD)
	@$(CC) -c $(CFLAGS) example.c -o $(BUILD)/example.o
	@$(LD) -N -ekernel_main -o $(BUILD)/example $(BUILD)/example.o

clean:
	@find . \( -name "*.o" -or -name "*.a" -or -name "*.elf" -or -name "*.bin" \) -type f -delete
