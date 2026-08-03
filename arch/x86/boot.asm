[bits 16]
%ifdef ELF_BUILD
section .text
global start
%else
org 0x7C00
%endif

CODE_SEG equ gdt_code_seg - gdt_start; should be 0x8 (selector)
DATA_SEG equ gdt_data_seg - gdt_start; 0x10

KERNEL_ADDR_RM equ 0x1000
KERNEL_ADDR_PM   equ 0x100000 ;protected mod address


start:
    mov [boot_drive], dl

    ;A20
    mov ax, 0x2401
    int 0x15

    xor ax, ax
    mov es, ax
    mov ds, ax
    mov bp, 0x8000
    mov sp, bp
    ;text mode (to clear the screen)
    mov ah, 0x0
    mov al, 0x3
    int 0x10


    call load_kernel

    cli
    lgdt [gdt_decriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax ;change protected mod flag


    jmp CODE_SEG:start_protected_mode
    hlt

boot_drive:
    db 0

; read from disk https://en.wikipedia.org/wiki/INT_13H
load_kernel:
    mov bx, KERNEL_ADDR_RM;load to addr
    mov ch, 0x00;cylinder
    mov dh, 0x00;head
    mov cl, 0x02;sector
    mov dl, [boot_drive]
    mov ah, 0x02;write mode
    ;! dont remove token i will use it in the build time
    mov al, 34 ; <SECTORS_LEN>  number of sectors (i will upt this in build time)
    int 0x13

    jc disk_error
    ; jmp $
    cmp ah, 0
    jne disk_error
    ret

disk_error:
    mov ax, 0xB800
    mov es, ax

    mov di, 0

    mov ax, 0x0444      ; AH = 04 red , 44 D
    mov [es:di], ax
    jmp $

gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0
    gdt_code_seg:
        dw 0xffff    ;limit[0->15]
        dw 0x0       ;base[0->15]
        db 0x0       ;base[16->23]
        db 10011010b ;access flags
        db 11001111b ;flags + limit[16->19]
        db 0x0       ;base[24->31]
    gdt_data_seg:
        dw 0xffff
        dw 0x0
        db 0x0
        db 10010010b
        db 11001111b
        db 0x0
gdt_end:

gdt_decriptor:
    dw gdt_end - gdt_start - 1 ;size
    dd gdt_start               ;start


msg:
    db "Protected Mode Enabled Successfuly ... " , 0

[bits 32]
start_protected_mode:
    ;reset all of the registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; print for test
    mov ebx,0xB8000
    mov esi,msg
    mov ah, 0x0f

    print:
        mov al,[esi]
        cmp al,0
        je print_end
        mov ah, 0x0F
        mov [ebx], ax
        inc esi
        add ebx, 2
        jmp print
    print_end:

    mov esi, KERNEL_ADDR_RM
    mov edi, KERNEL_ADDR_PM
    ;! dont remove token i will use it in the build time
    mov ecx, 17376 ; <KERNEL_SIZE> TODO: count dwords to copy
    rep movsb
kernel_switch:
    jmp CODE_SEG:KERNEL_ADDR_PM
    hlt


times 510-($-$$) db 0

dw 0xAA55
