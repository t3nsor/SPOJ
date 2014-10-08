// 2014-05-27
#include <iostream>
using namespace std;
int dp[1000][3];
int main() {
    int N; cin >> N;
    dp[1][0] = dp[1][1] = dp[1][2] = 1;
    for (int i = 2; i <= N; i++) {
        dp[i][0] = dp[i-1][0] + dp[i-1][1];
        dp[i][1] = dp[i-1][0] + dp[i-1][1] + dp[i-1][2];
        dp[i][2] = dp[i-1][1] + dp[i-1][2];
    }
    cout << dp[N][0] + dp[N][1] + dp[N][2] << endl;
}
