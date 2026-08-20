[bits 16]
%ifdef ELF_BUILD
section .text
global start
%else
org 0x7C00
%endif


CODE_SEG equ gdt_code_seg - gdt_start
DATA_SEG equ gdt_data_seg - gdt_start
STACK_PTR equ 0x8000

KERNEL_ADDR_RM equ 0x1000
KERNEL_ADDR_PM equ 0x100000

; this will be updated by the build script
KERNEL_SIZE equ 17376
KERNEL_SECTORS equ 34

BOOT_INFO_ADDR equ 0x7000

start:
    mov [boot_drive], dl
    mov ax, 0x2401
    int 0x15
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov bp, STACK_PTR
    mov sp, bp
    mov ah, 0x0
    mov al, 0x3
    int 0x10

    call detect_memory
    call load_kernel

    cli
    lgdt [gdt_decriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode
    hlt

boot_drive:
    db 0

%include "kernel_loader.asm"
%include "gdt.asm"
%include "detect_memory.asm"

msg:
    db "Protected Mode Enabled Successfuly ... ", 0

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov ebx, 0xB8000
    mov esi, msg
    mov ah, 0x0f

    call print
    
    call boot_info
    
    mov esi, KERNEL_ADDR_RM
    mov edi, KERNEL_ADDR_PM
    mov ecx, KERNEL_SIZE
    rep movsb
kernel_switch:
    jmp CODE_SEG:KERNEL_ADDR_PM
    hlt

boot_info:
    mov dword [BOOT_INFO_ADDR], KERNEL_ADDR_PM
    mov dword [BOOT_INFO_ADDR + 4], KERNEL_SIZE
    movzx eax, word [E820_COUNT_ADDR]
    mov dword [BOOT_INFO_ADDR + 8], eax
    mov dword [BOOT_INFO_ADDR + 12], E820_ENTRIES_ADDR
    mov dword [BOOT_INFO_ADDR + 16], E820_MAX_ENTRIES
    mov dword [BOOT_INFO_ADDR + 20], E820_ENTRY_SIZE
    

print:
    mov al, [esi]
    cmp al, 0
    je .print_end
    mov ah, 0x0F
    mov [ebx], ax
    inc esi
    add ebx, 2
    jmp print
    .print_end:
        ret

times 510-($-$$) db 0
dw 0xAA55