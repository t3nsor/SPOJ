// 2023-10-11
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int n; cin >> n;
    int c; cin >> c;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a.begin(), a.end());
    vector<long long> ps(n+1);
    ps[0] = 0;
    for (int i = 1; i <= n; i++) {
        ps[i] = ps[i-1] + a[i-1];
    }
    // dp[i][j] is the minimum cumulative error if `i` colours are used to
    // colour the first `j` regions
    vector<vector<long long>> dp(c + 1, vector<long long>(n + 1, 1ll << 42));
    dp[0][0] = 0;
    for (int i = 1; i <= c; i++) {
        for (int j = 0; j <= n; j++) {
            dp[i][j] = dp[i-1][j];
            // k is the size of the last group
            for (int k = 1; k <= j; k++) {
                const int first = j - k;
                const long long median = a[first + k / 2];
                const long long error =
                  (median * (k / 2))
                  - (ps[first + k / 2] - ps[first])
                  + (ps[first + k] - ps[first + k / 2])
                  - (median * (k - (k / 2)));
                dp[i][j] = min(dp[i][j], dp[i-1][j-k] + error);
            }
        }
    }
    cout << dp[c][n] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
