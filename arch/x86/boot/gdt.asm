gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0
    gdt_code_seg:
        dw 0xffff
        dw 0x0
        db 0x0
        db 10011010b
        db 11001111b
        db 0x0
    gdt_data_seg:
        dw 0xffff
        dw 0x0
        db 0x0
        db 10010010b
        db 11001111b
        db 0x0
gdt_end:

gdt_decriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start