#ifndef _BTreeSloveUnderflow

#define _BTreeSloveUnderflow
#include <iostream>
#include "BTree.h"
template <typename T> //�ؼ���ɾ�������ڵ����磬�����ڵ���ת��ϲ�����
void BTree<T>::solveUnderflow (BTNodePtr<T> v) {
    if ((_order + 1) / 2 <= v -> child.size()) return; //��ǰ�ڵ㲢δ����
    BTNodePtr<T> p = v -> parent;
    if (!p) { //�ѵ����ڵ㣬û�к��ӵ�����
        if (v -> key.empty() && v -> child[0]) {
         //��������Ϊ������v�Ѳ����ؼ��룬ȴ�У�Ψһ�ģ��ǿպ��ӣ���
            _root = v -> child[0];
            _root -> parent = nullptr; //����ڵ�ɱ�����
            v -> child[0] = nullptr;
            release(v); //���������ö�������
        } //�����߶Ƚ���һ��
        return;
    }
    int r = 0;
    while (p -> child[r] != v) {
        r++;
    }
   //ȷ��v��p�ĵ�r�����ӡ�����ʱv���ܲ����ؼ��룬�ʲ���ͨ���ؼ������
   //���⣬��ʵ���˺���ָ����е���֮��Ҳ��ֱ�ӵ���Vector::find()��λ
   // ���1�������ֵܽ�ؼ���
    if (0 < r) { //��v����p�ĵ�һ�����ӣ���
        BTNodePtr<T> ls = p -> child[r - 1]; //���ֵܱش���
        if (( _order + 1 ) / 2 < ls -> child.size()) { //�����ֵ��㹻���֡�����
            v -> key.insert (v -> key.begin(), p -> key[r - 1]); //p���һ���ؼ����v����Ϊ��С�ؼ��룩
            p -> key[r - 1] = ls -> key.back(); //ls�����ؼ���ת��p
            ls -> key.pop_back();
            v -> child.insert(v -> child.begin(), ls -> child.back());
            ls -> child.pop_back();
            //ͬʱls�����Ҳຢ�ӹ��̸�v
            if (v -> child[0]) {
                v -> child[0] -> parent = v; //��Ϊv������ຢ��
            }
            return; //���ˣ�ͨ����������ɵ�ǰ�㣨�Լ����в㣩�����紦��
        }

    } //���ˣ����ֵ�ҪôΪ�գ�Ҫô̫���ݡ�
// ���2�������ֵܽ�ؼ���
    if (p -> child.size() - 1 > r) { //��v����p�����һ�����ӣ���
        BTNodePtr<T> rs = p -> child[r + 1]; //���ֵܱش���
        if ((_order + 1) / 2 < rs -> child.size()) { //�����ֵ��㹻���֡�����
            v -> key.insert(v -> key.end(), p -> key[r]); //p���һ���ؼ����v����Ϊ���ؼ��룩
            p -> key[r] = rs -> key[0]; //rs����С�ؼ���ת��p
            rs -> key.erase(rs -> key.begin());
            v -> child.insert(v -> child.end(), rs -> child[0]);
            rs -> child.erase(rs -> child.begin());
            //ͬʱrs������ຢ�ӹ��̸�v
            if (v -> child.back()) { //��Ϊv�����Ҳຢ��
                v -> child.back() -> parent = v;
            }
            return; //���ˣ�ͨ����������ɵ�ǰ�㣨�Լ����в㣩�����紦��
        }
    } //���ˣ����ֵ�ҪôΪ�գ�Ҫô̫���ݡ�
// ���3�������ֵ�ҪôΪ�գ���������ͬʱ����Ҫô��̫���ݡ������ϲ�
    if (0 < r) { //�����ֵܺϲ�
        BTNodePtr<T> ls = p -> child[r - 1]; //���ֵܱش���
        ls -> key.insert(ls -> key.end(), p -> key.back());
        p -> key.pop_back();
        p -> child.erase(p -> child.begin() + r);
        //p�ĵ�r - 1���ؼ���ת��ls��v������p�ĵ�r������
        ls -> child.push_back(v -> child[0]);
        v -> child.erase(v -> child.begin());
        if (ls -> child.back()){ //v������ຢ�ӹ��̸�ls�����Ҳຢ��
            ls -> child.back() -> parent = ls;
        }
        while (!v -> key.empty()) { //vʣ��Ĺؼ���ͺ��ӣ�����ת��ls
            ls -> key.push_back(v -> key[0]);
            v -> key.erase(v -> key.begin());
            ls -> child.push_back(v -> child[0]);
            v -> child.erase(v -> child.begin());
            if (ls -> child.back()) {
                ls -> child.back() -> parent = ls;
            }
        }
        release(v); //�ͷ�v
    } else { //�����ֵܺϲ�
        BTNodePtr<T> rs = p -> child[r + 1]; //���ֵܱش���
        rs -> key.insert(rs -> key.begin(), p -> key[r]);
        p -> key.erase(p -> key.begin() + r);
        p -> child.erase(p -> child.begin() + r);
        //p�ĵ�r���ؼ���ת��rs��v������p�ĵ�r������
        rs -> child.insert(rs -> child.begin(), v -> child.back());
        v -> child.pop_back();
        if (rs -> child[0]) {
            rs -> child[0] -> parent = rs; //v�����Ҳຢ�ӹ��̸�rs������ຢ��
        }
        while (!v -> key.empty()) { //vʣ��Ĺؼ���ͺ��ӣ�����ת��rs
            rs -> key.insert(rs -> key.begin(), v -> key.back());
            v -> key.pop_back();
            rs -> child.insert(rs -> child.begin(), v -> child.back());
            v -> child.pop_back();
            if (rs -> child[0]) {
                rs -> child[0] -> parent = rs;
            }
        }
        release(v); //�ͷ�v
    }
    solveUnderflow(p); //����һ�㣬���б�Ҫ��������ѡ�������ݹ�O(logn)��
    return;
}

#endif
