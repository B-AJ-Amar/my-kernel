[bits 16]
[org 0x7c00]

CODE_SEG equ gdt_code_seg - gdt_start; should be 0x8 (selector)
DATA_SEG equ gdt_data_seg - gdt_start; 0x10

start:
    cli
    lgdt [gdt_decriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode
    
gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0
    gdt_code_seg:
        dw 0xffff     ;limit[0->15]
        dw 0x0     ;base[0->15]
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
    mov esp, 0x90000 ;get more space for the stack
    
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
    hlt


times 510-($-$$) db 0

dw 0xAA55