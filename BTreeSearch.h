#ifndef _BTreeSearch

#define _BTreeSearch

#include "BTree.h"
#include "searchInVector.h"
#include <iostream>
template <typename T>
BTNodePtr<T> BTree<T>::search (const T& e) { //��B-���в��ҹؼ���e
    BTNodePtr<T> v = _root;
    _hot = nullptr; //�Ӹ��ڵ����������
    while (v) {
        int pos = searchInVector(v -> key, e);
        //�ڵ�ǰ�ڵ��У��ҵ�������e�����ؼ���
        if (pos != -1 && e == v -> key[pos]) {
            return v; //�ɹ����ڵ�ǰ�ڵ�������Ŀ��ؼ���
        }
        _hot = v;
        v = v -> child[pos + 1];
        //����ת���Ӧ������ά��hot
    } //�������������Ƕ��ֲ��ң�����ͨ����_order��ֱ��˳�����
    return nullptr; //ʧ��
}
#endif
