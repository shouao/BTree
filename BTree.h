#ifndef _BTree

#define _BTree

#include "BTreeNode.h"
bool pr = false;
template <typename T>
class BTree {
    private:
        int _size; //存放的关键码总数
        int _order; //B-树的阶次
        BTNodePtr<T> _root; //根节点
        BTNodePtr<T> _hot; //BTree::search()最后访问的非空（除非树空）的节点位置
        void solveOverflow (BTNodePtr<T>); //因插入而上溢之后的分裂处理
        void solveUnderflow (BTNodePtr<T>); //因删除而下溢之后的合并处理
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
        } //阶次
        int const size() {
            return _size;
        } //规模
        BTNodePtr<T>& root() {
            return _root;
        } //树根
        bool empty() const {
            return !_root;
        } //判空
        BTNodePtr<T> search (const T& e); //查找
        bool insert (const T& e); //插入
        bool remove (const T& e); //删除
};

#endif
