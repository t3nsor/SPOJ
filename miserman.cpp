// 2014-04-26
#include <iostream>
#include <cstring>
using namespace std;
int dp[101][100];
int main() {
    int N, M;
    cin >> N >> M;
    memset(dp[0], 0, sizeof(dp[0]));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int val; cin >> val;
            dp[i+1][j] = val + dp[i][j];
            if (j > 0) dp[i+1][j] = min(dp[i+1][j], val + dp[i][j-1]);
            if (j < M-1) dp[i+1][j] = min(dp[i+1][j], val + dp[i][j+1]);
        }
    }
    int best = 1e9;
    for (int i = 0; i < M; i++) {
        best = min(best, dp[N][i]);
    }
    cout << best << endl;
    return 0;
}
