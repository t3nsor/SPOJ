// 2014-10-01
#include <iostream>
#include <cstring>
using namespace std;
int main() {
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        int dp[21][3];
        memset(dp, 0x3f, sizeof dp);
        dp[0][0] = dp[0][1] = dp[0][2] = 0;
        for (int i = 0; i < n; i++) {
            int a[3]; cin >> a[0] >> a[1] >> a[2];
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (j != k) {
                        dp[i+1][j] = min(dp[i+1][j], dp[i][k] + a[j]);
                    }
                }
            }
        }
        printf("%d\n", min(min(dp[n][0], dp[n][1]), dp[n][2]));
    }
    return 0;
}
