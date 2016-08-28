#pragma once

#include <utility>

namespace RBTree
{
    class NodeBase;

    enum class Color
    {
        RED, BLACK
    };

    class NodeBase
    {
        Color color;
        NodeBase* parent;
        NodeBase* left;
        NodeBase* right;

        friend class TreeBase;
        friend class IteratorBase;
        friend bool operator == (const NodeBase& node1, const NodeBase& node2);

    public:
        NodeBase() = default;
        NodeBase(Color c, NodeBase* p, NodeBase* l, NodeBase* r) : color(c), parent(p), left(l), right(r) {}
    };

    inline bool operator == (const NodeBase& node1, const NodeBase& node2)
    {
        return &node1 == &node2;
    }

    class TreeBase
    {
        typedef NodeBase* NodeBasePtr;
    protected:
        NodeBase head;

        NodeBasePtr& root() { return head.parent; }
        NodeBasePtr& leftmost() { return head.left; }
        NodeBasePtr& rightmost() { return head.right; }

        const NodeBasePtr& root() const { return head.parent; }
        const NodeBasePtr& leftmost() const { return head.left; }
        const NodeBasePtr& rightmost() const { return head.right; }

        static NodeBasePtr& parent(NodeBasePtr ptr) { return ptr->parent; }
        static NodeBasePtr& left(NodeBasePtr ptr) { return ptr->left; }
        static NodeBasePtr& right(NodeBasePtr ptr) { return ptr->right; }
        static Color& color(NodeBasePtr ptr) { return ptr->color; }

    public:
        TreeBase()
        {
            head.color = Color::RED;
            root() = nullptr;
            leftmost() = &head;
            rightmost() = &head;
        }

        NodeBase* minimum(NodeBase* x)
        {
            while (x->left != nullptr)
                x = x->left;
            return x;
        }

        NodeBase* maximum(NodeBase* x)
        {
            while (x->right != nullptr)
                x = x->right;
            return x;
        }

        void rotateLeft(NodeBase* x)
        {
            NodeBase* y = x->right;
            x->right = y->left;
            if (y->left != nullptr)
                y->left->parent = x;
            y->parent = x->parent;
            if (x == head.parent)
                head.parent = y;
            else if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
            y->left = x;
            x->parent = y;
        }

        void rotateRight(NodeBase* x)
        {
            NodeBase* y = x->left;
            x->left = y->right;
            if (y->right != nullptr)
                y->right->parent = x;
            y->parent = x->parent;
            if (x == head.parent)
                head.parent = y;
            else if (x == x->parent->right)
                x->parent->right = y;
            else
                x->parent->left = y;
            y->right = x;
            x->parent = y;
        }

        void rebalance(NodeBase* x)
        {
            x->color = Color::RED;
            while (x != root() && x->parent->color == Color::RED)
            {
                if (x->parent == x->parent->parent->left)
                {
                    NodeBase* y = x->parent->parent->right;
                    if (y != nullptr && y->color == Color::RED)
                    {
                        x->parent->color = Color::BLACK;
                        y->color = Color::BLACK;
                        x->parent->parent->color = Color::RED;
                        x = x->parent->parent;
                    }
                    else
                    {
                        if (x == x->parent->right)
                        {
                            x = x->parent;
                            rotateLeft(x);
                        }
                        x->parent->color = Color::BLACK;
                        x->parent->parent->color = Color::RED;
                        rotateRight(x->parent->parent);
                    }
                }
                else
                {
                    NodeBase* y = x->parent->parent->left;
                    if (y != nullptr && y->color == Color::RED)
                    {
                        x->parent->color = Color::BLACK;
                        y->color = Color::BLACK;
                        x->parent->parent->color = Color::RED;
                        x = x->parent->parent;
                    }
                    else
                    {
                        if (x == x->parent->left)
                        {
                            x = x->parent;
                            rotateRight(x);
                        }
                        x->parent->color = Color::BLACK;
                        x->parent->parent->color = Color::RED;
                        rotateLeft(x->parent->parent);
                    }
                }
            }
            root()->color = Color::BLACK;
        }

