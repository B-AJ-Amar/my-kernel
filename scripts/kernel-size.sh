

FILE_SIZE=$(stat -c %s "bin/kernel.bin")

if [ "$FILE_SIZE" -eq 0 ]; then
    echo "empty kernel"
    exit 1
fi

SECTORS=$(((FILE_SIZE + 511) / 512))

KERNEL_SIZE_D=$((FILE_SIZE / 4))


sed -i "s/[0-9]\+[[:space:]]*;[[:space:]]*<KERNEL_SIZE>/$KERNEL_SIZE_D ; <KERNEL_SIZE>/" boot/boot.asm

sed -i "s/[0-9]\+[[:space:]]*;[[:space:]]*<SECTORS_LEN>/$SECTORS ; <SECTORS_LEN>/" boot/boot.asm
echo $SECTORS