#include <iostream>
#include <algorithm>
#include <vector>

template <typename T>
struct BTNode;

template <typename T>
using BTNodePtr = BTNode<T>*;

template <typename T>
struct BTNode {
    BTNodePtr<T> parent;
    std::vector<T> key;
    std::vector<BTNodePtr<T>> child;
    BTNode() : parent(nullptr), child(1, nullptr) {}
    BTNode (T e, BTNodePtr<T> lc = nullptr, BTNodePtr<T> rc = nullptr) : parent(nullptr), key(1, e), child{lc, rc} {
        if (lc) {
            lc -> parent = this;
        }
        if (rc) {
            rc -> parent = this;
        }
    }
};

template <typename T>
class BTree {
    private:
        int _size; //存放的关键码总数
        int _order; //B-树的阶次
        BTNodePtr<T> _root; //根节点
        BTNodePtr<T> _hot; //BTree::search()最后访问的非空（除非树空）的节点位置

        void solveOverflow (BTNodePtr<T> v) {
            if (_order >= v -> child.size()) {
                return; //当前节点并未上溢
            }
            int s = _order / 2; //轴点（此时应有_order = key.size() = child.size() - 1）
            BTNodePtr<T> u = new BTNode<T>(); //注意：新节点已有一个空孩子
            for (int j = 0; j < _order - s - 1; j++) { //v右侧_order-s-1个孩子及关键码分裂为右侧节点u
                u -> child.insert(u -> child.begin() + j, v -> child[s + 1]); //逐个移动效率低
                u -> key.insert(u -> key.begin() + j, v -> key[s + 1]); //此策略可改进
                v -> child.erase(v -> child.begin() + s + 1);
                v -> key.erase(v -> key.begin() + s + 1);
            }
            u -> child[_order - s - 1] = v -> child[s + 1]; //移动v最靠右的孩子
            v -> child.erase(v -> child.begin() + s + 1);
            if (u -> child[0] ) {//若u的孩子们非空，则
                for (int j = 0; j < _order - s; j++ ) {//令它们的父节点统一
                    u -> child[j] -> parent = u; //指向u
                }
            }
            BTNodePtr<T> p = v -> parent; //v当前的父节点p
            if (!p) {
                _root = p = new BTNode<T>();
                p->child[0] = v;
                v->parent = p;
            } //若p空则创建之
            int r = 1 + searchInVector(p -> key, v -> key[0]); //p中指向v的指针的秩
            p -> key.insert(p -> key.begin() + r, v -> key[s]); //轴点关键码上升
            v -> key.erase(v -> key.begin() + s);
            p -> child.insert(p -> child.begin() + r + 1, u);
            u -> parent = p; //新节点u与父节点p互联
            solveOverflow (p); //上升一层，如有必要则继续分裂——至多递归O(logn)层
        }

