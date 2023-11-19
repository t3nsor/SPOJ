// 2023-11-18
// This solution is due to James Smith: https://www.quora.com/What-is-the-efficient-solution-to-SPOJ-CCROSSX/answer/James-Smith-16196
// The following variables are defined:
// * n, d, h[i]: as in the problem statement, except that we index from 0
// * a[i]: the current height of pillar `i`. Note that a[0] = h[0].
// * D[i]: the current value of a[i] - a[i-1] (constrained to fall within the
//         interval [-d, +d]). Thus, a[i] = h[0] + D[1] + ... + D[i].
// * s[i]: | +1   if a[i] >= h[i]
//         | -1   if a[i] < h[i]
// * S[i]: | s[i] + s[i+1] + ... + s[n-1]   if D[i] < d
//         | +inf                           otherwise
//         The meaning of S[i] is that if D[i] is increased by 1, then the
//         total cost increases by S[i].
// * l[i]: | h[i] - a[i]   if a[i] < h[i]
//         | +inf          otherwise
//
// With the above notation, the algorithm can be described as follows. First,
// set D[i] = -d for each `i` in {1, ..., n-1}. Then:
// 1. Find the `i` such that S[i] is minimal. (This means `i` is the index such
//    that D[i] is optimal to increase at this step in the greedy algorithm.)
// 2. Calculate how much to increase D[i] by before the minimal `i` potentially
//    changes. This is the minimum of {l[i], l[i+1], ..., l[n-1], d - D[i]}.
//    Call this value `x`.
// 3. Increase D[i] by `x`. This means that we also need to update `S` and `l`.
//    If we obtain a[n-1] = h[n-1], then terminate. Otherwise, go back to
//    step 1.
// The total number of times the above loop can run is at most 2(n-1), because
// each step either brings some a[j] up to h[j] (which can only happen once per
// `j`) or raises D[i] up to +d (likewise).
//
// To implement the above steps efficiently:
// * The `S` values need to be stored in a range tree so we can easily query the
//   minimum in step 1. In step 3, we must use lazy propagation to update
//   S[1], ..., S[i] in one go.
// * Similarly, the `l` values need to be stored in a range tree so we can query
//   the minimum in step 2 and use lazy propagation to update in step 3: when
//   D[i] increases by `x`, the values l[i], l[i+1], ..., l[n-1] decrease by
//   `x`.

#include <algorithm>
#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;

struct RangeTree {
    const int n_;
    vector<long long> data_;
    vector<long long> lazy_;
    vector<int> argmin_;
    template <class RIt>
    RangeTree(RIt begin, RIt end) : n_(end - begin) {
        init(begin, end, 1, 0, n_);
    }
    template <class RIt>
    void init(RIt begin, RIt end, int node, int tl, int tr) {
        if (tr == tl + 1) {
            if (data_.size() <= node) {
                data_.resize(node + 1);
                lazy_.resize(node + 1);
                argmin_.resize(node + 1);
            }
            data_[node] = begin[tl];
            argmin_[node] = tl;
            return;
        }
        const int m = (tl + tr) / 2;
        init(begin, end, 2*node, tl, m);
        init(begin, end, 2*node + 1, m, tr);
        if (data_[2*node] <= data_[2*node+1]) {
            data_[node] = data_[2*node];
            argmin_[node] = argmin_[2*node];
        } else {
            data_[node] = data_[2*node+1];
            argmin_[node] = argmin_[2*node+1];
        }
    }
    pair<int, long long> query(int rl, int rr,
                               int node = 1, int tl = -1, int tr = -1) {
        if (node == 1) {
            tl = 0;
            tr = n_;
        }
        if (rl == tl && rr == tr) {
            return {argmin_[node], data_[node] + lazy_[node]};
        }
        lazy_[2*node] += lazy_[node];
        lazy_[2*node+1] += lazy_[node];
        data_[node] += exchange(lazy_[node], 0);
        int m = (tl + tr) / 2;
        pair<int, long long> result = {-1, LLONG_MAX};
        if (rl < m) {
            const auto lresult = query(rl, min(rr, m), 2*node, tl, m);
            if (lresult.second < result.second) {
                result = lresult;
            }
        }
        if (rr > m) {
            const auto rresult = query(max(rl, m), rr, 2*node+1, m, tr);
            if (rresult.second < result.second) {
                result = rresult;
            }
        }
        return result;
    }
    void update(int rl, int rr, long long x,
                int node = 1, int tl = -1, int tr = -1) {
        if (node == 1) {
            tl = 0;
            tr = n_;
        }
        if (rl == tl && rr == tr) {
            lazy_[node] += x;
            return;
        }
        lazy_[2*node] += lazy_[node];
        lazy_[2*node+1] += lazy_[node];
        lazy_[node] = 0;
        int m = (tl + tr) / 2;
        if (rl < m) {
            update(rl, min(rr, m), x, 2*node, tl, m);
        }
        if (rr > m) {
            update(max(rl, m), rr, x, 2*node+1, m, tr);
        }
        if (data_[2*node] + lazy_[2*node] <=
            data_[2*node+1] + lazy_[2*node+1]) {
            data_[node] = data_[2*node] + lazy_[2*node];
            argmin_[node] = argmin_[2*node];
        } else {
            data_[node] = data_[2*node+1] + lazy_[2*node+1];
            argmin_[node] = argmin_[2*node+1];
        }
    }
};
void do_testcase() {
    int n; long long d; cin >> n >> d;
    vector<int> h(n);
    for (int i = 0; i < n; i++) cin >> h[i];
    if (abs(h[n-1] - h[0]) > (n-1)*d) {
        cout << "impossible\n";
        return;
    }
    vector<long long> D(n, -d);  // remove if not needed
    vector<long long> ldata(n);
    long long result = 0;
    for (int i = 1; i < n; i++) {
        long long init = h[0] - i*d;
        if (init <= h[i]) {
            ldata[i] = h[i] - init;
        } else {
            ldata[i] = 2e9;
        }
        result += abs(init - h[i]);
    }
    RangeTree l(ldata.begin(), ldata.end());
    vector<long long> Sdata(n);
    long long curS = 0;
    for (int i = n-1; i > 0; i--) {
        long long init = h[0] - i*d;
        if (init <= h[i]) {
            curS--;
        } else {
            curS++;
        }
        Sdata[i] = curS;
    }
    RangeTree S(Sdata.begin(), Sdata.end());
    for (;;) {
        const auto q1 = S.query(1, n);
        const int i = q1.first;
        const long long dcost = q1.second;
        const auto q2 = l.query(i, n);
        const int j = q2.first;
        const long long x = q2.second;
        if (d - D[i] < x) {
            result += (d - D[i]) * dcost;
            // unnecessary as `i` will never be selected again anyway:
            // D[i] = d;
            l.update(i, n, -(d - D[i]));
            S.update(i, i + 1, 1e9);
        } else {
            D[i] += x;
            result += x * dcost;
            l.update(i, n, -x);
            l.update(j, j + 1, 1e14);
            S.update(1, j + 1, 2);
            if (j == n - 1) break;
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
