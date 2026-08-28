set architecture i386:x86-64

target remote :1234

file build/kernel.elf

add-symbol-file build/boot.o 0x7c00

# Breakpoints

break kernel_switch
break kernel
break mm_init
break pmm_init
break vmm_init


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