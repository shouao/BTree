#ifndef _BTreeInsert

#define _BTreeInsert

#include "BTree.h"

template <typename T>
bool BTree<T>::insert (const T& e) { //���ؼ���e����B����
   BTNodePtr<T> v = search(e);
   if (v) return false; //ȷ��Ŀ��ڵ����
   int pos = searchInVector(_hot -> key, e); //�ڽڵ�_hot������ؼ��������в��Һ��ʵĲ���λ��
   _hot -> key.insert(_hot -> key.begin() + pos + 1, e); //���¹ؼ��������Ӧ��λ��
   _hot -> child.insert(_hot -> child.begin() + pos + 2, nullptr); //����һ��������ָ��
   _size++; //����ȫ����ģ
   solveOverflow (_hot); //���б�Ҫ����������
   return true; //����ɹ�
}

#endif
