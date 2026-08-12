
E820_MAP_ADDR      equ 0x8000
E820_COUNT_ADDR    equ E820_MAP_ADDR
E820_ENTRIES_ADDR  equ E820_MAP_ADDR + 4

E820_MAX_ENTRIES   equ 128
E820_ENTRY_SIZE    equ 24
SMAP_SIGNATURE    equ 0x534D4150


; http://www.uruk.org/orig-grub/mem64mb.html
detect_memory:
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov word [E820_COUNT_ADDR], 0 ; reset count

    mov bp, E820_ENTRIES_ADDR 

    xor ebx, ebx

.next:
    ; check if reach the max entries
    cmp word [E820_COUNT_ADDR], E820_MAX_ENTRIES
    jae .too_many

    mov di, bp ; destination buffer

    mov eax, 0xE820
    mov edx, SMAP_SIGNATURE 
    mov ecx, E820_ENTRY_SIZE

    int 0x15

    jc .failed ; in case of carry flag

    cmp eax, SMAP_SIGNATURE
    jne .failed

    cmp ecx, 20 ; ecx: returned bytes len (it should be at least 20)
    jb .failed ; >

    inc word [E820_COUNT_ADDR]


    test ebx, ebx 
    jnz .next ; check if ebx is 0

    mov ax, [E820_COUNT_ADDR]

    ret

.failed:
    clc ;reset carry flag
    jmp $

.too_many:
    jmp $





