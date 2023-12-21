// 2023-12-20
#include <algorithm>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
int dp[3831][3831][2];
int dp2[3831][3831][2];
void do_testcase() {
    int N, B; cin >> N >> B;
    vector<int> U(N);
    for (int i = 0; i < N; i++) cin >> U[i];
    // dp[i][j][k] = maximum sleep utility from first `i` time periods if a
    // total of `j` periods were spent sleeping, and `k` is 1 if and only if
    // period `j-1` was spent sleeping.
    memset(dp, 0x80, sizeof(dp));
    dp[0][0][0] = 0;
    for (int i = 1; i <= N; i++) {
        for (int j = 0; j <= B; j++) {
            dp[i][j][0] = max(dp[i-1][j][0], dp[i-1][j][1]);
            if (j > 0) {
                dp[i][j][1] = max(dp[i-1][j-1][0],
                                  dp[i-1][j-1][1] + U[i-1]);
            }
        }
    }
    // now do it again to account for the wraparound
    memset(dp2, 0x80, sizeof(dp2));
    dp2[1][1][1] = U[0];
    for (int i = 2; i <= N; i++) {
        for (int j = 0; j <= B; j++) {
            dp2[i][j][0] = max(dp2[i-1][j][0], dp2[i-1][j][1]);
            if (j > 0) {
                dp2[i][j][1] = max(dp2[i-1][j-1][0],
                                   dp2[i-1][j-1][1] + U[i-1]);
            }
        }
    }
    int result = -2e9;
    for (int i = 1; i <= B; i++) {
        result = max(result, dp[N][i][0]);
        result = max(result, dp[N][i][1]);
        result = max(result, dp2[N][i][1]);
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
