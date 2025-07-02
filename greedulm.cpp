// 2025-07-02
// This is an easy O(n^3) DP. According to the official analysis, it is possible
// to improve the running time to O(n^2) (not shown in this program); the
// reference given is
//     Chapter 6.2.2, pp. 436-442 in:
//     Knuth, D.E.
//     The Art of Computer Programming, Volume 3: Sorting and Searching
//     Addison-Wesley, 2nd edition, 1998
//     ISBN 0-201-89685-0
// The idea is that some optimal value of `k` in the inner loop for given (i, j)
// can always be found between some optimal values of `k` for (i+1, j) and
// (i, j-1); it can be shown that limiting the search accordingly results in
// O(n^2) running time.
#include <iostream>
#include <vector>
using namespace std;
void do_testcase(int n) {
    vector<int> f(2*n + 1);
    vector<int> psum(2*n + 2);
    for (int i = 1; i < 2*n; i += 2) {
        cin >> f[i];
    }
    for (int i = 0; i <= 2*n; i += 2) {
        cin >> f[i];
    }
    psum[0] = 0;
    for (int i = 1; i <= 2*n + 1; i++) {
        psum[i] = psum[i - 1] + f[i - 1];
    }
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 1e9));
    for (int i = 0; i <= n; i++) {
        dp[i][i] = 0;
    }
    for (int l = 1; l <= n; l++) {
        for (int i = 0, j = i + l; j <= n; i++, j++) {
            for (int k = i; k < j; k++) {
                dp[i][j] = min(dp[i][j],
                               dp[i][k] + dp[k+1][j] + psum[2*j+1] - psum[2*i]);
            }
        }
    }
    cout << dp[0][n] << '\n';
}
int main() {
    for (;;) {
        int n; cin >> n;
        if (n == 0) return 0;
        do_testcase(n);
    }
}
