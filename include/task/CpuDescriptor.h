#pragma once

#pragma pack(push, 1)

struct gdt_descriptor
{
    short l_limit;
    short l_base;
    char m_base;
    short attr;
    char h_base;
};


struct tss_struct
{
    uint32_t back_link;
    uint32_t esp0, ss0;
    uint32_t esp1, ss1;
    uint32_t esp2, ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t idt;
    uint32_t debug;
};

#pragma pack(pop)