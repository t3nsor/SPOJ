// 2025-06-10
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;

struct Data {
    int np;     // number of parts
    int first;
    int last;
};

struct Node {
    int lazy = -1;
    Data data;
};

struct Segtree {
    int N;
    vector<Node> nodes;

    Segtree(int N, int* a) : N(N), nodes(4*N) {
        build_tree(1, 0, N, a);
    }

    static Data combine(const Data& l, const Data& r) {
        if (l.np < 0) return r;
        if (r.np < 0) return l;
        Data result;
        result.first = l.first;
        result.last = r.last;
        result.np = l.np + r.np - (l.last == r.first);
        return result;
    }

    void propagate(int node) {
        if (nodes[node].lazy >= 0) {
            for (int child = 2*node; child <= 2*node + 1; child++) {
                nodes[child].data.first = nodes[child].data.last
                = nodes[child].lazy = nodes[node].lazy;
                nodes[child].data.np = 1;
            }
            nodes[node].lazy = -1;
        }
    }

    void build_tree(int node, int l, int r, int* a) {
        if (r == l + 1) {
            nodes[node].data = Data{1, a[l], a[l]};
            return;
        }
        build_tree(2*node, l, (l + r) / 2, a);
        build_tree(2*node + 1, (l + r) / 2, r, a);
        nodes[node].data = combine(nodes[2*node].data, nodes[2*node + 1].data);
    }

    Data query(int node, int tl, int tr, int ql, int qr) {
        if (tl >= qr || tr <= ql) return Data{-1};
        if (tl >= ql && tr <= qr) return nodes[node].data;
        propagate(node);
        const int tm = (tl + tr) / 2;
        return combine(query(2*node, tl, tm, ql, qr),
                       query(2*node + 1, tm, tr, ql, qr));
    }

    Data query(int l, int r) {
        return query(1, 0, N, l, r);
    }

    void update_range(int node, int tl, int tr, int ul, int ur, int v) {
        if (tl >= ur || tr <= ul) return;
        if (tl >= ul && tr <= ur) {
            nodes[node].data = Data{1, v, v};
            if (tr - tl > 1) nodes[node].lazy = v;
            return;
        }
        propagate(node);
        const int tm = (tl + tr) / 2;
        if (ul < tm && ur > tl) update_range(2*node, tl, tm, ul, ur, v);
        if (ul < tr && ur > tm) update_range(2*node + 1, tm, tr, ul, ur, v);
        nodes[node].data = combine(nodes[2*node].data, nodes[2*node + 1].data);
    }

    void update_range(int l, int r, int v) {
        update_range(1, 0, N, l, r, v);
    }

    void update(int node, int tl, int tr, int i, int v) {
        if (tr == tl + 1) {
            nodes[node].data = Data{1, v, v};
            return;
        }
        propagate(node);
        const int tm = (tl + tr) / 2;
        if (i < tm) update(2*node, tl, tm, i, v);
        else update(2*node + 1, tm, tr, i, v);
        nodes[node].data = combine(nodes[2*node].data, nodes[2*node + 1].data);
    }

    void update(int i, int v) {
        update(1, 0, N, i, v);
    }
};

int main() {
    int N, C; scanf("%d %d", &N, &C);
    vector<int> init(N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &init[i]); --init[i];
    }
    Segtree tree(N, init.data());
    // `rev` and `ofs` together identify an element of the dihedral group with
    // 2N elements. Leaf `i` of the tree equals bead (+/- i) + ofs (mod N) of
    // the necklace, where `i` is negated only if `rev` is true.
    bool rev = false; int ofs = 0;
    int Q; scanf("%d", &Q);
    while (Q--) {
        char c; do { c = getchar_unlocked(); } while (c <= 32);
        switch (c) {
          case 'C': {
            c = getchar_unlocked();
            Data data;
            int i, j;
            bool circ = false;
            if (c == 'S') {
                scanf("%d %d", &i, &j); i--; j--;
            } else {
                i = 0; j = N - 1; circ = true;
            }
            i = (i - ofs + N) % N;
            j = (j - ofs + N) % N;
            if (rev) { i = (N - i) % N; j = (N - j) % N; swap(i, j); }
            j = (j + 1) % N;
            if (i < j) {
                data = tree.query(i, j);
            } else {
                data = Segtree::combine(tree.query(i, N),
                                        tree.query(0, j));
            }
            if (circ && data.np > 1 && data.first == data.last) --data.np;
            printf("%d\n", data.np);
          } break;
          case 'F': {
            rev = !rev;
            ofs = (N - ofs) % N;
          } break;
          case 'P': {
            int i, j, c; scanf("%d %d %d", &i, &j, &c); --i; --j; --c;
            i = (i - ofs + N) % N;
            j = (j - ofs + N) % N;
            if (rev) { i = (N - i) % N; j = (N - j) % N; swap(i, j); }
            j = (j + 1) % N;
            if (i < j) tree.update_range(i, j, c);
            else {
                tree.update_range(0, j, c);
                tree.update_range(i, N, c);
            }
          } break;
          case 'R': {
            int k; scanf("%d", &k);
            ofs = (ofs + k) % N;
          } break;
          case 'S': {
            int i, j; scanf("%d %d", &i, &j); --i; --j;
            i = (i - ofs + N) % N;
            j = (j - ofs + N) % N;
            if (rev) { i = (N - i) % N; j = (N - j) % N; }
            const int ci = tree.query(i, i + 1).first;
            const int cj = tree.query(j, j + 1).first;
            tree.update(i, cj);
            tree.update(j, ci);
          } break;
        }
    }
}
