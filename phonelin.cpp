// 2026-08-10
// The "skips 3 towers" thing is a pointless complication.  The problem splits
// into 4 completely independent subproblems, where subproblem `r` consists of
// towers numbered r (modulo 4) assuming the towers are numbered in order of
// increasing x-coordinate, and the set of cities such that the closest tower to
// the left is part of the subproblem.
//
// The problem statement makes it sound like towers and cities can't be at the
// same point, but the test data shows a tower at the same point as a city, so I
// guess it just meant two towers can't be at the same point and two cities
// can't be at the same point.
//
// Bounds not stated in the problem description but determined experimentally on
// SPOJ include:
//   D <= 10^6
//   0 <= coordinate of each city or tower <= 10^6
//   0 <= cost of each tower <= 10^4
// Since the problem doesn't tell us bounds, I should have used `long long`, but
// I used `int` and got accepted anyway.
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
struct SubP {
    vector<int> cx;  // not sorted
    vector<int> tx;
    vector<int> cost;
};
int solve(int D, const SubP& subp) {
    const auto& cx = subp.cx;
    const auto& tx = subp.tx;
    const auto& cost = subp.cost;
    const int T = tx.size();
    // dp[i][j] is the maximum profit for the first `i` towers and the cities
    // that would have one of the first `i` towers as their nearest tower,
    // assuming that the last `j` towers are on.
    vector<vector<int>> dp(T + 1, vector<int>(T + 1));
    dp[0][0] = 0;
    for (int i = 1; i <= T; i++) {
        // make a list of city coordinates whose closest tower is `i - 1`
        vector<int> v;
        for (const auto x : cx) {
            if (x > tx[i - 1] && (i == T || x <= tx[i])) v.push_back(x);
        }
        dp[i][0] = 0;
        for (int k = 0; k <= i - 1; k++) {
            dp[i][0] = max(dp[i][0], dp[i - 1][k]);
        }
        // We just need to make sure the inner loop isn't too slow (time limit
        // is 0.01s).  The obvious way to do it is to incrementally update the
        // revenue when we insert the assumption that an additional tower to the
        // left is operational instead of starting from scratch using the fact
        // that the last `j` towers are operational.
        int rev = 0;
        for (const auto x : v) {
            if (x - tx[i - 1] < D) {
                rev += D - (x - tx[i - 1]);
            }
        }
        dp[i][1] = dp[i - 1][0] - cost[i - 1] + rev;
        for (int j = 2; j <= i; j++) {
            for (const auto x : v) {
                if (x - tx[i - j] < D) {
                    rev += D - (x - tx[i - j]);
                }
            }
            dp[i][j] = dp[i - 1][j - 1] - cost[i - 1] + rev;
        }
        for (int j = i - 1; j >= 0; j--) dp[i][j] = max(dp[i][j], dp[i][j + 1]);
    }
    return dp[T][0];
}
int do_testcase(int D, int C, int T) {
    vector<int> cx(C);
    for (int i = 0; i < C; i++) cin >> cx[i];
    vector<pair<int, int>> tower(T);
    for (int i = 0; i < T; i++) cin >> tower[i].first >> tower[i].second;
    sort(tower.begin(), tower.end());
    SubP subproblems[4];
    for (int i = 0; i < tower.size(); i++) {
        subproblems[i % 4].tx.push_back(tower[i].first);
        subproblems[i % 4].cost.push_back(tower[i].second);
    }
    for (int i = 0; i < cx.size(); i++) {
        int j = -1;
        while (j + 1 < T && tower[j + 1].first < cx[i]) j++;
        if (j >= 0) subproblems[j % 4].cx.push_back(cx[i]);
    }
    int result = 0;
    for (const auto& sp : subproblems) result += solve(D, sp);
    return result;
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int D, C, T; cin >> D >> C >> T;
        if (C < 0) break;
        cout << do_testcase(D, C, T) << '\n';
    }
}
