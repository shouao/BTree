#ifndef _BTreeNode

#define _BTreeNode

#include <vector>

template <typename T>
struct BTNode;

template <typename T>
using BTNodePtr = BTNode<T>*;

template <typename T>
struct BTNode {
    BTNodePtr<T> parent;
    std::vector<T> key;
    std::vector<BTNodePtr<T>> child;
    BTNode() : parent(nullptr), child(1, nullptr) {}
    BTNode (T e, BTNodePtr<T> lc = nullptr, BTNodePtr<T> rc = nullptr) : parent(nullptr), key(1, e), child{lc, rc} {
        if (lc) {
            lc -> parent = this;
        }
        if (rc) {
            rc -> parent = this;
        }
    }
};

#endif
