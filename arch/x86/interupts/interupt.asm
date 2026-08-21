
; ? from: https://wiki.osdev.org/Exceptions and https://wiki.osdev.org/Interrupt_Descriptor_Table

; ? in case of irs dont need an error code i will push 0 to avoid problems

extern interupt_handler

global isr0 ;division by zero
isr0:
    push 0x0
    push 0x0
    jmp inetupt_common
    

global isr1;debug
isr1:
    push 0x0
    push 0x1
    jmp inetupt_common


global isr2;non maskable interrupt
isr2:
    push 0x0
    push 0x2
    jmp inetupt_common


global isr3;breakpoint
isr3:
    push 0x0
    push 0x3
    jmp inetupt_common


global isr4;overflow
isr4:
    push 0x0
    push 0x4
    jmp inetupt_common


global isr5;bound range exceeded
isr5:
    push 0x0
    push 0x5
    jmp inetupt_common


global isr6;invalid opcode
isr6:
    push 0x0
    push 0x6
    jmp inetupt_common


global isr7;device not available
isr7:
    push 0x0
    push 0x7
    jmp inetupt_common


global isr8;double fault
isr8:
    push 0x8
    jmp inetupt_common


global isr9;coprocessor segment overrun
isr9:
    push 0x0
    push 0x9
    jmp inetupt_common


global isr10;invalid tss
isr10:
    push 0xA
    jmp inetupt_common


global isr11;segment not present
isr11:
    push 0xB
    jmp inetupt_common


global isr12;stack segment fault
isr12:
    push 0xC
    jmp inetupt_common


global isr13;general protection fault
isr13:
    push 0xD
    jmp inetupt_common


global isr14;page fault
isr14:
    push 0xE
    jmp inetupt_common


global isr15;reserved
isr15:
    push 0x0
    push 0xF
    jmp inetupt_common


global isr16;x87 floating point exception
isr16:
    push 0x0
    push 0x10
    jmp inetupt_common


global isr17;alignment check
isr17:
    push 0x11
    jmp inetupt_common


global isr18;machine check
isr18:
    push 0x0
    push 0x12
    jmp inetupt_common


global isr19;SIMD floating point exception
isr19:
    push 0x0
    push 0x13
    jmp inetupt_common


global isr20;Virtualization Exception
isr20:
    push 0x0
    push 0x14
    jmp inetupt_common


global isr21;Control Protection Exception
isr21:
    push 0x15
    jmp inetupt_common

; ? reserved =================

global isr22
isr22:
    push 0x0
    push 0x16
    jmp inetupt_common


global isr23
isr23:
    push 0x0
    push 0x17
    jmp inetupt_common


global isr24
isr24:
    push 0x0
    push 0x18
    jmp inetupt_common


global isr25
isr25:
    push 0x0
    push 0x19
    jmp inetupt_common


global isr26
isr26:
    push 0x0
    push 0x1A
    jmp inetupt_common


global isr27
isr27:
    push 0x0
    push 0x1B
    jmp inetupt_common

;? ==============================


global isr28; Hypervisor Injection Exception
isr28:
    push 0x0
    push 0x1C
    jmp inetupt_common


global isr29;VMM Communication Exception
isr29:
    push 0x1D
    jmp inetupt_common


global isr30;Security Exception
isr30:
    push 0x1E
    jmp inetupt_common


global isr31;reserved
isr31:
    push 0x0
    push 0x1F
    jmp inetupt_common

; ? Hardware interrupts ============================== 
global irq0
irq0:
    push 0x0
    push 0x20
    jmp inetupt_common

global irq1
irq1:
    push 0x0
    push 0x21
    jmp inetupt_common

global irq2
irq2:
    push 0x0
    push 0x22
    jmp inetupt_common

global irq3
irq3:
    push 0x0
    push 0x23
    jmp inetupt_common

global irq4
irq4:
    push 0x0
    push 0x24
    jmp inetupt_common

global irq5
irq5:
    push 0x0
    push 0x25
    jmp inetupt_common

global irq6
irq6:
    push 0x0
    push 0x26
    jmp inetupt_common

global irq7
irq7:
    push 0x0
    push 0x27
    jmp inetupt_common

global irq8
irq8:
    push 0x0
    push 0x28
    jmp inetupt_common

global irq9
irq9:
    push 0x0
    push 0x29
    jmp inetupt_common

global irq10
irq10:
    push 0x0
    push 0x2A
    jmp inetupt_common

global irq11
irq11:
    push 0x0
    push 0x2B
    jmp inetupt_common

global irq12
irq12:
    push 0x0
    push 0x2C
    jmp inetupt_common

global irq13
irq13:
    push 0x0
    push 0x2D
    jmp inetupt_common

global irq14
irq14:
    push 0x0
    push 0x2E
    jmp inetupt_common

global irq15
irq15:
    push 0x0
    push 0x2F
    jmp inetupt_common



inetupt_common:
    pushad ;push : EAX, ECX, EDX, EBX,ESP, EBP, ESI, EDI
    cld

    push esp         ; ? push the vlue of the stackpointer ( pointer to registers :) )
    call interupt_handler
    add esp, 4       ; ? rm esp
    
    popad                    
    
    add esp, 8       ; ? rm int_no and err_code
    

    iretd             