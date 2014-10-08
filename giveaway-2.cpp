// 2014-03-30
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstddef>
#include <cstdio>
using namespace std;
int N;
int X[500000];
vector<vector<int> > ys(500000);
vector<vector<int> > trees(2000000);
vector<vector<int> > pts(2000000);
vector<int> init;
void build_tree_1d(int node1, int node2, int b, int t) {
    if (b == t) return;
    if (t == b + 1) {
        trees[node1][node2] = init[b];
    } else {
        int m = b + (t - b + 1)/2;
        build_tree_1d(node1, 2*node2, b, m);
        build_tree_1d(node1, 2*node2+1, m, t);
        trees[node1][node2] = trees[node1][2*node2] + trees[node1][2*node2+1];
    }
}
void build_tree(int node, int l, int r) {
    if (l == r) return;
    if (r == l + 1) {
        swap(ys[l], pts[node]);
    } else {
        int m = l + (r - l + 1)/2;
        build_tree(2*node, l, m);
        build_tree(2*node+1, m, r);
        set_union(pts[2*node].begin(), pts[2*node].end(),
                  pts[2*node+1].begin(), pts[2*node+1].end(),
                  back_inserter(pts[node]));
    }
    init.clear(); init.resize(pts[node].size());
    for (int i = l; i < r; i++) {
        init[lower_bound(pts[node].begin(), pts[node].end(), X[i])
             - pts[node].begin()]++;
    }
    trees[node].resize(4*pts[node].size());
    build_tree_1d(node, 1, 0, pts[node].size());
}
void update_rec_1d(int node1, int node2, int b, int t, int idx, int increment) {
    if (t == b) return;
    trees[node1][node2] += increment;
    if (t > b + 1) {
        int m = b + (t - b + 1)/2;
        if (idx < m) {
            update_rec_1d(node1, 2*node2, b, m, idx, increment);
        } else {
            update_rec_1d(node1, 2*node2+1, m, t, idx, increment);
        }
    }
}
void update_1d(int node, int y, int increment) {
    int idx = lower_bound(pts[node].begin(), pts[node].end(), y)
              - pts[node].begin();
    update_rec_1d(node, 1, 0, pts[node].size(), idx, increment);
}
void update_rec(int node, int l, int r, int x, int y, int increment) {
    if (r == l) return;
    if (r > l + 1) {
        int m = l + (r - l + 1)/2;
        if (x < m) {
            update_rec(2*node, l, m, x, y, increment);
        } else {
            update_rec(2*node+1, m, r, x, y, increment);
        }
    }
    update_1d(node, y, increment);
}
void update(int x, int y, int increment) {
    update_rec(1, 0, N, x, y, increment);
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
int query_1d(int node, int y1, int y2) {
    int idx1 = lower_bound(pts[node].begin(), pts[node].end(), y1)
               - pts[node].begin();
    int idx2 = lower_bound(pts[node].begin(), pts[node].end(), y2)
               - pts[node].begin();
    return query_rec_1d(node, 1, 0, pts[node].size(), idx1, idx2);
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
int main() {
    int Q;
    int a[500000], b[500000], c[500000], q[500000];
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", X + i);
        ys[i].push_back(X[i]);
    }
    scanf("%d", &Q);
    for (int i = 0; i < Q; i++) {
        scanf("%d", q + i);
        if (q[i] == 0) {
            scanf("%d %d %d", a+i, b+i, c+i);
            a[i]--;
        } else {
            scanf("%d %d", a+i, b+i); a[i]--;
            ys[a[i]].push_back(b[i]);
        }
    }
    for (int i = 0; i < N; i++) {
        sort(ys[i].begin(), ys[i].end());
        ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
    }
    build_tree(1, 0, N);
    for (int i = 0; i < Q; i++) {
        if (q[i] == 0) {
            printf("%d\n", query(a[i], b[i], c[i], 2e9));
        } else {
            update(a[i], X[a[i]], -1);
            update(a[i], b[i], 1);
            X[a[i]] = b[i];
        }
    }
    return 0;
}
