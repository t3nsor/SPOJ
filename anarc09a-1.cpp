// 2014-04-25
// DP approach (unnecessary! see anarc09a-2.cpp)
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
using namespace std;
char str[2001];
int main() {
    for (int cs = 1;; cs++) {
        scanf("%s", str);
        if (str[0] == '-') return 0;
        int N = strlen(str);
        vector<vector<int> > dp(N + 1, vector<int>(N + 1, 1e6));
        dp[0][0] = 0;
        for (int i = 1; i <= N; i++) {
            for (int j = 0; j <= N; j++) {
                if (str[i-1] == '{') {
                    dp[i][j] = 1 + dp[i-1][j+1];
                    if (j > 0) {
                        dp[i][j] = min(dp[i][j], dp[i-1][j-1]);
                    }
                } else {
                    dp[i][j] = dp[i-1][j+1];
                    if (j > 0) {
                        dp[i][j] = min(dp[i][j], 1 + dp[i-1][j-1]);
                    }
                }
            }
        }
        printf("%d. %d\n", cs, dp[N][0]);
    }
}
