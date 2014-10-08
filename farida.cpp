// 2014-05-03
#include <cstdio>
#include <algorithm>
using namespace std;
long long dp[10001][2];
int main() {
    int T; scanf("%d", &T);
    for (int cs = 1; cs <= T; cs++) {
        int N; scanf("%d", &N);
        dp[0][0] = dp[0][1] = 0;
        for (int i = 0; i < N; i++) {
            int x; scanf("%d", &x);
            dp[i+1][0] = max(dp[i][0], dp[i][1]);
            dp[i+1][1] = x + dp[i][0];
        }
        printf("Case %d: %lld\n", cs, max(dp[N][0], dp[N][1]));
    }
    return 0;
}
