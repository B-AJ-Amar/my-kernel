[bits 32]

extern task_wrapper
CODE_SEG equ 0x08
DATA_SEG equ 0x10
DEFAULT_EFLAGS equ 0x202

global switch_context
global switch_context_interrupt
global task_stub

; ------------------------------------------------------------
; void switch_context(uint32_t *old_esp, uint32_t *new_esp)
; Cooperative switch (not used by the pre-emptive scheduler).
; ------------------------------------------------------------
switch_context:
    pushfd
    pushad
    mov eax, [esp + 40]        ; old_esp
    mov [eax], esp             ; save current ESP
    mov eax, [esp + 44]        ; new_esp
    mov esp, [eax]             ; load new ESP
    popad
    popfd
    ret

; ------------------------------------------------------------
; void switch_context_interrupt(uint32_t *old_esp, uint32_t *new_esp)
; Called from an IRQ handler.  The stack already contains the
; complete interrupt frame.  Saves current ESP, loads new ESP,
; restores the frame, and iretd's to the new task.
; ------------------------------------------------------------
switch_context_interrupt:
    mov edx, esp               ; save current ESP
    mov eax, [edx + 4]         ; old_esp
    test eax, eax
    jz .skip_save
    mov [eax], esp             ; *old_esp = current ESP
.skip_save:
    mov eax, [edx + 8]         ; new_esp
    mov esp, [eax]             ; load new task's ESP

    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8                 ; skip int_no and err_code
    iretd

; ------------------------------------------------------------
; Initial task stub
; EAX = entry function, EBX = params pointer
; ------------------------------------------------------------
task_stub:
    push ebx
    push eax
    call task_wrapper
    ; task_wrapper never returns
    cli
    hlt
    jmp $