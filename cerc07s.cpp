// 2026-04-22
// FHQ treap makes this problem easy.  We augment each node with a field storing
// the minimum value of all keys in its subtree.  That way, we can always "find"
// the node that needs to be moved into position in each iteration.  (Note that
// the official analysis suggests a different data structure, which gives an
// O(N^1.5) algorithm.  But at least one of the official solution
// implementations does use a tree-based approach.)
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
namespace my { unsigned int random() {
    static mt19937 engine(597254518U);
    static uniform_int_distribution<unsigned> dist;
    return dist(engine);
} }
struct item {
    int k;
    int cnt = 1;
    unsigned p = my::random();
    int min;
    bool rev = false;
    item *l = nullptr, *r = nullptr;
    item(int k) : k(k), min(k) {}
    ~item() { delete l; delete r; }
};
typedef item* pitem;
int cnt(pitem t) { return t ? t->cnt : 0; }
void upd(pitem t) {
    if (t) {
        t->cnt = 1 + cnt(t->l) + cnt(t->r);
        t->min = t->k;
        if (t->l) t->min = min(t->min, t->l->min);
        if (t->r) t->min = min(t->min, t->r->min);
    }
}
void prop(pitem t) {
    if (t->rev) {
        t->rev = false;
        swap(t->l, t->r);
        if (t->l) t->l->rev = !t->l->rev;
        if (t->r) t->r->rev = !t->r->rev;
    }
}
void merge(pitem & t, pitem l, pitem r) {
    if (!l || !r) {
        t = l ? l : r;
    } else if (l->p > r->p) {
        prop(l);
        merge(l->r, l->r, r);
        t = l;
    } else {
        prop(r);
        merge(r->l, l, r->l);
        t = r;
    }
    upd(t);
}
void split(pitem t, pitem& l, pitem& root, pitem& r) {
    if (!t) {
        l = root = r = nullptr;
        return;
    }
    prop(t);
    l = t->l;
    r = t->r;
    root = t;
    root->l = root->r = nullptr;
    root->min = root->k;
}
void do_testcase(int N) {
    vector<pair<int, int>> v;
    for (int i = 0; i < N; i++) {
        int x; cin >> x;
        v.emplace_back(x, i);
    }
    sort(v.begin(), v.end());
    vector<int> v2(N);
    for (int i = 0; i < N; i++) {
        v2[v[i].second] = i;
    }
    // v2 contains values 0, ..., N - 1 exactly once
    pitem tree = nullptr;
    for (int i = 0; i < N; i++) {
        pitem node = new item(v2[i]);
        merge(tree, tree, node);
    }
    for (int i = 0; i < N; i++) {
        vector<pitem> ls;
        vector<pitem> rs;
        pitem cur = tree;
        pitem l, root, r;
        int rank = 0;
        for (;;) {
            split(cur, l, root, r);
            if (root->k == i) {
                ls.push_back(l);
                rs.push_back(r);
                rank += cnt(l);
                break;
            } else if (l && l->min == i) {
                cur = l;
                rs.push_back(r);
                rs.push_back(root);
            } else {
                cur = r;
                ls.push_back(l);
                ls.push_back(root);
                rank += cnt(l) + 1;
            }
        }
        delete cur;
        if (i > 0) cout << ' ';
        cout << i + rank + 1;
        tree = nullptr;
        for (int j = 0; j < ls.size(); j++) {
            merge(tree, tree, ls[j]);
        }
        if (tree) tree->rev = !tree->rev;
        for (int j = rs.size() - 1; j >= 0; j--) {
            merge(tree, tree, rs[j]);
        }
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int N; cin >> N;
        if (N == 0) break;
        do_testcase(N);
    }
}
