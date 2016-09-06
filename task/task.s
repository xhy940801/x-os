global scheduleesp
global _setRet

scheduleesp:
    mov eax, [esp + 4]
    mov edx, [esp + 8]

    mov [edx], esp
    mov esp, eax
    ret

_setRet:
    mov eax, [esp + 4]
    mov dword [eax], 1
    ret