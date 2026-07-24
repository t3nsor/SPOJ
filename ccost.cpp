// 2026-07-24
// segtree of prefix sum arrays
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void integrate(vector<pair<int, int>>& v) {
    int sum = 0;
    for (int i = 0; i < v.size(); i++) sum = (v[i].second += sum);
}
void build_tree(vector<vector<pair<int, int>>>& rows,
                vector<vector<pair<int, int>>>& segtree,
                int node, int l, int r) {
    if (segtree.size() <= node) segtree.resize(node + 1);
    if (r == l + 1) { segtree[node] = move(rows[l]); return; }
    const int m = (l + r + 1) / 2;
    build_tree(rows, segtree, 2*node, l, m);
    build_tree(rows, segtree, 2*node + 1, m, r);
    auto& lvec = segtree[2*node];
    auto& rvec = segtree[2*node + 1];
    auto& vec = segtree[node];
    int i = 0, j = 0;
    while (i < lvec.size() && j < rvec.size()) {
        if (lvec[i].first <= rvec[j].first) {
            vec.push_back(lvec[i++]);
        } else {
            vec.push_back(rvec[j++]);
        }
    }
    if (i < lvec.size()) vec.insert(vec.end(), lvec.begin() + i, lvec.end());
    if (j < rvec.size()) vec.insert(vec.end(), rvec.begin() + j, rvec.end());
    integrate(lvec);
    integrate(rvec);
}
int query(const vector<vector<pair<int, int>>>& segtree,
          int node, int nl, int nr, int ql, int qr,
          int ymin, int ymax) {
    if (ql >= qr) return 0;
    if (nl >= ql && nr <= qr) {
        const auto& vec = segtree[node];
        int idx = upper_bound(vec.begin(), vec.end(), make_pair(ymax, INT_MAX))
                  - vec.begin() - 1;
        if (idx < 0) return 0;
        int result = vec[idx].second;
        idx = lower_bound(vec.begin(), vec.end(), make_pair(ymin, INT_MIN))
              - vec.begin() - 1;
        return result - (idx >= 0 ? vec[idx].second : 0);
    } else {
        int result = 0;
        const int nm = (nl + nr + 1) / 2;
        if (ql < nm && qr > nl) {
            result += query(segtree, 2*node, nl, nm, ql, qr, ymin, ymax);
        }
        if (ql < nr && qr > nm) {
            result += query(segtree, 2*node + 1, nm, nr, ql, qr, ymin, ymax);
        }
        return result;
    }
}
void do_testcase() {
    int n; scanf("%d", &n);
    vector<int> X(n), Y(n), val(n);
    vector<pair<int, int>> allx;
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &X[i], &Y[i], &val[i]);
        allx.emplace_back(X[i], i);
    }
    sort(allx.begin(), allx.end());
    vector<vector<pair<int, int>>> rows;
    vector<int> Xc;
    int nx = 0;
    for (int i = 0; i < allx.size(); i++) {
        if (i == 0 || allx[i].first != allx[i - 1].first) {
            ++nx;
            rows.emplace_back();
            Xc.push_back(allx[i].first);
        }
        const int idx = allx[i].second;
        rows.back().emplace_back(Y[idx], val[idx]);
    }
    for (auto& row : rows) sort(row.begin(), row.end());
    vector<vector<pair<int, int>>> segtree;
    build_tree(rows, segtree, 1, 0, nx);
    integrate(segtree[1]);
    int m; scanf("%d", &m);
    while (m--) {
        int x1, y1, x2, y2; scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        x1 = lower_bound(Xc.begin(), Xc.end(), x1) - Xc.begin();
        x2 = upper_bound(Xc.begin(), Xc.end(), x2) - Xc.begin();
        printf("%d\n", query(segtree, 1, 0, nx, x1, x2, y1, y2));
    }
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
