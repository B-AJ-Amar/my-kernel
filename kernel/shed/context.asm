[bits 32]
extern task_wrapper
DEFAULT_EFLAGS equ 0x202
CODE_SEG equ 0x08
DATA_SEG equ 0x10

global switch_context
switch_context:
    pushfd                    ; save EFLAGS (including IF) of the outgoing task
    pushad
    mov eax, [esp + 40]       ; current  (offsets shift by 4 because of pushfd)
    mov [eax], esp

    mov eax, [esp + 44]       ; next
    mov esp, [eax]

    popad
    popfd                     ; restore EFLAGS (including IF) of the incoming task
    ret

; ? in case of the current exited
global switch_to
switch_to:
    mov eax, [esp + 4]       ; next
    mov esp, [eax]

    popad
    popfd
    ret
; ? docs: Intel® 64 and IA-32 Architectures Software Developer’s Manual, Volume 3 (3A, 3B, 3C & 3D): System Programming Guide page 199
; ESP -->
; ───────────────── (switch_context > pushad)
;  EDI              
;  ESI             
;  EBP             
;  saved ESP       
;  EBX             
;  EDX             
;  ECX             
;  EAX             
; ───────────────── (push_int_context.int_context)
;  EIP             
;  CS              
;  EFLAGS          
; ───────────────── (push_int_context.thread_context)
;  fake return     
;  entry           
;  params          
; ─────────────────


global push_int_context
push_int_context:
    mov eax, [esp + 4]    ; stack top (from task_alloc_stack)
    mov ecx, [esp + 8]    ; params
    mov edx, [esp + 12]   ; entry

    sub eax, 52            ; 8 regs(32) + eflags(4) + wrapper eip(4) + fake ret(4) + entry(4) + params(4)

    mov dword [eax + 0],  0   ; edi
    mov dword [eax + 4],  0   ; esi
    mov dword [eax + 8],  0   ; ebp
    mov dword [eax + 12], 0   ; esp (ignored by popad)
    mov dword [eax + 16], 0   ; ebx
    mov dword [eax + 20], 0   ; edx
    mov dword [eax + 24], 0   ; ecx
    mov dword [eax + 28], 0   ; eax

    mov dword [eax + 32], DEFAULT_EFLAGS   ; IF=1 for the new task, from the start

    mov dword [eax + 36], task_wrapper     ; popped as EIP by switch_context's ret
    mov dword [eax + 40], 0                ; fake return addr for task_wrapper
    mov [eax + 44], edx                     ; entry  -> task_wrapper arg1
    mov [eax + 48], ecx                     ; params -> task_wrapper arg2

    ret                                      ; eax = new sp, returned to caller