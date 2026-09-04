[bits 32]

extern task_wrapper

DEFAULT_EFLAGS equ 0x202
CODE_SEG equ 0x08
DATA_SEG equ 0x10

global switch_context
switch_context:
    pushad

    mov eax, [esp + 36]      ; current
    mov [eax], esp
    
    mov eax, [esp + 40]      ; next
    mov esp, [eax]
    
    popad
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

    mov eax, [esp + 4] ; stack pointer
    .int_context:
    mov ecx, [esp + 8] ; eip
    mov [eax-4], ecx ; params
    mov ecx, [esp + 12] ; eip
    mov [eax-8], ecx ; entry
    xor ecx, ecx
    mov [eax-12], ecx ; fake return to keep the structure of the call correct, i dont think that wiill be needed anyway (the theread wll be deleted in task_exit()
    
    .thread_context:
    mov ecx, DEFAULT_EFLAGS
    mov [eax-16], ecx
    mov ecx, CODE_SEG
    mov [eax-20], ecx
    mov ecx, task_wrapper
    mov [eax-24], ecx ; eip (wrapper_function)
    ret