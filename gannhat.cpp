// 2023-12-26
// We break the problem down into 4 subproblems. The first subproblem is to
// find, for each point (x, y), the closest point that is of the form (x', y')
// where x' < x and y' <= y (that is, (x', y') is below and to the left of
// (x, y)). (It's also possible that x' = x; this is a detail that turns out not
// to matter.) If we can solve one such subproblem, then we can reduce the other
// three subproblems to it by successive 90 degree rotations.
//
// To solve the subproblem, we use a left-to-right sweep line with a segment
// tree indexed by (compressed) y-coordinate. The segment tree keeps track of
// the maximum value of x + y achieved by a point at a given y-coordinate. When
// we process a point (x, y), we use the segment tree to find the maximum value
// of x + y for all points with y' <= y. This is the value of x' + y' for the
// point (x', y') that is closest to (x, y) while being below and to the left of
// it. The difference (x + y) - (x' + y') is the distance between them. Then we
// insert the current point into the segment tree.
//
// This problem only accepts C++03, but luckily the pain is fairly limited.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void rotate(vector<int>& X, vector<int>& Y) {
    for (int i = 0; i < X.size(); i++) {
        const int t = Y[i];
        Y[i] = X[i];
        X[i] = -t;
    }
}
struct Segtree {
    vector<int> data_;
    int size_;
    Segtree(int size, int initial) : data_(4 * size, initial), size_(size) {}
    int query(int ql, int qr) {
        return query(ql, qr, 1, 0, size_);
    }
    int query(int ql, int qr, int node, int tl, int tr) {
        if (ql <= tl && qr >= tr) return data_[node];
        int result = -1e9;
        const int tm = (tl + tr) / 2;
        if (ql < tm && qr > tl) {
            result = max(result, query(ql, qr, 2*node, tl, tm));
        }
        if (ql < tr && qr > tm) {
            result = max(result, query(ql, qr, 2*node + 1, tm, tr));
        }
        return result;
    }
    void update(int idx, int val) {
        update(idx, val, 1, 0, size_);
    }
    void update(int idx, int val, int node, int tl, int tr) {
        if (tr == tl + 1) {
            data_[node] = max(data_[node], val);
            return;
        }
        const int tm = (tl + tr) / 2;
        if (idx < tm) {
            update(idx, val, 2*node, tl, tm);
        } else {
            update(idx, val, 2*node + 1, tm, tr);
        }
        data_[node] = max(data_[2*node], data_[2*node + 1]);
    }
};
void do_pass(const vector<int>& X, const vector<int>& Y, vector<int>& result) {
    const int N = X.size();
    vector<pair<int, int> > Ysorted;
    for (int i = 0; i < N; i++) {
        Ysorted.push_back(make_pair(Y[i], i));
    }
    sort(Ysorted.begin(), Ysorted.end());
    int yunique = 0;
    vector<int> Ycompress(N);
    for (int j = 0; j < N; j++) {
        if (j == 0 || Ysorted[j].first > Ysorted[j - 1].first) {
            ++yunique;
        }
        Ycompress[Ysorted[j].second] = yunique - 1;
    }
    Segtree S(yunique, -1e9);
    vector<pair<int, int> > Xsorted;
    for (int i = 0; i < N; i++) {
        Xsorted.push_back(make_pair(X[i], i));
    }
    sort(Xsorted.begin(), Xsorted.end());
    for (int j = 0; j < N; j++) {
        const int i = Xsorted[j].second;
        const int yindex = Ycompress[i];
        result[i] = min(result[i], X[i] + Y[i] - S.query(0, yindex + 1));
        S.update(yindex, X[i] + Y[i]);
    }
}
int main() {
    int N; scanf("%d", &N);
    vector<int> X(N);
    vector<int> Y(N);
    vector<int> result(N, 1e9);
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &X[i], &Y[i]);
    }
    for (int pass = 0; pass < 4; pass++) {
        do_pass(X, Y, result);
        if (pass != 3) {
            rotate(X, Y);
        }
    }
    for (int i = 0; i < N; i++) {
        printf("%d\n", result[i]);
    }
}
