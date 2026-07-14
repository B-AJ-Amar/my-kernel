set architecture i386:x86-64

target remote :1234

file build/kernel.elf

add-symbol-file build/boot.o 0x7c00

# Breakpoints
break kernel_switch
break kernel

break enable_interrupts
break vga_init
break console_set
break ps2_init
break printf

# Optional pwndbg settings
#context
#layout asm
#layout regs
lay next
lay next
lay next
lay next
lay next
lay next

continue