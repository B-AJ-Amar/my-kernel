
; read from disk https://en.wikipedia.org/wiki/INT_13H
load_kernel:
    mov bx, KERNEL_ADDR_RM
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    mov dl, [boot_drive]
    mov ah, 0x02
    mov al, KERNEL_SECTORS
    int 0x13

    jc disk_error
    cmp ah, 0
    jne disk_error
    ret

disk_error:
    mov ax, 0xB800
    mov es, ax
    mov di, 0
    mov ax, 0x0444
    mov [es:di], ax
    jmp $