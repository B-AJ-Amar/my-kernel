set architecture i386:x86-64

target remote :1234

file build/kernel.elf

add-symbol-file build/boot.o 0x7c00

# Breakpoints
break start_protected_mode
break print_end
break kernel_switch
break kernel
break outb
break inb
break vga_clear

# Optional pwndbg settings
#context
#layout asm
#layout regs

continue