#pragma once

#include <cstdint>
#include <cstddef>

#include <type_traits>

template <typename T = uint32_t>
class Bitmap
{
    T* _map;
public:
    Bitmap()
    {
    }

    Bitmap(T* map)
        : _map(map)
    {
        static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value, "T must be a unsigned integral");
    }

    void setMap(T* map)
    {
        _map = map;
    }

    void set(size_t pos)
    {
        _map[pos / (sizeof(T) * 8)] |= ((T) 1) << (pos % (sizeof(T) * 8));
    }

    void clear(size_t pos)
    {
        _map[pos / (sizeof(T) * 8)] &= ~(((T) 1) << (pos % (sizeof(T) * 8)));
    }

    void flip(size_t pos)
    {
        _map[pos / (sizeof(T) * 8)] ^= ((T) 1) << (pos % (sizeof(T) * 8));
    }

    bool value(size_t pos)
    {
        return _map[pos / (sizeof(T) * 8)] & ((T) 1) << (pos % (sizeof(T) * 8));
    }
};