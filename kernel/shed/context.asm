bits 32

global switch_context

switch_context:
    pushad

    mov eax, [esp + 36]      ; current
    mov [eax], esp

    mov eax, [esp + 40]      ; next
    mov esp, [eax]

    popad
    ret