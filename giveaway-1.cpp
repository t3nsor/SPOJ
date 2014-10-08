// 2014-03-29
#include <cstdio>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
vector<vector<int> > pts(2000000);
vector<vector<int> > trees(2000000);
vector<int> init;
vector<vector<int> > ys(500000);
int X[500000];
int N;
void build_tree_1d(int node1, int node2, int b, int t) {
    if (b == t) return;
    if (t == b + 1) {
        // populate from precomputed initial values
        trees[node1][node2] = init[b];
    } else {
        int m = b + (t - b + 1)/2;
        build_tree_1d(node1, 2*node2, b, m);
        build_tree_1d(node1, 2*node2+1, m, t);
        trees[node1][node2] = trees[node1][2*node2] + trees[node1][2*node2+1];
    }
}
void build_tree(int node, int l, int r) {
    if (r == l) return;
    if (r == l + 1) {
        // C++11: will move
        swap(ys[l], pts[node]);
    } else {
        int m = l + (r - l + 1)/2;
        build_tree(2*node, l, m);
        build_tree(2*node+1, m, r);
        // merge sublists
        set_union(pts[2*node].begin(), pts[2*node].end(),
                  pts[2*node+1].begin(), pts[2*node+1].end(),
                  back_inserter(pts[node]));
    }
    // prepare the list of initial values for this range
    init.clear(); init.resize(pts[node].size());
    for (int i = l; i < r; i++) {
        init[lower_bound(pts[node].begin(), pts[node].end(), X[i])
             - pts[node].begin()] += 1;
    }
    trees[node].resize(4*pts[node].size());
    build_tree_1d(node, 1, 0, pts[node].size());
}
void update_rec_1d(int idx, int newval, int node1, int node2, int b, int t) {
    if (t == b) return;
    if (t == b + 1) {
        trees[node1][node2] += newval;
    } else {
        int m = b + (t - b + 1)/2;
        if (idx < m) {
            update_rec_1d(idx, newval, node1, 2*node2, b, m);
        } else {
            update_rec_1d(idx, newval, node1, 2*node2+1, m, t);
        }
        trees[node1][node2] = trees[node1][2*node2] + trees[node1][2*node2+1];
    }
}
void update_1d(int y, int newval, int node1) {
    int idx = lower_bound(pts[node1].begin(), pts[node1].end(), y)
              - pts[node1].begin();
    update_rec_1d(idx, newval, node1, 1, 0, pts[node1].size());
}
void update_rec(int x, int y, int newval, int node, int l, int r) {
    if (r == l) return;
    int m = l + (r - l + 1)/2;
    if (r > l + 1) {
        if (x < m) {
            update_rec(x, y, newval, 2*node, l, m);
        } else {
            update_rec(x, y, newval, 2*node+1, m, r);
        }
    }
    update_1d(y, newval, node);
}
void update(int x, int y, int newval) {
    update_rec(x, y, newval, 1, 0, N);
}
int query_rec_1d(int node1, int node2, int b, int t, int y1, int y2) {
    if (b >= y1 && t <= y2) {
        return trees[node1][node2];
    } else {
        int m = b + (t - b + 1)/2;
        int res = 0;
        if (m > y1 && b < y2) {
            res += query_rec_1d(node1, 2*node2, b, m, y1, y2);
        }
        if (t > y1 && m < y2) {
            res += query_rec_1d(node1, 2*node2+1, m, t, y1, y2);
        }
        return res;
    }
}
int query_1d(int node1, int y1, int y2) {
    int b = lower_bound(pts[node1].begin(), pts[node1].end(), y1)
            - pts[node1].begin();
    int t = lower_bound(pts[node1].begin(), pts[node1].end(), y2)
            - pts[node1].begin();
    return query_rec_1d(node1, 1, 0, pts[node1].size(), b, t);
}
int query_rec(int node, int l, int r, int x1, int x2, int y1, int y2) {
    if (l >= x1 && r <= x2) {
        return query_1d(node, y1, y2);
    } else {
        int m = l + (r - l + 1)/2;
        int res = 0;
        if (m > x1 && l < x2) {
            res += query_rec(2*node, l, m, x1, x2, y1, y2);
        }
        if (r > x1 && m < x2) {
            res += query_rec(2*node+1, m, r, x1, x2, y1, y2);
        }
        return res;
    }
}
int query(int x1, int x2, int y1, int y2) {
    return query_rec(1, 0, N, x1, x2, y1, y2);
}
int a[100000], b[100000], c[100000], q[100000];
int main() {
    int Q;
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", X + i);
        ys[i].push_back(X[i]);
    }
    scanf("%d", &Q);
    for (int i = 0; i < Q; i++) {
        scanf("%d", q + i);
        if (q[i] == 0) {
            scanf("%d %d %d", a+i, b+i, c+i); a[i]--;
        } else {
            scanf("%d %d", a+i, b+i); a[i]--;
            ys[a[i]].push_back(b[i]);
        }
    }
    for (int i = 0; i < N; i++) {
        sort(ys[i].begin(), ys[i].end());
        ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
    }
    // construct 2d segtree
    build_tree(1, 0, N);
    // process the queries
    for (int i = 0; i < Q; i++) {
        if (q[i] == 0) {
            printf("%d\n", query(a[i], b[i], c[i], INT_MAX));
        } else {
            update(a[i], X[a[i]], -1);
            update(a[i], b[i], 1);
            X[a[i]] = b[i];
        }
    }
}
