#pragma once

#define _stosb(dst, val, len) __asm__("rep stosb"::"D"(dst),"a"(val),"c"(len):"memory")
#define _movsb(dst, src, len) __asm__("rep movsb"::"D"(dst),"S"(src),"c"(len):"memory")

#define _sti() __asm__("sti")
#define _cli() __asm__("cli")
#define _std() __asm__("std")
#define _cld() __asm__("cld")
#define _outb(port, data) __asm__ volatile("outb %%al, %%dx"::"d"(port),"a"(data))
#define _outw(port, data) __asm__ volatile("outw %%ax, %%dx"::"d"(port),"a"(data))
#define _outd(port, data) __asm__ volatile("outl %%eax, %%dx"::"d"(port),"a"(data))
#define _inb(port) ({char _v; __asm__ volatile("inb %%dx, %%al":"=a"(_v):"d"(port)); _v;})
#define _inw(port) ({short _v; __asm__ volatile("inw %%dx, %%ax":"=a"(_v):"d"(port)); _v;})
#define _ind(port) ({int _v; __asm__ volatile("inl %%dx, %%eax":"=a"(_v):"d"(port)); _v;})
#define _ltr(index) __asm__("ltr %%ax"::"a"(index))
#define _lcr3(pos) __asm__("mov %%eax, %%cr3"::"a"(pos))
#define _scr3() ({unsigned int _v; __asm__("mov %%cr3, %%eax":"=a"(_v)); _v})
#define _bsf(src) ({int _v; __asm__("bsf %1, %0":"=r"(_v):"r"(src):"cc"); _v;})
#define _bsr(src) ({int _v; __asm__("bsr %1, %0":"=r"(_v):"r"(src):"cc"); _v;})
#define _scr2() ({int _v; __asm__("mov %%cr2, %0":"=g"(_v)); _v;})

#define _bsfc(src, flags)({int _v; __asm__("bsf %1, %0\nlahf":"=r"(_v),"=a"(flags):"r"(src):"cc"); _v;})
#define _bsrc(src, flags)({int _v; __asm__("bsr %1, %0\nlahf":"=r"(_v),"=a"(flags):"r"(src):"cc"); _v;})

#define _fnsave(ptr) __asm__("fnsave (%0)"::"g"(ptr):"memory")
#define _frstor(ptr) __asm__("frstor (%0)"::"g"(ptr))

#define _lesp(ptr) __asm__("mov %%esp, %0":"=g"(ptr))

#define _memorybar() __asm__ volatile("":::"memory")

#define PSW_CF 0x01
#define PSW_PF 0x04
#define PSW_AF 0x10
#define PSW_ZF 0x40
#define PSW_SF 0x80
