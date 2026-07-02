.PHONY: all build docker-build build-local build-kernel build-boot run debug clean

all: clean build run


build: docker-build

docker-build:
	docker run --rm \
		-v "$(PWD):/workspace" \
		-w /workspace \
		kernel-builder \
		make build-local

build-local: clean build-kernel build-boot


build-kernel:
	i686-elf-gcc -ffreestanding -m32 -nostdlib -T kernel/linker.ld kernel/kernel.c -Wl,--oformat=binary -o bin/kernel.bin

build-boot:
	./scripts/kernel-size.sh
	nasm -f bin boot/boot.asm -o bin/boot.bin


run:
	qemu-system-x86_64 --hda ./bin/boot.bin

debug:
	qemu-system-x86_64 --hda ./bin/boot.bin -S -gdb tcp::1234

clean:
	rm -f ./bin/boot.bin