        void solveUnderflow (BTNodePtr<T> v) {
            if ((_order + 1) / 2 <= v -> child.size()) return; //当前节点并未下溢
            BTNodePtr<T> p = v -> parent;
            if (!p) { //已到根节点，没有孩子的下限
                if (v -> key.empty() && v -> child[0]) {
                 //但倘若作为树根的v已不含关键码，却有（唯一的）非空孩子，则
                    _root = v -> child[0];
                    _root -> parent = nullptr; //这个节点可被跳过
                    v -> child[0] = nullptr;
                    release(v); //并因不再有用而被销毁
                } //整树高度降低一层
                return;
            }
            int r = 0;
            while (p -> child[r] != v) {
                r++;
            }
           //确定v是p的第r个孩子——此时v可能不含关键码，故不能通过关键码查找
           //另外，在实现了孩子指针的判等器之后，也可直接调用Vector::find()定位
           // 情况1：向左兄弟借关键码
            if (0 < r) { //若v不是p的第一个孩子，则
                BTNodePtr<T> ls = p->child[r - 1]; //左兄弟必存在
                if (( _order + 1 ) / 2 < ls->child.size()) { //若该兄弟足够“胖”，则
                    v -> key.insert (v -> key.begin(), p -> key[r - 1]); //p借出一个关键码给v（作为最小关键码）
                    p -> key[r - 1] = ls -> key.back(); //ls的最大关键码转入p
                    ls -> key.pop_back();
                    v -> child.insert(v -> child.begin(), ls -> child.back());
                    ls -> child.pop_back();
                    //同时ls的最右侧孩子过继给v
                    if (v -> child[0]) {
                        v -> child[0] -> parent = v; //作为v的最左侧孩子
                    }
                    return; //至此，通过右旋已完成当前层（以及所有层）的下溢处理
                }
            } //至此，左兄弟要么为空，要么太“瘦”
        // 情况2：向右兄弟借关键码
            if (p -> child.size() - 1 > r) { //若v不是p的最后一个孩子，则
                BTNodePtr<T> rs = p -> child[r + 1]; //右兄弟必存在
                if ((_order + 1) / 2 < rs -> child.size()) { //若该兄弟足够“胖”，则
                    v -> key.insert(v -> key.end(), p -> key[r]); //p借出一个关键码给v（作为最大关键码）
                    p -> key[r] = rs -> key[0]; //rs的最小关键码转入p
                    rs -> key.erase(rs -> key.begin());
                    v -> child.insert(v -> child.end(), rs -> child[0]);
                    rs -> child.erase(rs -> child.begin());
                    //同时rs的最左侧孩子过继给v
                    if (v -> child.back()) { //作为v的最右侧孩子
                        v -> child.back() -> parent = v;
                    }
                    return; //至此，通过左旋已完成当前层（以及所有层）的下溢处理
                }
            } //至此，右兄弟要么为空，要么太“瘦”
        // 情况3：左、右兄弟要么为空（但不可能同时），要么都太“瘦”——合并
            if (0 < r) { //与左兄弟合并
                BTNodePtr<T> ls = p -> child[r - 1]; //左兄弟必存在
                ls -> key.insert(ls -> key.end(), p -> key.back());
                p -> key.pop_back();
                p -> child.erase(p -> child.begin() + r);
                //p的第r - 1个关键码转入ls，v不再是p的第r个孩子
                ls -> child.insert(ls -> child.end(), v -> child[0]);
                v -> child.erase(v -> child.begin());
                if (ls -> child.back()){ //v的最左侧孩子过继给ls做最右侧孩子
                    ls -> child.back() -> parent = ls;
                }
                while (!v -> key.empty()) { //v剩余的关键码和孩子，依次转入ls
                    ls -> key.insert(ls -> key.end(), v -> key[0]);
                    v -> key.erase(v -> key.begin());
                    ls -> child.insert(ls -> child.end(), v -> child[0]);
                    v -> child.erase(v -> child.begin());
                    if (ls -> child.back()) {
                        ls -> child.back() -> parent = ls;
                    }
                }
                release(v); //释放v
            } else { //与右兄弟合并
                BTNodePtr<T> rs = p -> child[r + 1]; //右兄弟必存在
                rs -> key.insert(rs -> key.begin(), p -> key[r]);
                p -> key.erase(p -> key.begin() + r);
                p -> child.erase(p -> child.begin() + r);
                //p的第r个关键码转入rs，v不再是p的第r个孩子
                rs -> child.insert(rs -> child.begin(), v -> child.back());
                v -> child.pop_back();
                if (rs -> child[0]) {
                    rs -> child[0] -> parent = rs; //v的最右侧孩子过继给rs做最左侧孩子
                }
                while (!v -> key.empty()) { //v剩余的关键码和孩子，依次转入rs
                    rs -> key.insert(rs -> key.begin(), v -> key.back());
                    v -> key.pop_back();
                    rs -> child.insert(rs -> child.begin(), v -> child.back());
                    v -> child.pop_back();
                    if (rs -> child[0]) {
                        rs -> child[0] -> parent = rs;
                    }
                }
                release(v); //释放v
            }
            solveUnderflow(p); //上升一层，如有必要则继续分裂——至多递归O(logn)层
            return;
        }

        int searchInVector(const std::vector<T>& vec, const T& e) {
            auto it = upper_bound(vec.begin(), vec.end(), e);
            if (it == vec.begin()) {
                return -1;
            }
            --it;
            return it - vec.begin();
        }

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

        BTNodePtr<T> search (const T& e){ //在B-树中查找关键码e
            BTNodePtr<T> v = _root;
            _hot = nullptr; //从根节点出发逐层查找
            while (v) {
                int pos = searchInVector(v -> key, e);
                //在当前节点中，找到不大于e的最大关键码

                if (pos != v -> key.size() && pos != -1 && (e == v -> key[pos])) {
                    return v; //成功：在当前节点中命中目标关键码
                }
                _hot = v;
                v = v -> child[pos + 1];
                //否则，转入对应子树、维护hot
            } //这里在向量内是二分查找，但对通常的_order可直接顺序查找
            return nullptr; //失败
        }

        bool insert (const T& e) { //将关键码e插入B树中
           BTNodePtr<T> v = search(e);
           if (v) return false; //确认目标节点存在
           int pos = searchInVector(_hot -> key, e); //在节点_hot的有序关键码向量中查找合适的插入位置
           _hot -> key.insert(_hot -> key.begin() + pos + 1, e); //将新关键码插至对应的位置
           _hot -> child.insert(_hot -> child.begin() + pos + 2, nullptr); //创建一个空子树指针
           _size++; //更新全树规模
           solveOverflow (_hot); //如有必要，需做分裂
           return true; //插入成功
        }

        bool remove (const T& e) { //从BTree树中删除关键码e
           BTNodePtr<T> v = search (e);
           if (v) return false; //确认目标节点已经存在
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
};

int main() {

    return 0;
}
