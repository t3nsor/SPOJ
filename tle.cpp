// 2026-09-30
// despite its name this problem does not appear to actually be that hard to get
// accepted
// idea is similar to KOSARE, we must have a fast way of transforming an array
// of size 2^M into an array where each element a[i] has been replaced by the
// sum of all a[j] where j's set bits are a subset of i's
#include <iostream>
#include <utility>
using namespace std;
constexpr int MOD = 1000000000;
int dp[51][32768];
void do_testcase() {
    int N, M; cin >> N >> M;
    for (int i = 1; i <= N; i++) {
        for (int j = 0; j < (1 << M); j++) {
            dp[i][j] = dp[i - 1][j];
        }
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < (1 << (M - 1)); k++) {
                const int lo = k & ((1 << j) - 1);
                const int hi = (k >> j) << (j + 1);
                dp[i][lo+(1<<j)+hi] = (dp[i][lo+(1<<j)+hi] +
                                       dp[i][lo+hi]) % MOD;
            }
        }
        for (int j = 0; j < (1 << (M - 1)); j++) {
            swap(dp[i][j], dp[i][(1 << M) - j - 1]);
        }
        int c; cin >> c;
        for (int j = 0; j < (1 << M); j += c) dp[i][j] = 0;
    }
    int result = 0;
    for (int i = 0; i < (1 << M); i++) {
        result = (result + dp[N][i]) % MOD;
    }
    cout << result << '\n';
}
int main() {
    dp[0][0] = 1;
    int T; cin >> T; while (T--) do_testcase();
}
