#pragma once

#include <cstdint>

#include "vfs.h"

class TTY0VfsInode : public VfsInode
{
    enum class TTY0State : uint16_t
    {
        NORMAL,
        SETTING
    };

    struct TTY0Info
    {
        uint8_t flags;
        uint8_t reverse;
        TTY0State state;
        TTY0Info() {}
        TTY0Info(uint8_t f, TTY0State s) : flags(f), state(s) {}
    };

public:
    union TTY0Character
    {
        uint16_t data;
        struct
        {
            char c;
            uint8_t flags;
        };
    };


private:
    uint32_t _pos;
    TTY0Info _info;
    TTY0Character* _addr;

private:
    void flushCursor();

public:
    TTY0VfsInode(VfsType vfstype, DriverType drivertype, VfsInode* parent, TTY0Character* addr)
        : VfsInode(vfstype, drivertype, parent), _pos(0), _info(0x07, TTY0State::NORMAL), _addr(addr)
    {
        static_assert(sizeof(_addr[0]) == 2, "sizeof(TTY0Character) must equal to 2");
    }

    ssize_t write(const uint8_t* buf, size_t len, FdInfo* info);
    void close();
};
