all: clean build run

build:
	nasm -f bin ./boot/boot.asm -o ./bin/boot.bin

run:
	qemu-system-x86_64 --hda ./bin/boot.bin

debug:
	qemu-system-x86_64 \
    --hda ./bin/boot.bin \
    -S -gdb tcp::1234

clean:
	rm -f ./bin/boot.bin