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
        int _size; //��ŵĹؼ�������
        int _order; //B-���Ľ״�
        BTNodePtr<T> _root; //���ڵ�
        BTNodePtr<T> _hot; //BTree::search()�����ʵķǿգ��������գ��Ľڵ�λ��

        void solveOverflow (BTNodePtr<T> v) {
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

        void solveUnderflow (BTNodePtr<T> v) {
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
                BTNodePtr<T> ls = p->child[r - 1]; //���ֵܱش���
                if (( _order + 1 ) / 2 < ls->child.size()) { //�����ֵ��㹻���֡�����
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
                ls -> child.insert(ls -> child.end(), v -> child[0]);
                v -> child.erase(v -> child.begin());
                if (ls -> child.back()){ //v������ຢ�ӹ��̸�ls�����Ҳຢ��
                    ls -> child.back() -> parent = ls;
                }
                while (!v -> key.empty()) { //vʣ��Ĺؼ���ͺ��ӣ�����ת��ls
                    ls -> key.insert(ls -> key.end(), v -> key[0]);
                    v -> key.erase(v -> key.begin());
                    ls -> child.insert(ls -> child.end(), v -> child[0]);
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

        BTNodePtr<T> search (const T& e){ //��B-���в��ҹؼ���e
            BTNodePtr<T> v = _root;
            _hot = nullptr; //�Ӹ��ڵ����������
            while (v) {
                int pos = searchInVector(v -> key, e);
                //�ڵ�ǰ�ڵ��У��ҵ�������e�����ؼ���

                if (pos != v -> key.size() && pos != -1 && (e == v -> key[pos])) {
                    return v; //�ɹ����ڵ�ǰ�ڵ�������Ŀ��ؼ���
                }
                _hot = v;
                v = v -> child[pos + 1];
                //����ת���Ӧ������ά��hot
            } //�������������Ƕ��ֲ��ң�����ͨ����_order��ֱ��˳�����
            return nullptr; //ʧ��
        }

        bool insert (const T& e) { //���ؼ���e����B����
           BTNodePtr<T> v = search(e);
           if (v) return false; //ȷ��Ŀ��ڵ����
           int pos = searchInVector(_hot -> key, e); //�ڽڵ�_hot������ؼ��������в��Һ��ʵĲ���λ��
           _hot -> key.insert(_hot -> key.begin() + pos + 1, e); //���¹ؼ��������Ӧ��λ��
           _hot -> child.insert(_hot -> child.begin() + pos + 2, nullptr); //����һ��������ָ��
           _size++; //����ȫ����ģ
           solveOverflow (_hot); //���б�Ҫ����������
           return true; //����ɹ�
        }

        bool remove (const T& e) { //��BTree����ɾ���ؼ���e
           BTNodePtr<T> v = search (e);
           if (v) return false; //ȷ��Ŀ��ڵ��Ѿ�����
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
};

int main() {

    return 0;
}
