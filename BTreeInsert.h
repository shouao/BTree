#ifndef _BTreeInsert

#define _BTreeInsert

#include "BTree.h"

template <typename T>
bool BTree<T>::insert (const T& e) { //将关键码e插入B树中
   BTNodePtr<T> v = search(e);
   if (v) return false; //确认目标节点存在
   int pos = searchInVector(_hot -> key, e); //在节点_hot的有序关键码向量中查找合适的插入位置
   _hot -> key.insert(_hot -> key.begin() + pos + 1, e); //将新关键码插至对应的位置
   _hot -> child.insert(_hot -> child.begin() + pos + 2, nullptr); //创建一个空子树指针
   _size++; //更新全树规模
   solveOverflow (_hot); //如有必要，需做分裂
   return true; //插入成功
}

#endif
