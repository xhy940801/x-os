extern doHDInterrupt

global on_hd_interrupt

on_hd_interrupt:
    push ds
    push es
    push fs
    push gs

    push eax
    push ebx
    push ecx
    push edx
    cld

    mov eax, 0x10
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    call doHDInterrupt

    pop edx
    pop ecx
    pop ebx
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    iret