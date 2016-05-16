#pragma once

#include <type_traits>

template <typename T, int N = 0>
class List;

template <typename T, int N = 0>
class ListNode
{
    friend class List<T, N>;
protected:
    T* _next;
    T* _prev;

    ListNode()
        : _next(static_cast<T*>(this)), _prev(static_cast<T*>(this))
    {
        static_assert(std::is_base_of<ListNode<T, N>, T>::value, "class ListNode<T, N> must base of T");
    }

    inline ListNode(List<T, N>& list);
public:

    void insertAfter(T& prev)
    {
        _next = prev._next;
        _prev = &prev;
        prev._next = static_cast<T*>(this);
        _next->_prev = static_cast<T*>(this);
    }

    void insertBefore(T& next)
    {
        next.insertAfter(*this);
    }

    void removeSelf()
    {
        _prev->_next = _next;
        _next->_prev = _prev;
    }

    T* next()
    {
        return _next;
    }

    T* prev()
    {
        return _prev;
    }
};

template <typename T, int N>
class List
{
    friend class ListNode<T, N>;
    ListNode<T, N> head;
public:
    void pushFront(T& node)
    {
        head.insertBefore(node);
    }

    void pushBack(T& node)
    {
        head.insertAfter(node);
    }

    bool empty()
    {
        return head.prev() == head.next();
    }
};

template <typename T, int N>
inline ListNode<T, N>::ListNode(List<T, N>& list)
{
    static_assert(std::is_base_of<ListNode<T, N>, T>::value, "class ListNode<T, N> must base of T");
    list.pushBack(*static_cast<T*>(this));
}
