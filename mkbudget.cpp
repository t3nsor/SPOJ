// 2025-08-29
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;
int do_testcase(int N) {
    int F, W, S; cin >> F >> W >> S;
    vector<int> v(N);
    for (int i = 0; i < N; i++) cin >> v[i];
    vector<int> ns = v;
    ns.push_back(0);
    sort(ns.begin(), ns.end());
    ns.erase(unique(ns.begin(), ns.end()), ns.end());
    vector<vector<int>> dp(N + 1, vector<int>(ns.size()));
    dp[0][0] = 0;
    for (int i = 1; i < ns.size(); i++) dp[0][i] = 1e9;
    int result = 1e9;
    for (int i = 1; i <= N; i++) {
        for (int j = 0; j < ns.size(); j++) {
            dp[i][j] = 1e9;
            if (ns[j] < v[i - 1]) continue;
            for (int k = 0; k < ns.size(); k++) {
                int val = dp[i - 1][k] + W*ns[j];
                if (k < j) {
                    // hire some new workers
                    val += F*(ns[j] - ns[k]);
                } else if (k > j) {
                    // fire some workers
                    val += S*(ns[k] - ns[j]);
                }
                dp[i][j] = min(dp[i][j], val);
            }
            if (i == N) result = min(result, dp[i][j]);
        }
    }
    return result;
}
int main() {
    for (int cs = 1;; cs++) {
        int N; cin >> N; if (N == 0) break;
        printf("Case %d, cost = $%d\n", cs, do_testcase(N));
    }
}
