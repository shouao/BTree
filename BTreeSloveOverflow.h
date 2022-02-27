#ifndef _BTreeSloveOverflow

#define _BTreeSloveOverflow

#include "BTree.h"

template <typename T> //�ؼ����������ڵ����磬�����ڵ���Ѵ���
void BTree<T>::solveOverflow (BTNodePtr<T> v) {
    if (_order >= v -> child.size()) {
        return; //��ǰ�ڵ㲢δ����
    }
    int s = _order / 2; //��㣨��ʱӦ��_order = key.size() = child.size() - 1��
    BTNodePtr<T> u = new BTNode<T>(); //ע�⣺�½ڵ�����һ���պ���
    for (int j = 0; j < _order - s - 1; j++) { //v�Ҳ�_order-s-1�����Ӽ��ؼ������Ϊ�Ҳ�ڵ�u
        u -> child.insert(u -> child.begin() + j, v -> child[s + 1]); //����ƶ�Ч�ʵ�
        u -> key.insert(u -> key.begin() + j, v -> key[s + 1]); //�˲��ԿɸĽ�
        v -> child.erase(v -> child.begin() + s + 1);
        v -> key.erase(v -> key.begin() + s + 1);
    }
    u -> child[_order - s - 1] = v -> child[s + 1]; //�ƶ�v��ҵĺ���
    v -> child.erase(v -> child.begin() + s + 1);
    if (u -> child[0] ) {//��u�ĺ����Ƿǿգ���
        for (int j = 0; j < _order - s; j++ ) {//�����ǵĸ��ڵ�ͳһ
            u -> child[j] -> parent = u; //ָ��u
        }
    }
    BTNodePtr<T> p = v -> parent; //v��ǰ�ĸ��ڵ�p
    if (!p) {
        _root = p = new BTNode<T>();
        p->child[0] = v;
        v->parent = p;
    } //��p���򴴽�֮
    int r = 1 + searchInVector(p -> key, v -> key[0]); //p��ָ��v��ָ�����
    p -> key.insert(p -> key.begin() + r, v -> key[s]); //���ؼ�������
    v -> key.erase(v -> key.begin() + s);
    p -> child.insert(p -> child.begin() + r + 1, u);
    u -> parent = p; //�½ڵ�u�븸�ڵ�p����
    solveOverflow (p); //����һ�㣬���б�Ҫ��������ѡ�������ݹ�O(logn)��
}

#endif
