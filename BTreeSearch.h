#ifndef _BTreeSearch

#define _BTreeSearch

#include "BTree.h"
#include "searchInVector.h"
#include <iostream>
template <typename T>
BTNodePtr<T> BTree<T>::search (const T& e) { //在B-树中查找关键码e
    BTNodePtr<T> v = _root;
    _hot = nullptr; //从根节点出发逐层查找
    while (v) {
        int pos = searchInVector(v -> key, e);
        //在当前节点中，找到不大于e的最大关键码
        if (pos != -1 && e == v -> key[pos]) {
            return v; //成功：在当前节点中命中目标关键码
        }
        _hot = v;
        v = v -> child[pos + 1];
        //否则，转入对应子树、维护hot
    } //这里在向量内是二分查找，但对通常的_order可直接顺序查找
    return nullptr; //失败
}
#endif
