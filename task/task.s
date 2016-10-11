global _task_swapesp

_task_swapesp:
    mov eax, [esp + 12]
    mov edx, [esp + 16]

    mov [edx], ebp
    mov ebp, eax

    mov eax, [esp + 4]
    mov edx, [esp + 8]

    mov [edx], esp
    mov esp, eax
    ret