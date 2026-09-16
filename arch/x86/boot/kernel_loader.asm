
; ? i got a problem withloading so i did this quick fix and it works for now
; TODO: make 2 stage boot, and (real mode (load) ->  p mode (copy) -> r mode ...)
CHUNK_SECTORS equ 10
load_kernel:
    mov word [kernel_sectors_left], KERNEL_SECTORS
    mov dword [kernel_lba], 1
    mov word [kernel_destination_offset], 0x0010

.load_chunk:
    cmp word [kernel_sectors_left], 0
    je .done

    mov ax, [kernel_sectors_left]
    cmp ax, CHUNK_SECTORS
    jbe .count_ready
    mov ax, CHUNK_SECTORS

.count_ready:
    mov [disk_address_packet.sector_count], ax
    mov word [disk_address_packet.buffer_offset], KERNEL_ADDR_RM
    mov word [disk_address_packet.buffer_segment], 0
    mov eax, [kernel_lba]
    mov [disk_address_packet.lba_low], eax
    mov dword [disk_address_packet.lba_high], 0

    mov si, disk_address_packet
    mov dl, [boot_drive]
    mov ah, 0x42
    int 0x13
    jc disk_error

    ; Copy from 0000:1000 to the current destination segment.
    mov ax, 0xffff
    mov es, ax
    mov si, KERNEL_ADDR_RM
    mov di, [kernel_destination_offset]
    mov cx, [disk_address_packet.sector_count]
    shl cx, 8
    rep movsw

    xor eax, eax
    mov ax, [disk_address_packet.sector_count]
    sub [kernel_sectors_left], ax
    add [kernel_lba], eax
    shl ax, 9
    add [kernel_destination_offset], ax
    jmp .load_chunk

.done:
    ret

disk_address_packet:
    db 0x10
    db 0
.sector_count:
    dw 0
.buffer_offset:
    dw 0
.buffer_segment:
    dw 0
.lba_low:
    dd 0
.lba_high:
    dd 0

kernel_sectors_left:
    dw 0
kernel_lba:
    dd 0
kernel_destination_offset:
    dw 0

disk_error:
    cli
    hlt
    jmp $