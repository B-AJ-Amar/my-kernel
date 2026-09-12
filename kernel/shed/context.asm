[bits 32]

extern task_wrapper
CODE_SEG equ 0x08
DATA_SEG equ 0x10
DEFAULT_EFLAGS equ 0x202

global switch_to_task
global task_stub

; ------------------------------------------------------------
; void switch_to_task(uint32_t new_sp)
;
; new_sp must point to an interrupt frame of the form
; (low -> high):
;   EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX,   <- pushad
;   int_no, err_code,
;   EIP, CS, EFLAGS
;
; The caller passes the address of the first dword (EDI).
; We load it into ESP, undo the frame, and iretd.
; This routine never returns to the caller.
; ------------------------------------------------------------
switch_to_task:
    mov eax, [esp + 4]      ; new_sp
    mov esp, eax

    popad
    add esp, 8              ; discard int_no, err_code
    iretd


task_stub:
    push ebx
    push eax
    call task_wrapper
    ; task_wrapper never returns (but in case it does it just halts)
    cli
    hlt
    jmp $