#include <cstddef>
#include <limits>
#include "common.h"
#include "tty0.h"

static inline void _writeToFD1(const char* str, size_t len)
{
    VfsInode* inode = tty0vfs.getRootInode(DriverType(0, MainDriverType::TTY0));
    inode->write(reinterpret_cast<const uint8_t*>(str), len, nullptr);
    inode->release();
}

static inline void _print(char c)
{
    _writeToFD1(&c, 1);
}

static inline void _print(const char* str)
{
    _writeToFD1(str, strlen(str));
}

static inline void _print(int num)
{
    if(num == std::numeric_limits<int>::min())
    {
        _print("-2147483648");
        return;
    }
    if(num < 0)
    {
        _print('-');
        num = 0 - num;
    }
    char tmp[16];
    size_t i = 0;
    do
    {
        tmp[i] = (num % 10) + '0';
        num /= 10;
        ++i;
    } while(num);
    --i;
    while(i < 16)
    {
        _print(tmp[i]);
        --i;
    }
}

static inline void _print(unsigned int num)
{
    char tmp[16];
    size_t i = 0;
    do
    {
        tmp[i] = (num % 10) + '0';
        num /= 10;
        ++i;
    } while(num);
    --i;
    while(i < 16)
    {
        _print(tmp[i]);
        --i;
    }
}

void printk(const char* fmt, ...)
{
    void* params = ((char*) &fmt) + 4;
    while(*fmt)
    {
        if(*fmt == '%')
        {
            if(fmt[1] == 'd')
                _print(*((int*) params));
            else if(fmt[1] == 'u')
                _print(*((unsigned int*) params));
            else if(fmt[1] == 'c')
                _print(*((char*) params));
            else if(fmt[1] == 's')
                _print(*((char**) params));
            else if(fmt[1] == '\0')
                break;
            params = ((char*) params) + 4;
            ++fmt;
        }
        else
            _print(*fmt);
        ++fmt;
    }
}
