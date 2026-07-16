.PHONY: all build docker-build build-local build-kernel build-boot build-boot-elf \
        concatinate run debug clean format

ARCH := x86

CFLAGS := -g -ffreestanding -m32 -Iinclude -Iarch/$(ARCH)/include -nostdlib

BUILD_DIR := build
BIN_DIR := bin


C_SRCS := $(shell find kernel drivers lib arch -name '*.c')
ASM_SRCS := $(shell find kernel drivers lib arch -name '*.asm' ! -name 'boot.asm')

C_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst %.asm,$(BUILD_DIR)/%_asm.o,$(ASM_SRCS))

KERNEL_OBJS := $(C_OBJS) $(ASM_OBJS)

KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_BIN := $(BIN_DIR)/kernel.bin

BOOT_ELF := $(BUILD_DIR)/boot.o
BOOT_BIN := $(BIN_DIR)/boot.bin

DISK_IMG := $(BIN_DIR)/disk.img

all: clean build run

format:
	find . -regex '.*\.\(c\|h\|cpp\|hpp\|cc\|cxx\)' \
		-exec clang-format -style=LLVM -i {} \;

fmt: format	

build: docker-build

docker-build:
	docker run --rm \
		--user "$(id -u):$(id -g)" \
		-v "$(PWD):/workspace" \
		-w /workspace \
		kernel-builder \
		make build-local

build-local: clean build-kernel build-boot build-boot-elf concatinate



$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	i686-elf-gcc $(CFLAGS) -c $< -o $@



$(BUILD_DIR)/%_asm.o: %.asm
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@



build-kernel: $(KERNEL_OBJS)
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

	i686-elf-ld \
		-m elf_i386 \
		-T kernel/linker.ld \
		-o $(KERNEL_ELF) \
		$(KERNEL_OBJS)

	i686-elf-objcopy \
		-O binary \
		$(KERNEL_ELF) \
		$(KERNEL_BIN)



build-boot:
	@mkdir -p $(BIN_DIR)
	./scripts/kernel-size.sh $(ARCH)
	nasm -f bin arch/$(ARCH)/boot.asm -o $(BOOT_BIN)

build-boot-elf:
	@mkdir -p $(BUILD_DIR)
	nasm -f elf32 -DELF_BUILD arch/$(ARCH)/boot.asm -o $(BOOT_ELF)



concatinate:
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(DISK_IMG)


run:
	qemu-system-x86_64 \
		-drive file=$(DISK_IMG),format=raw

debug:
	qemu-system-x86_64 \
		-drive file=$(DISK_IMG),format=raw \
		-S -gdb tcp::1234
		
dbg: debug

gdb:
	pwndbg -x debug.gdb


clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	mkdir -p $(BUILD_DIR) $(BIN_DIR)