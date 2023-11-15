// 2023-11-14
// This is the straightforward O(n^3) DP solution using monotonicity described
// in the editorial: if the optimal transition to the state with height `h` at
// index `i` is from the state with height `k` at index `i-1`, then the optimal
// transition to (h', i) where `h' > h` is from some state (k', i-1) where
// `k' >= k`. For a harder problem, see CCROSSX.
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
long long dp[100][22222];
void do_testcase() {
    int h[100];
    int n; long long d; cin >> n >> d;
    vector<long long> hts;
    int foo = 0;
    for (int i = 0; i < n; i++) {
        cin >> h[i];
        for (int j = -(n-1); j <= n-1; j++) {
            if (h[i] + j*d >= 0) {
                hts.push_back(h[i] + j*d);
            }
        }
    }
    sort(hts.begin(), hts.end());
    hts.erase(unique(hts.begin(), hts.end()), hts.end());
    for (int j = 0; j < hts.size(); j++) {
        dp[0][j] = 1e17;
    }
    vector<long long>::iterator I =
        lower_bound(hts.begin(), hts.end(), h[0]);
    dp[0][I - hts.begin()] = 0;
    for (int i = 1; i < n; i++) {
        int k = 0;
        for (int j = 0; j < hts.size(); j++) {
            while (k < hts.size() &&
                   (dp[i - 1][k] >= 1e17 ||
                    hts[k] + d < hts[j])) ++k;
            if (k == hts.size() || hts[k] > hts[j] + d) {
                dp[i][j] = 1e17;
                continue;
            }
            while (k + 1 < hts.size() &&
                   abs(hts[j] - hts[k + 1]) <= d &&
                   dp[i - 1][k + 1] < dp[i - 1][k]) ++k;
            dp[i][j] = dp[i - 1][k] + abs(h[i] - hts[j]);
        }        
    }
    I = lower_bound(hts.begin(), hts.end(), h[n-1]);
    if (dp[n-1][I - hts.begin()] < 1e17) {
        cout << dp[n-1][I - hts.begin()] << endl;
    } else {
        cout << "impossible" << endl;
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
