// 2014-05-02
#include <iostream>
#include <cstdlib>
using namespace std;
int w[1000], h[1000];
int dp[1000][2];
int main() {
    int n; cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> h[i];
        if (i == 0) {
            dp[0][0] = w[0];
            dp[0][1] = h[0];
        }
        else {
            dp[i][0] = w[i] + max(dp[i-1][0] + abs(h[i] - h[i-1]),
                                  dp[i-1][1] + abs(h[i] - w[i-1]));
            dp[i][1] = h[i] + max(dp[i-1][0] + abs(w[i] - h[i-1]),
                                  dp[i-1][1] + abs(w[i] - w[i-1]));
        }
    }
    cout << max(dp[n-1][0], dp[n-1][1]) << endl;
}
