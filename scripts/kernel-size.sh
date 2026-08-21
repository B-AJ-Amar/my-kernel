FILE_SIZE=$(stat -c %s "bin/kernel.bin")

if [ "$FILE_SIZE" -eq 0 ]; then
    echo "empty kernel"
    exit 1
fi

SECTORS=$(((FILE_SIZE + 511) / 512))


sed -i "s/KERNEL_SIZE equ [0-9]\+/KERNEL_SIZE equ $FILE_SIZE/" arch/${1}/boot/boot.asm

sed -i "s/KERNEL_SECTORS equ [0-9]\+/KERNEL_SECTORS equ $SECTORS/" arch/${1}/boot/boot.asm
echo $SECTORS