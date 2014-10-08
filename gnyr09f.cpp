// 2014-05-03
#include <iostream>
using namespace std;
int dp[101][100][2]; // initialized to 0
int main() {
    dp[1][0][0] = 1;
    dp[1][0][1] = 1;
    for (int n = 2; n <= 100; n++) {
        for (int k = 0; k < n; k++) {
            dp[n][k][0] = dp[n-1][k][0] + dp[n-1][k][1];
            dp[n][k][1] = dp[n-1][k][0];
            if (k > 0) dp[n][k][1] += dp[n-1][k-1][1];
        }
    }
    int P; cin >> P;
    while (P--) {
        int cs, n, k; scanf("%d %d %d", &cs, &n, &k);
        if (k < 0 || k >= n) {
            printf("%d 0\n", cs);
        } else {
            printf("%d %d\n", cs, dp[n][k][0] + dp[n][k][1]);
        }
    }
}
