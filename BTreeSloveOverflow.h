#ifndef _BTreeSloveOverflow

#define _BTreeSloveOverflow

#include "BTree.h"

template <typename T> //关键码插入后若节点上溢，则做节点分裂处理
void BTree<T>::solveOverflow (BTNodePtr<T> v) {
    if (_order >= v -> child.size()) {
        return; //当前节点并未上溢
    }
    int s = _order / 2; //轴点（此时应有_order = key.size() = child.size() - 1）
    BTNodePtr<T> u = new BTNode<T>(); //注意：新节点已有一个空孩子
    for (int j = 0; j < _order - s - 1; j++) { //v右侧_order-s-1个孩子及关键码分裂为右侧节点u
        u -> child.insert(u -> child.begin() + j, v -> child[s + 1]); //逐个移动效率低
        u -> key.insert(u -> key.begin() + j, v -> key[s + 1]); //此策略可改进
        v -> child.erase(v -> child.begin() + s + 1);
        v -> key.erase(v -> key.begin() + s + 1);
    }
    u -> child[_order - s - 1] = v -> child[s + 1]; //移动v最靠右的孩子
    v -> child.erase(v -> child.begin() + s + 1);
    if (u -> child[0] ) {//若u的孩子们非空，则
        for (int j = 0; j < _order - s; j++ ) {//令它们的父节点统一
            u -> child[j] -> parent = u; //指向u
        }
    }
    BTNodePtr<T> p = v -> parent; //v当前的父节点p
    if (!p) {
        _root = p = new BTNode<T>();
        p->child[0] = v;
        v->parent = p;
    } //若p空则创建之
    int r = 1 + searchInVector(p -> key, v -> key[0]); //p中指向v的指针的秩
    p -> key.insert(p -> key.begin() + r, v -> key[s]); //轴点关键码上升
    v -> key.erase(v -> key.begin() + s);
    p -> child.insert(p -> child.begin() + r + 1, u);
    u -> parent = p; //新节点u与父节点p互联
    solveOverflow (p); //上升一层，如有必要则继续分裂——至多递归O(logn)层
}

#endif
