// 2024-07-26
#include <iostream>
#include <string.h>
using namespace std;
long long dp[32768][64][71];  // # blocks, height, # blocks on last level
long long calc(int n, int h, int m) {
    if (n < 0) return 0;
    if (h == 0) return 1;
    long long& result = dp[n][h][m];
    if (result == -1) {
        result = calc(n - m - 1, h - 1, m + 1);
        if (m > 1) result += calc(n - m + 1, h - 1, m - 1);
    }
    return result;
}
int main() {
    memset(dp, -1, sizeof(dp));
    int T; cin >> T;
    while (T--) {
        int N, H, M; cin >> N >> H >> M;
        cout << calc(N - M, H - 1, M) << '\n';
    }
}
