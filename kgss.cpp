// 2014-05-01
#include <cstdio>
#include <utility>
#include <algorithm>
using namespace std;
pair<int, int> segtree[400000];
int a[200000];
void set(int node, int val) {
    segtree[node].first = val;
    segtree[node].second = -1e9;
}
pair<int, int> combine(pair<int, int> l, pair<int, int> r) {
    return make_pair(
      max(l.first, r.first),
      max(max(l.second, r.second), l.first + r.first)
    );
}
void build_tree(int node, int l, int r) {
    if (r == l+1) {
        set(node, a[l]);
    } else {
        int m = l + (r - l + 1)/2;
        build_tree(2*node, l, m);
        build_tree(2*node+1, m, r);
        segtree[node] = combine(segtree[2*node], segtree[2*node+1]);
    }
}
void update(int node, int l, int r, int pos, int val) {
    if (r == l+1) {
        set(node, val);
    } else {
        int m = l + (r - l + 1)/2;
        if (pos < m) {
            update(2*node, l, m, pos, val);
        } else {
            update(2*node+1, m, r, pos, val);
        }
        segtree[node] = combine(segtree[2*node], segtree[2*node+1]);
    }
}
pair<int, int> query(int node, int tl, int tr, int al, int ar) {
    if (tl >= al && tr <= ar) {
        return segtree[node];
    } else {
        int m = tl + (tr - tl + 1)/2;
        if (m > al && tl < ar) {
            if (tr > al && m < ar) {
                return combine(query(2*node, tl, m, al, ar),
                               query(2*node+1, m, tr, al, ar));
            } else {
                return query(2*node, tl, m, al, ar);
            }
        } else {
            return query(2*node+1, m, tr, al, ar);
        }
    }
}
int main() {
    int N; scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", a+i);
    }
    build_tree(1, 0, N);
    int Q; scanf("%d\n", &Q);
    while (Q--) {
        char c; int x, y;
        scanf("%c %d %d\n", &c, &x, &y);
        if (c == 'U') {
            update(1, 0, N, x-1, y);
        } else {
            printf("%d\n", query(1, 0, N, x-1, y).second);
        }
    }
}
