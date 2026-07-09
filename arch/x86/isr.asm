
; ? from: https://wiki.osdev.org/Exceptions and https://wiki.osdev.org/Interrupt_Descriptor_Table

; ? in case of irs dont need an error code i will push 0 to avoid problems

extern isr_handler

global isr0 ;division by zero
isr0:
    push 0x0
    push 0x0
    jmp isr_common
    

global isr1;debug
isr1:
    push 0x0
    push 0x1
    jmp isr_common


global isr2;non maskable interrupt
isr2:
    push 0x0
    push 0x2
    jmp isr_common


global isr3;breakpoint
isr3:
    push 0x0
    push 0x3
    jmp isr_common


global isr4;overflow
isr4:
    push 0x0
    push 0x4
    jmp isr_common


global isr5;bound range exceeded
isr5:
    push 0x0
    push 0x5
    jmp isr_common


global isr6;invalid opcode
isr6:
    push 0x0
    push 0x6
    jmp isr_common


global isr7;device not available
isr7:
    push 0x0
    push 0x7
    jmp isr_common


global isr8;double fault
isr8:
    push 0x8
    jmp isr_common


global isr9;coprocessor segment overrun
isr9:
    push 0x0
    push 0x9
    jmp isr_common


global isr10;invalid tss
isr10:
    push 0xA
    jmp isr_common


global isr11;segment not present
isr11:
    push 0xB
    jmp isr_common


global isr12;stack segment fault
isr12:
    push 0xC
    jmp isr_common


global isr13;general protection fault
isr13:
    push 0xD
    jmp isr_common


global isr14;page fault
isr14:
    push 0xE
    jmp isr_common


global isr15;reserved
isr15:
    push 0x0
    push 0xF
    jmp isr_common


global isr16;x87 floating point exception
isr16:
    push 0x0
    push 0x10
    jmp isr_common


global isr17;alignment check
isr17:
    push 0x11
    jmp isr_common


global isr18;machine check
isr18:
    push 0x0
    push 0x12
    jmp isr_common


global isr19;SIMD floating point exception
isr19:
    push 0x0
    push 0x13
    jmp isr_common


global isr20;Virtualization Exception
isr20:
    push 0x0
    push 0x14
    jmp isr_common


global isr21;Control Protection Exception
isr21:
    push 0x15
    jmp isr_common

; ? reserved =================

global isr22
isr22:
    push 0x0
    push 0x16
    jmp isr_common


global isr23
isr23:
    push 0x0
    push 0x17
    jmp isr_common


global isr24
isr24:
    push 0x0
    push 0x18
    jmp isr_common


global isr25
isr25:
    push 0x0
    push 0x19
    jmp isr_common


global isr26
isr26:
    push 0x0
    push 0x1A
    jmp isr_common


global isr27
isr27:
    push 0x0
    push 0x1B
    jmp isr_common

;? ==============================


global isr28; Hypervisor Injection Exception
isr28:
    push 0x0
    push 0x1C
    jmp isr_common


global isr29;VMM Communication Exception
isr29:
    push 0x1D
    jmp isr_common


global isr30;Security Exception
isr30:
    push 0x1E
    jmp isr_common


global isr31;reserved
isr31:
    push 0x0
    push 0x1F
    jmp isr_common
    
isr_common:
    pushad ;push : EAX, ECX, EDX, EBX,ESP, EBP, ESI, EDI
    cld

    push esp         ; ? push the vlue of the stackpointer ( pointer to registers :) )
    call isr_handler
    add esp, 4       ; ? rm esp
    
    popad                    
    
    add esp, 8       ; ? rm int_no and err_code
    

    iretd             