        void remove(NodeBase* z)
        {
            NodeBase* y = z;
            NodeBase* x = nullptr;
            NodeBase* xParent = nullptr;

            if (y->left == nullptr)
                x = y->right;
            else if (y->right == nullptr)
                x = y->left;
            else
            {
                y = y->right;
                while (y->left != nullptr)
                    y = y->left;
                x = y->right;
            }

            if (y != z)
            {
                z->left->parent = y;
                y->left = z->left;

                if (y != z->right)
                {
                    xParent = y->parent;
                    if (x != nullptr)
                        x->parent = y->parent;
                    y->parent->left = x;
                    y->right = z->right;
                    z->right->parent = y;
                }
                else
                    xParent = y;

                if (z == root())
                    root() = y;
                else if (z->parent->left == z)
                    z->parent->left = y;
                else
                    z->parent->right = y;

                y->parent = z->parent;
                std::swap(y->color, z->color);
                y = z;
            }
            else
            {
                xParent = y->parent;
                if (x != nullptr)
                    x->parent = y->parent;
                if (z == root())
                    root() = x;
                else if (z->parent->left == z)
                    z->parent->left = x;
                else
                    z->parent->right = x;

                if (z == leftmost())
                    if (z->right == nullptr)
                        leftmost() = z->parent;
                    else
                        leftmost() = minimum(x);

                if (z == rightmost())
                    if (z->left == nullptr)
                        rightmost() = z->parent;
                    else
                        rightmost() = maximum(x);
            }

            if (y->color != Color::RED)
            {
                while (x != root() && (x == 0 || x->color == Color::BLACK))
                {
                    if (x == xParent->left)
                    {
                        NodeBase* w = xParent->right;
                        if (w->color == Color::RED)
                        {
                            w->color = Color::BLACK;
                            xParent->color = Color::RED;
                            rotateLeft(xParent);
                            w = xParent->right;
                        }

                        if ((w->left == nullptr || w->left->color == Color::BLACK) &&
                            (w->right == nullptr || w->right->color == Color::BLACK))
                        {
                            w->color = Color::RED;
                            x = xParent;
                            xParent = x->parent;
                        }
                        else
                        {
                            if (w->right == nullptr || w->right->color == Color::BLACK)
                            {
                                w->left->color = Color::BLACK;
                                w->color = Color::RED;
                                rotateRight(w);
                                w = xParent->right;
                            }

                            w->color = xParent->color;
                            xParent->color = Color::BLACK;
                            w->right->color = Color::BLACK;
                            rotateLeft(xParent);
                            break;
                        }
                    }
                    else
                    {
                        NodeBase* w = xParent->left;
                        if (w->color == Color::RED)
                        {
                            w->color = Color::BLACK;
                            xParent->color = Color::RED;
                            rotateRight(xParent);
                            w = xParent->left;
                        }

                        if ((w->right == nullptr || w->right->color == Color::BLACK) &&
                            (w->left == nullptr || w->left->color == Color::BLACK))
                        {
                            w->color = Color::RED;
                            x = xParent;
                            xParent = xParent->parent;
                        }
                        else
                        {
                            if (w->left == nullptr || w->left->color == Color::BLACK)
                            {
                                w->right->color = Color::BLACK;
                                w->color = Color::RED;
                                rotateLeft(w);
                                w = xParent->left;
                            }

                            w->color = xParent->color;
                            xParent->color = Color::BLACK;
                            w->left->color = Color::BLACK;
                            rotateRight(xParent);
                            break;
                        }
                    }
                }
                if (x != nullptr)
                    x->color = Color::BLACK;
            }
        }
    };

    class IteratorBase
    {
    protected:
        NodeBase* node;

    public:
        IteratorBase(NodeBase* _node) : node(_node) {}

        void increment()
        {
            if (node->right != nullptr)
            {
                node = node->right;
                while (node->left != nullptr)
                    node = node->left;
            }
            else
            {
                NodeBase* y = node->parent;
                while (node == y->right)
                {
                    node = y;
                    y = y->parent;
                }
                if (node->right != y)
                    node = y;
            }
        }

        void decrement()
        {
            if (node->color == Color::RED && node->parent->parent == node)
                node = node->right;
            else if (node->left != nullptr)
            {
                node = node->left;
                while (node->right != nullptr)
                    node = node->right;
            }
            else
            {
                NodeBase* y = node->parent;
                while (node == y->left)
                {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }

        static bool equal(const IteratorBase& it1, const IteratorBase& it2)
        {
            return it1.node == it2.node;
        }
    };
}
