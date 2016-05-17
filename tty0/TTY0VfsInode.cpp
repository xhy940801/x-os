#include <tty0/TTY0VfsInode.h>

#include <common.h>

void TTY0VfsInode::flushCursor()
{
    _addr[_pos].flags = _info.flags;
    _outb(0x3d4, 0x0f);
    _outb(0x3d5, _pos & 0xff);
    _outb(0x3d4, 0x0e);
    _outb(0x3d5, (_pos >> 8) & 0xff);
}

ssize_t TTY0VfsInode::write(const uint8_t* buf, size_t len, FdInfo* info)
{
    const char* data = reinterpret_cast<const char*>(buf);
    for(size_t i = 0; i < len; ++i)
    {
        if(_info.state == TTY0State::SETTING)
        {
            _info.flags = buf[i];
            _info.state = TTY0State::NORMAL;
            continue;
        }
        char c = data[i];
        if(c == 27)
        {
            _info.state = TTY0State::SETTING;
            continue;
        }
        if(c == '\b')
        {
            if(_pos == 0)
                continue;
            --_pos;
            _addr[_pos].data = 0;
            continue;
        }
        if(c == '\0')
            continue;

        if(c == '\n')
            _pos = (_pos / 80 + 1) * 80;
        else if(c == '\r')
            _pos = _pos / 80 * 80;
        else if(c == '\t')
            _pos = (_pos / 4 + 1) * 4;
        else if(c == '\f')
        {
            memset(_addr, 0, 80 * 25 * sizeof(_addr[0]));
            _pos = 0;
        }
        else if(c == '\v')
            _pos += 80;
        else
        {
            _addr[_pos].c = c;
            _addr[_pos].flags = _info.flags;
            ++_pos;
        }

        while(_pos >= 80 * 25)
        {
            memmove(_addr, _addr + 80, sizeof(_addr[0]) * 80 * 24);
            memset(_addr + 80 * 24, 0, sizeof(_addr[0]) * 80);
            _pos -= 80;
        }
    }
    flushCursor();
    return len;
}

void TTY0VfsInode::close()
{
}
