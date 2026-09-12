[bits 32]
global kernel_entry
extern kernel

extern __stack_bottom
extern __stack_top

section .start

kernel_entry:
    ; Set up a safe stack for the kernel
    mov ax, 0x10          ; Data segment selector (same as DATA_SEG set by boot.asm)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, __stack_top

    call kernel

    cli
    hlt