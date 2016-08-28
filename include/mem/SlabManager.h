#pragma once

#include <cstdint>
#include <cstddef>

#include "common.h"

#include "list.h"

#include "mem/commondefs.h"

class SlabManager
{
    struct Slab : public ListNode<Slab, 0>
    {
    };

    struct Page
    {
        size_t usedcount;
        Slab slabs[0];
    };

private:
    List<Slab, 0> slots[mem::MAXSLOT / mem::MINSLOT];

private:
    Page* getPageOfSlab(Slab* slab)
    {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(slab);
        return reinterpret_cast<Page*>(ptr / mem::PAGESIZE * mem::PAGESIZE);
    }

    Slab* nextSlab(Slab* slab, size_t level)
    {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(slab);
        ptr += (level + 1) * mem::MINSLOT;
        assert(ptr % mem::PAGESIZE != 0);
        return reinterpret_cast<Slab*>(ptr);
    }

    bool mallocOnePage(size_t level);

public:
    void* alloc(size_t level);
    void free(void* buffer, size_t level);

    static constexpr size_t objectLevel(size_t size)
    {
        return (size + mem::MINSLOT - 1) / mem::MINSLOT;
    }

    template <typename T, typename... Args>
    T* newObject(Args... args)
    {
        static_assert(sizeof(T) <= mem::MAXSLOT, "sizeof(T) > mem::MAXSLOT");
        T* ptr = reinterpret_cast<T*>(alloc(objectLevel(sizeof(T))));
        return ptr == nullptr ? nullptr : new (ptr) T(args...);
    }

    template <typename T>
    void deleteObject(T* ptr)
    {
        ptr->~T();
        free(ptr, objectLevel(sizeof(T)));
    }
};
