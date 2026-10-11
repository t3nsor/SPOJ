// 2026-10-11
// bog-standard segtree with lazy propagation (not sure why one user in the
// comments is very confused about the problem)
#include <stdio.h>
using LL = long long;
LL tree[40000];
LL lazy[40000];
int M;
LL modexp(LL b, int e) {
    if (e == 0) return 1;
    LL r = modexp(b, e / 2);
    r = (r * r) % M;
    if (e % 2) {
        r = (r * b) % M;
    }
    return r;
}
void propagate(int node, int l, int r) {
    const int m = (l + r) / 2;
    lazy[2*node] = (lazy[2*node] * lazy[node]) % M;
    lazy[2*node + 1] = (lazy[2*node + 1] * lazy[node]) % M;
    tree[2*node] = (tree[2*node] * modexp(lazy[node], m - l)) % M;
    tree[2*node + 1] = (tree[2*node + 1] * modexp(lazy[node], r - m)) % M;
    lazy[node] = 1;
}
void upd(int node, int l, int r, int idx, int val) {
    if (r - l == 1) {
        tree[node] = val;
        return;
    }
    if (lazy[node] != 1) propagate(node, l, r);
    const int m = (l + r) / 2;
    if (idx < m) upd(2*node, l, m, idx, val);
    else upd(2*node + 1, m, r, idx, val);
    tree[node] = (tree[2*node] * tree[2*node + 1]) % M;
}
LL query(int node, int tl, int tr, int ql, int qr) {
    if (tl >= ql && tr <= qr) return tree[node];
    if (lazy[node] != 1) propagate(node, tl, tr);
    const int tm = (tl + tr) / 2;
    LL result = 1;
    if (tl < qr && tm > ql) {
        result = (result * query(2*node, tl, tm, ql, qr)) % M;
    }
    if (tm < qr && tr > ql) {
        result = (result * query(2*node + 1, tm, tr, ql, qr)) % M;
    }
    return result;
}
int main() {
    int R, N; scanf("%d %d %d", &R, &N, &M);
    for (int i = 0; i < 4*N; i++) tree[i] = lazy[i] = 1;
    int pos[2] = {0, 0};
    int player = 0;
    while (R--) {
        int d, v, a, b; scanf("%d %d %d %d", &d, &v, &a, &b); b++;
        upd(1, 0, N, d, v);
        const int steps = query(1, 0, N, a, b);
        pos[player] = (pos[player] + steps) % M;
        printf("%lld\n", pos[player]);
        player = !player;
        if (pos[0] == pos[1]) {
            lazy[1] = (lazy[1] * pos[0]) % M;
            tree[1] = (tree[1] * modexp(pos[0], N)) % M;
        }
    }
}
