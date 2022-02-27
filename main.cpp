#include<bits/stdc++.h>
#include "BTreeImplementation.h"
using namespace std;

void print(BTNode<int>* root) {
    if (!root) {
        return;
    }
    for (int x : root -> key) {
        cout << x << " ";
    }
    cout << "\n";
    for (BTNode<int>* p : root -> child) {
        print(p);
    }

}
int main () {
    srand(time(NULL));
    BTree<int>tr(3);
    set<int>st;
    vector<int>I{0, 1, 2, 3, 4};
    vector<int>E{2};
    for (int w : I) {
        st.insert(w);
        tr.insert(w);
    }
     //print(tr.root());
    pr = true;
    for (int w : E) {
        st.erase(w);
        tr.remove(w);
    }

    print(tr.root());

    for (int i = 0; i < 5; ++i) {
        if (tr.search(i)) {
            cout << i << " ";
        }
    }
    cout <<"\n";
    for (int i = 0; i < 5; ++i) {
        if (st.count(i)) {
            cout << i << " ";
        }
    }

    return 0;
}
