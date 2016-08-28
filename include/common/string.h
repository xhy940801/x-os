#pragma once

#include <cstddef>

#include <asm.h>

inline void memset(void* dst, int val, size_t len)
{
    _cld();
    _stosb(dst, val, len);
}

inline void memcpy(void* dst, void* src, size_t len)
{
    _cld();
    _movsb(dst, src, len);
}

inline void memmove(void* dst, void* src, size_t len)
{
    if(dst < src)
    {
        _cld();
        _movsb(dst, src, len);
    }
    else
    {
        _std();
        _movsb(reinterpret_cast<char*>(dst) + len - 1, reinterpret_cast<char*>(src) + len - 1, len);
    }
}

inline size_t strlen(const char* str)
{
    size_t i;
    for(i = 0; str[i] != '\0'; ++i);
    return i;
}