// 2023-04-02
#include <iostream>
using namespace std;
// dp[i][j][k] is the number of partitions of `i` into `j` components, such that
// the first number in the partition is at least `k`
long long dp[221][221][221];
void do_testcase() {
    int i, n; long long idx; cin >> i >> n >> idx; --idx;
    int last = 1;
    for (int j = n; j >= 1; j--) {
        int k = last + 1;
        while (dp[i][j][last] - dp[i][j][k] <= idx) k++;
        idx -= (dp[i][j][last] - dp[i][j][k - 1]);
        i -= k - 1;
        last = k - 1;
        cout << k - 1;
        cout << (j == 1 ? '\n' : ' ');
    }
}
int main() {
    // compute partitions
    for (int i = 1; i <= 220; i++) {
        for (int j = 1; j <= i; j++) {
            for (int k = i; k >= 1; k--) {
                if (k == i) {
                    dp[i][j][k] = (j == 1);
                } else {
                    dp[i][j][k] += dp[i][j][k + 1];
                    dp[i][j][k] += dp[i - k][j - 1][k];
                }
            }
        }
    }
    int t; cin >> t;
    while (t--) do_testcase();
}
