set architecture i386:x86-64

target remote :1234

file build/kernel.elf

add-symbol-file build/boot.o 0x7c00

# Breakpoints
break detect_memory
break detect_memory.next
break detect_memory.end
break detect_memory.failed
break detect_memory.too_many
break kernel_switch
break kernel


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