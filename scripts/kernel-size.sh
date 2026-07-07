FILE_SIZE=$(stat -c %s "bin/kernel.bin")

if [ "$FILE_SIZE" -eq 0 ]; then
    echo "empty kernel"
    exit 1
fi

SECTORS=$(((FILE_SIZE + 511) / 512))


sed -i "s/[0-9]\+[[:space:]]*;[[:space:]]*<KERNEL_SIZE>/$FILE_SIZE ; <KERNEL_SIZE>/" arch/${1}/boot.asm

sed -i "s/[0-9]\+[[:space:]]*;[[:space:]]*<SECTORS_LEN>/$SECTORS ; <SECTORS_LEN>/" arch/${1}/boot.asm
echo $SECTORS