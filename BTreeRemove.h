#ifndef _BTreeRemove

#define _BTreeRemove

#include "BTree.h"

template <typename T>
bool BTree<T>::remove ( const T& e ) { //从BTree树中删除关键码e
   BTNodePtr<T> v = search (e);
   if (!v) return false; //确认目标节点存在
   int pos = searchInVector(v -> key, e); //确定目标关键码在节点v中的秩（由上，肯定合法）
   if (v -> child[0]) { //若v非叶子，则e的后继必属于某叶节点
      BTNodePtr<T> u = v -> child[pos + 1]; //在右子树中一直向左，即可
      while (u -> child[0]) {
            u = u -> child[0]; //找出e的后继
      }
      v -> key[pos] = u -> key[0];
      v = u;
      pos = 0; //将e与e的后继之交换位置
   } //至此，v必然位于最底层，且其中第pos个关键码就是待删除者
   v -> key.erase(v -> key.begin() + pos);
   v -> child.erase(v -> child.begin() + pos + 1);
   _size--; //删除e，以及其下两个外部节点之一
   solveUnderflow (v); //如有必要，需做旋转或合并
   return true;
}

#endif
