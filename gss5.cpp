// 2014-04-26
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct info {
    int i;
    int l;
    int r;
    int s;
} segtree[40000];
int a[10000];
int N;
void set(int node, int val) {
    segtree[node] = (info){val, val, val, val};
}
void merge(const info& l, const info& r, info& out) {
    out.s = l.s + r.s;
    out.l = max(l.l, l.s + r.l);
    out.r = max(r.r, r.s + l.r);
    out.i = max(max(l.i, r.i), l.r + r.l);
}
void build_tree(int node, int l, int r) {
    if (r - l == 1) {
        set(node, a[l]);
    } else {
        int m = l + (r - l + 1)/2;
        build_tree(2*node, l, m);
        build_tree(2*node+1, m, r);
        merge(segtree[2*node], segtree[2*node+1], segtree[node]);
    }
}
void update(int node, int l, int r, int idx, int newval) {
    if (r - l == 1) {
        a[idx] = newval;
        set(node, newval);
    } else {
        int m = l + (r - l + 1)/2;
        if (idx < m) {
            update(2*node, l, m, idx, newval);
        } else {
            update(2*node+1, m, r, idx, newval);
        }
        merge(segtree[2*node], segtree[2*node+1], segtree[node]);
    }
}
info query_rec(int node, int tbegin, int tend, int abegin, int aend) {
    if (abegin == aend) {
        return (info){-2e9, -2e9, -2e9, 0};
    } else if (tbegin >= abegin && tend <= aend) {
        return segtree[node];
    } else {
        int mid = tbegin + (tend - tbegin + 1)/2;
        if (mid > abegin && tbegin < aend) {
            if (tend > abegin && mid < aend) {
                info l = query_rec(2*node, tbegin, mid, abegin, aend);
                info r = query_rec(2*node+1, mid, tend, abegin, aend);
                info i; merge(l, r, i); return i;
            } else {
                return query_rec(2*node, tbegin, mid, abegin, aend);
            }
        } else {
            return query_rec(2*node+1, mid, tend, abegin, aend);
        }
    }
}
int query(int l1, int r1, int l2, int r2) {
    if (l2 < r1 && r2 <= r1) {
        // case 1: second range contained within first
        info i1 = query_rec(1, 0, N, l1, l2);
        info i2 = query_rec(1, 0, N, l2, r2);
        return max(i2.i, i1.r + i2.l);
    } else if (l2 < r1) {
        // case 2: second range overlaps first
        info i1 = query_rec(1, 0, N, l1, l2);
        info i2 = query_rec(1, 0, N, l2, r1);
        info i3 = query_rec(1, 0, N, r1, r2);
        return max(max(i1.r + i2.l, i1.r + i2.s + i3.l),
                   max(i2.i, i2.r + i3.l));
    } else {
        // case 3: disjoint ranges
        info i1 = query_rec(1, 0, N, l1, r1);
        info i2 = query_rec(1, 0, N, r1, l2);
        info i3 = query_rec(1, 0, N, l2, r2);
        return i1.r + i2.s + i3.l;
    }
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        scanf("%d", &N);
        for (int i = 0; i < N; i++) {
            scanf("%d", a+i); 
        }
        build_tree(1, 0, N);
        int M; scanf("%d", &M);
        while (M--) {
            int l1, r1, l2, r2;
            scanf("%d %d %d %d", &l1, &r1, &l2, &r2);
            printf("%d\n", query(l1-1, r1, l2-1, r2));
        }
    }
}
