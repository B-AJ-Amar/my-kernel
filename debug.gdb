set architecture i386:x86-64

target remote :1234

file build/kernel.elf

add-symbol-file build/boot.o 0x7c00

# Breakpoints

break kernel_switch
break kernel
break shed_init
break switch_context
break push_int_context
break task_wrapper
break task_create
break schedule

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