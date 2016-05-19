#include <common.h>
#include <tty0.h>

void _static_assert_func(const char* name, const char* file, unsigned int line, const char* func)
{
    printk("\x1b\x0c""assertion \"%s\" failed: file \"%s\""
            ", line %u, function: %s", name, file, line, func);
    while(1);
}
