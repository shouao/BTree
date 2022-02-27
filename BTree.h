#ifndef _BTree

#define _BTree

#include "BTreeNode.h"
bool pr = false;
template <typename T>
class BTree {
    private:
        int _size; //��ŵĹؼ�������
        int _order; //B-���Ľ״�
        BTNodePtr<T> _root; //���ڵ�
        BTNodePtr<T> _hot; //BTree::search()�����ʵķǿգ��������գ��Ľڵ�λ��
        void solveOverflow (BTNodePtr<T>); //����������֮��ķ��Ѵ���
        void solveUnderflow (BTNodePtr<T>); //��ɾ��������֮��ĺϲ�����
        int searchInVector(const std::vector<T>&, const T&);
        void release(BTNodePtr<T> root) {
            if (root) {
                for (BTNodePtr<T> p : root -> child) {
                    release(p);
                }
            }
            delete root;
        }
    public:
        BTree (int order = 3) : _order (order), _size (0) {
            _root = new BTNode<T>();
        }
        ~BTree() {
            if (_root) release (_root);
        }

        int const order() {
            return _order;
        } //�״�
        int const size() {
            return _size;
        } //��ģ
        BTNodePtr<T>& root() {
            return _root;
        } //����
        bool empty() const {
            return !_root;
        } //�п�
        BTNodePtr<T> search (const T& e); //����
        bool insert (const T& e); //����
        bool remove (const T& e); //ɾ��
};

#endif
