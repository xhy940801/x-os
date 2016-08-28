#pragma once

#include <type_traits>
#include <cstddef>

#include "common/RBTree.h"

struct _CommLess
{
    template <typename T>
    bool operator () (const T& v1, const T& v2)
    {
        return v1 < v2;
    }
};

template <size_t N = 0>
class MapNode : public RBTree::NodeBase
{
};

template <typename T, size_t N = 0, typename Less = _CommLess>
class Map : protected RBTree::TreeBase
{
    Less less;
    size_t count;
    typedef MapNode<N> NodeType;

public:
    class Iterator : public RBTree::IteratorBase
    {
        T* pointer()
        {
            return static_cast<T*>(static_cast<MapNode<N>*>(node));
        }

        const T* pointer() const
        {
            return static_cast<T*>(static_cast<MapNode<N>*>(node));
        }

    public:
        Iterator(RBTree::NodeBase* _node) : RBTree::IteratorBase(_node) {}

        Iterator& operator ++ ()
        {
            increment();
            return *this;
        }

        Iterator operator ++ (int)
        {
            Iterator it = *this;
            increment();
            return it;
        }

        Iterator& operator -- ()
        {
            decrement();
            return *this;
        }

        Iterator operator -- (int)
        {
            Iterator it = *this;
            decrement();
            return it;
        }

        T* operator -> ()
        {
            return pointer();
        }

        const T* operator -> () const
        {
            return pointer();
        }

        T& operator * ()
        {
            return *pointer();
        }

        const T& operator * () const
        {
            return *pointer();
        }

        friend bool operator == (const Iterator& it1, const Iterator& it2)
        {
            return RBTree::IteratorBase::equal(it1, it2);
        }

        friend bool operator != (const Iterator& it1, const Iterator& it2)
        {
            return !RBTree::IteratorBase::equal(it1, it2);
        }
    };

public:
    Map(Less _less = Less()) : less(_less), count(0)
    {
        static_assert(std::is_base_of<NodeType, T>::value, "std::is_base_of<NodeType, T>::value must be true");
    }

    void insert(T* node)
    {
        NodeType* y = static_cast<NodeType*>(&head);
        NodeType* x = static_cast<NodeType*>(root());
        NodeType* n = node;

        while (x != nullptr)
        {
            y = x;
            x = less(*node, *static_cast<T*>(x)) ? static_cast<NodeType*>(left(x)) : static_cast<NodeType*>(right(x));
        }

        if (y == &head)
        {
            root() = n;
            leftmost() = n;
            rightmost() = n;
        }
        else if (less(*node, *static_cast<T*>(y)))
        {
            left(y) = n;
            if (y == leftmost())
                leftmost() = n;
        }
        else
        {
            right(y) = n;
            if (y == rightmost())
                rightmost() = n;
        }

        parent(n) = y;
        left(n) = nullptr;
        right(n) = nullptr;

        rebalance(n);
        ++count;
    }

    void remove(T* node)
    {
        NodeType* n = node;
        RBTree::TreeBase::remove(n);
        --count;
    }

    size_t size() const
    {
        return count;
    }

    Iterator begin()
    {
        return Iterator(leftmost());
    }

    Iterator end()
    {
        return Iterator(&head);
    }
};