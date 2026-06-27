[bits 16]
[org 0x7c00]

start:
    mov si, msg
    call print
    cli
    lgdt [gdt_decriptor]
    mov si, msg2
    call print
    

end:
    jmp $
    
print:
    mov ah, 0x0E    ; write char (needed with int 0x100
    print_loop:
        mov al,[si]
        cmp al,0
        je print_end
        int 0x10 ; display interupt
        inc si
        jmp print_loop
    print_end:
        ret
    
gdt_start:
    gdt_null:
        dw 0x0000
        dw 0x0000
    gdt_code_seg:
        dw 0xffff     ;limit[0->15]
        dw 0x0000     ;base[0->15]
        db 0x00       ;base[16->23]
        dw 0b10011010 ;access flags
        dw 0b11001111 ;flags + limit[16->19]
        db 0x00       ;base[24->31]
    gdt_data_seg:
        dw 0xffff
        dw 0x0000  
        db 0x00       
        dw 0b10010010 
        dw 0b11001111
        db 0x00       
gdt_end:

gdt_decriptor:
    dw gdt_end - gdt_start - 1 ;size
    dd gdt_start               ;start


msg:
    db "Bootloader is running ...", 0x0D, 0x0A, 0

msg2:
    db "Loading Kernel ...", 0x0D, 0x0A, 0


times 510-($-$$) db 0

dw 0xAA55