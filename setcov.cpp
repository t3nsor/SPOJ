// 2026-04-11
// The first challenge is understanding the problem statement.  To restate it
// succinctly, we are given a binary matrix with M rows and N columns, and a
// guarantee that there exists a way to permute the rows so that the resulting
// matrix has the property that for each column, all ones (if any) are
// consecutive.  For the original matrix, we need to find a minimum cardinality
// subset of rows such that their bitwise OR consists of all ones.
//
// If we can exhibit such a permutation, we reduce the problem to one that can
// be solved with a simple greedy algorithm.  However, we don't actually need to
// construct the permutation.  All we need to know is, if the permutation were
// to be applied (making the set of ones in each column into an interval) how
// those intervals' endpoints are ordered relative to each other.  This
// information can be constructed by comparing intersections of columns (the
// details are left as an exercise for the reader).
#include <bitset>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>
using namespace std;
using Col = bitset<300>;
bool sbst(const Col& x, const Col& y) {
    return (x & ~y).none();
}
bool isct(const Col& x, const Col& y) {
    return (x & y).any();
}
int main() {
    ios::sync_with_stdio(false);
    int N, M; cin >> N >> M;
    vector<Col> C(N);
    for (int i = 0; i < M; i++) {
        int s; cin >> s;
        while (s--) {
            int col; cin >> col;
            C[col][i] = 1;
        }
    }
    vector<Col> C2;
    for (int i = 0; i < N; i++) {
        if (C[i].none()) {
            cout << "-1\n";
            return 0;
        }
        bool remove = false;
        for (int j = 0; j < C.size(); j++) {
            // If C_i is a superset of C_j, remove C_i, except that if the two
            // are identical, we have to keep one and discard the rest; we
            // arbitrarily choose the one with largest index.  This step is
            // necessary so that later on, whenever interval A starts after
            // interval B, it also finishes after.
            if (sbst(C[j], C[i]) && (C[i] != C[j] || i < j)) {
                remove = true;
                break;
            }
        }
        if (!remove) C2.push_back(C[i]);
    }
    swap(C, C2);
    N = C.size();
    vector<int> vis(N);
    vector<int> result;
    for (int i = 0; i < N; i++) {
        if (vis[i]) continue;
        // In this iteration of the outer loop, we'll solve the connected
        // component that contains column `i` (two columns are considered
        // adjacent if their intersection is nonempty).
        vector<int> o1;
        stack<int> S;
        S.push(i);
        while (!S.empty()) {
            int a = S.top(); S.pop();
            if (vis[a]) continue; vis[a] = 1;
            if (o1.size() <= 1) {
                // If `a` is the second interval encountered in this component
                // so far, arbitrarily designate it as being to the right of the
                // first.
                o1.push_back(a);
            } else {
                int j = 0;
                // Find the first `j` such that `a` overlaps with the `j`th
                // interval that's already in `o1`.
                while (!isct(C[a], C[o1[j]])) ++j;
                // If `a` has a bigger overlap with interval `o1[j]` than with
                // any later interval, `a` is the leftmost interval seen so far.
                if (j + 1 < o1.size() &&
                    sbst(C[a] & C[o1[j + 1]], C[a] & C[o1[j]])) {
                    o1.insert(o1.begin() + j, a);
                } else {
                    // Find `j` such that `a` should be inserted immediately
                    // after the `j`th interval currently in `o1`.
                    for (;; j++) {
                        if (j == o1.size() - 1 ||
                            !sbst(C[a] & C[o1[j]], C[a] & C[o1[j + 1]])) {
                            o1.insert(o1.begin() + j + 1, a);
                            break;
                        }
                    }
                }
            }
            for (int j = 0; j < N; j++) {
                if (j != i && isct(C[a], C[j])) S.push(j);
            }
        }
        const int K = o1.size();
        // Assign the start points the coordinates 0, K, 2K, ..., and determine
        // the positions of the end points relative to the start points.
        vector<pair<int, int>> o2(K);
        vector<int> cur(K);
        for (int j = 0; j < o1.size(); j++) {
            o2[j].first = j*K;
            cur[j] = j*K + 1;
        }
        for (int j = 0; j < o1.size(); j++) {
            int k = j + 1;
            while (k < o1.size() && isct(C[o1[j]], C[o1[k]])) ++k;
            o2[j].second = cur[k - 1]++;
        }
        // We now have `o2`, a list of intervals given by their start and end
        // coordinates.  The idea is we partition this list into sublists such
        // that the intervals in each sublist have nonempty intersection, and
        // then (using the original bit vectors) arbitrarily select one element
        // from each such intersection.
        for (int j = 0; j < K;) {
            Col c = C[o1[j]];
            int k = j + 1;
            while (k < K && o2[k].first < o2[j].second) c &= C[o1[k++]];
            j = k;
            for (int k = 0; k < c.size(); k++) {
                if (c[k]) {
                    result.push_back(k);
                    break;
                }
            }
        }
    }
    cout << result.size() << '\n';
    for (int i = 0; i < result.size(); i++) {
        if (i > 0) cout << ' ';
        cout << result[i];
    }
    cout << '\n';
}
