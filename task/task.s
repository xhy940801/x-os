global _task_swapesp

_task_swapesp:
    mov eax, [esp + 4]
    mov edx, [esp + 8]

    mov [edx], esp
    mov esp, eax
    ret