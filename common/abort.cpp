#include "common.h"

void _abort(const char* file, unsigned int line, const char* func)
{
    printk("\x1b\x0c""abort! file \"%s\""
            ", line %u, function: %s", file, line, func);
    while(1);
}

extern "C"
{
    void __cxa_pure_virtual();
}

void __cxa_pure_virtual()
{
    printk("\x1b\x0c""abort! pure virtual function has been called.");
    while(1);
}