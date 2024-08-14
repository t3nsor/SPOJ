// 2024-08-14
// The first part of the problem is easy: binary search for the minimum possible
// time and determine feasibility of a time using a greedy algorithm. For the
// second part, we use DP: let dp[i][j] be the minimum total time needed for
// agents 0, 1, ... i to move into positions such that they are all connected to
// each other and agent `i` is at position `j`.
//
// Evidently, if i > 0 then
//   dp[i][j] = |a[i] - j| + min_{|k - j| <= D} dp[i-1][k]
// where a[i] is the initial position of agent `i`. Implementing this transition
// naively gives O(NMD) (where M is the maximum initial position). Because the
// expression to be minimized is independent of j, we could optimize this to
// O(NM) using the sliding window minimum technique, but this is not necessary
// for getting AC.

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;
int calc1(int N, int D, const vector<int>& a) {
    const int M = a.back();
    int l = 0, r = M - 1;
    while (l < r) {
        const int m = (l + r) / 2;
        bool fail = false;
        int last = a[0] + m;
        for (int i = 1; i < N; i++) {
            int newlast = min(a[i] + m, last + D);
            if (newlast < a[i] - m) {
                fail = true;
                break;
            }
            last = newlast;
        }
        if (fail) l = m + 1; else r = m;
    }
    return l;
}
int calc2(int N, int D, const vector<int>& a) {
    const int M = a.back();
    vector<vector<int>> dp(N, vector<int>(M + 1, 1e6));
    for (int j = 0; j <= M; j++) {
        dp[0][j] = abs(j - a[0]);
    }
    for (int i = 1; i < N; i++) {
        for (int j = 0; j <= M; j++) {
            dp[i][j] = abs(j - a[i]) +
                       *min_element(dp[i - 1].begin() + max(0, j - D),
                                    dp[i - 1].begin() + min(M, j + D) + 1);
        }
    }
    return *min_element(dp[N - 1].begin(), dp[N - 1].end());
}
int main() {
    int T; cin >> T;
    while (T--) {
        int N, D; cin >> N >> D;
        vector<int> a(N);
        for (int i = 0; i < N; i++) {
            cin >> a[i];
            --a[i];
        }
        sort(a.begin(), a.end());
        cout << calc1(N, D, a) << ' ' << calc2(N, D, a) << '\n';
    }
}
