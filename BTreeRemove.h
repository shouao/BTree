#ifndef _BTreeRemove

#define _BTreeRemove

#include "BTree.h"

template <typename T>
bool BTree<T>::remove ( const T& e ) { //��BTree����ɾ���ؼ���e
   BTNodePtr<T> v = search (e);
   if (!v) return false; //ȷ��Ŀ��ڵ����
   int pos = searchInVector(v -> key, e); //ȷ��Ŀ��ؼ����ڽڵ�v�е��ȣ����ϣ��϶��Ϸ���
   if (v -> child[0]) { //��v��Ҷ�ӣ���e�ĺ�̱�����ĳҶ�ڵ�
      BTNodePtr<T> u = v -> child[pos + 1]; //����������һֱ���󣬼���
      while (u -> child[0]) {
            u = u -> child[0]; //�ҳ�e�ĺ��
      }
      v -> key[pos] = u -> key[0];
      v = u;
      pos = 0; //��e��e�ĺ��֮����λ��
   } //���ˣ�v��Ȼλ����ײ㣬�����е�pos���ؼ�����Ǵ�ɾ����
   v -> key.erase(v -> key.begin() + pos);
   v -> child.erase(v -> child.begin() + pos + 1);
   _size--; //ɾ��e���Լ����������ⲿ�ڵ�֮һ
   solveUnderflow (v); //���б�Ҫ��������ת��ϲ�
   return true;
}

#endif
