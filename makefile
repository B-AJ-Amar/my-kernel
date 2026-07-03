.PHONY: all build docker-build build-local build-kernel build-boot build-boot-elf change-own concatinate run debug clean

CFLAGS = -g -ffreestanding -m32 -Iinclude -nostdlib
all: clean build run

build: docker-build



docker-build:
	docker run --rm \
		-v "$(PWD):/workspace" \
		-w /workspace \
		kernel-builder \
		make build-local

build-local: clean build-kernel build-boot build-boot-elf concatinate


concatinate:
	cat bin/boot.bin bin/kernel.bin > bin/disk.img


build-kernel:
	i686-elf-gcc $(CFLAGS) \
		-c kernel/kernel.c \
		-o bin/kernel.o

	i686-elf-ld \
		-T kernel/linker.ld \
		bin/kernel.o \
		-o bin/kernel.elf

	i686-elf-objcopy \
		-O binary \
		bin/kernel.elf \
		bin/kernel.bin

build-boot:
	./scripts/kernel-size.sh
	nasm -f bin boot/boot.asm -o bin/boot.bin

build-boot-elf:
	nasm -f elf32 -DELF_BUILD boot/boot.asm -o bin/boot.o


run:
	qemu-system-x86_64 -drive file=bin/disk.img,format=raw

debug:
	qemu-system-x86_64 -drive file=bin/disk.img,format=raw -S -gdb tcp::1234
# 	qemu-system-x86_64 -drive file=bin/disk.img,format=raw -gdb stdio -S

clean:
	mkdir -p ./bin/
	rm -f ./bin/*