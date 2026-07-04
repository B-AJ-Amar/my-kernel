.PHONY: all build docker-build build-local build-kernel build-boot build-boot-elf \
        concatinate run debug clean format

CFLAGS := -g -ffreestanding -m32 -Iinclude -nostdlib

BUILD_DIR := build
BIN_DIR := bin

KERNEL_SRCS := $(shell find kernel drivers lib -name '*.c')
KERNEL_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))

KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_BIN := $(BIN_DIR)/kernel.bin

BOOT_ELF := $(BUILD_DIR)/boot.o
BOOT_BIN := $(BIN_DIR)/boot.bin

DISK_IMG := $(BIN_DIR)/disk.img

all: clean build run

format:
	find . -regex '.*\.\(c\|h\|cpp\|hpp\|cc\|cxx\)' \
		-exec clang-format -style=LLVM -i {} \;

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

build-kernel: $(KERNEL_OBJS)
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

	i686-elf-ld \
		-T kernel/linker.ld \
		$(KERNEL_OBJS) \
		-o $(KERNEL_ELF)

	i686-elf-objcopy \
		-O binary \
		$(KERNEL_ELF) \
		$(KERNEL_BIN)

build-boot:
	@mkdir -p $(BIN_DIR)
	./scripts/kernel-size.sh
	nasm -f bin boot/boot.asm -o $(BOOT_BIN)

build-boot-elf:
	@mkdir -p $(BUILD_DIR)
	nasm -f elf32 -DELF_BUILD boot/boot.asm -o $(BOOT_ELF)

concatinate:
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(DISK_IMG)

run:
	qemu-system-x86_64 \
		-drive file=$(DISK_IMG),format=raw

debug:
	qemu-system-x86_64 \
		-drive file=$(DISK_IMG),format=raw \
		-S -gdb tcp::1234
#	qemu-system-x86_64 \
#		-drive file=$(DISK_IMG),format=raw \
#		-gdb stdio -S

gdb:
	pwndbg -x debug.gdb
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	mkdir -p $(BUILD_DIR) $(BIN_DIR)