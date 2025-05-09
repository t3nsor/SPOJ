// 2025-05-08
// Solution using FHQ treap. A real PITA to debug. Maintaining the treap
// structure is the easy part; we need to put `propagate` in the right places.
// This is the slowest accepted solution on SPOJ, and times out on DMOJ.
#include <algorithm>
#include <initializer_list>
#include <random>
#include <stdio.h>
using namespace std;
unsigned myrand() {
    static mt19937 gen(3828311353U);
    static uniform_int_distribution<unsigned> dist;
    return dist(gen);
}
struct Value {
    int size = 1;
    int sum;
    int maxsum;
    int maxpresum;
    int maxsufsum;
    short val;
    short lazy = 0;  // values 1000 ... 3000 represent themselves minus 2000
    bool rev = false;
};
struct node {
    node* left = nullptr;
    node* right = nullptr;
    unsigned prio;
    Value v;
    node(int val) : prio(myrand()) {
        v.sum = v.maxsum = v.val = val;
        v.maxpresum = v.maxsufsum = max(0, int(v.val));
    }
    ~node() { delete left; delete right; }
    void propagate() {
        if (v.rev) {
            swap(v.maxpresum, v.maxsufsum);
            swap(left, right);
            if (left) left->v.rev ^= 1;
            if (right) right->v.rev ^= 1;
            v.rev = false;
        }
        if (v.lazy) {
            const int val = v.lazy - 2000;
            v.val = val;
            v.sum = v.size * val;
            v.maxsum = max(val, v.sum);
            v.maxpresum = v.maxsufsum = max(0, v.sum);
            if (left) left->v.lazy = v.lazy;
            if (right) right->v.lazy = v.lazy;
            v.lazy = 0;
        }
    }
    void update() {
        if (left) left->propagate();
        if (right) right->propagate();
        if (left) {
            if (right) {
                v.size = left->v.size + 1 + right->v.size;
                v.sum = left->v.sum + v.val + right->v.sum;
                v.maxsum = max({
                  left->v.maxsum,
                  int(v.val),
                  right->v.maxsum,
                  left->v.maxsufsum + v.val,
                  v.val + right->v.maxpresum,
                  left->v.maxsufsum + v.val + right->v.maxpresum
                });
                v.maxpresum = max({left->v.maxpresum,
                                   left->v.sum + v.val,
                                   left->v.sum + v.val + right->v.maxpresum});
                v.maxsufsum = max({left->v.maxsufsum + v.val + right->v.sum,
                                   v.val + right->v.sum,
                                   right->v.maxsufsum});
            } else {
                v.size = left->v.size + 1;
                v.sum = left->v.sum + v.val;
                v.maxsum = max({left->v.maxsum,
                                int(v.val),
                                left->v.maxsufsum + v.val});
                v.maxpresum = max(left->v.maxpresum, v.sum);
                v.maxsufsum = max({0, left->v.maxsufsum + v.val, int(v.val)});
            }
        } else if (right) {
            v.size = 1 + right->v.size;
            v.sum = v.val + right->v.sum;
            v.maxsum = max({int(v.val),
                            v.val + right->v.maxpresum,
                            right->v.maxsum});
            v.maxpresum = max({0, int(v.val), v.val + right->v.maxpresum});
            v.maxsufsum = max(v.sum, right->v.maxsufsum);
        } else {
            v.size = 1;
            v.sum = v.maxsum = v.val;
            v.maxpresum = v.maxsufsum = max(int(v.val), 0);
        }
    }
};
using pnode = node*;
// split tree `t` into left subtree `l` and right subtree `r`, such that `l`
// contains the leftmost `ofs` elements
void split(pnode t, pnode& l, pnode& r, int ofs) {
    if (!t) { l = r = nullptr; return; }
    t->propagate();
    const int lsize = t->left ? t->left->v.size : 0;
    if (ofs <= lsize) {
        split(t->left, l, t->left, ofs);
        r = t;
    } else {
        split(t->right, t->right, r, ofs - 1 - lsize);
        l = t;
    }
    t->update();
}
// join trees `l` and `r` into `t`
void join(pnode& t, pnode l, pnode r) {
    if (!l || !r) { t = l ? l : r; return; }
    if (l->prio < r->prio) {
        r->propagate();
        join(r->left, l, r->left);
        t = r;
    } else {
        l->propagate();
        join(l->right, l->right, r);
        t = l;
    }
    t->update();
}
void do_testcase() {
    int N, M; scanf("%d %d", &N, &M);
    pnode t = nullptr;
    for (int i = 0; i < N; i++) {
        int x; scanf("%d", &x);
        join(t, t, new node(x));
    }
    for (int i = 0; i < M; i++) {
        char c;
        do { c = getchar(); } while (c <= 32);
        if (c == 'I') {
            int pos, len;
            scanf("NSERT %d %d", &pos, &len);
            pnode l, r; split(t, l, r, pos);
            for (int j = 0; j < len; j++) {
                int x; scanf("%d", &x);
                join(l, l, new node(x));
            }
            join(t, l, r);
        } else if (c == 'D') {
            int pos, len; scanf("ELETE %d %d", &pos, &len);
            pnode l, m, r;
            split(t, l, m, pos - 1);
            split(m, m, r, len);
            delete m;
            join(t, l, r);
        } else if (c == 'M') {
            getchar();
            if (getchar() == 'K') {
                int pos, len, val;
                scanf("E-SAME %d %d %d", &pos, &len, &val);
                if (len == 0) continue;
                pnode l, m, r;
                split(t, l, m, pos - 1);
                split(m, m, r, len);
                m->v.lazy = 2000 + val;
                join(r, m, r);
                join(t, l, r);
            } else {
                scanf("-SUM");
                t->propagate();
                printf("%d\n", t->v.maxsum);
            }
        } else if (c == 'R') {
            int pos, len; scanf("EVERSE %d %d", &pos, &len);
            if (len == 0) continue;
            pnode l, m, r;
            split(t, l, m, pos - 1);
            split(m, m, r, len);
            m->v.rev ^= 1;
            join(r, m, r);
            join(t, l, r);
        } else /* c == 'G' */ {
            int pos, len; scanf("ET-SUM %d %d", &pos, &len);
            if (len == 0) { puts("0"); continue; }
            pnode l, m, r;
            split(t, l, m, pos - 1);
            split(m, m, r, len);
            m->propagate();
            printf("%d\n", m->v.sum);
            join(r, m, r);
            join(t, l, r);
        }
    }
    delete t;
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
