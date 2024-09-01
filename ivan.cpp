// 2024-08-31
// We start by subtracting 1 from each number, so the cost of a move is just
// S1 * S2. Then, the basic observation is that an optimal move never takes more
// than 1 number from both arrays, because taking K1 >= 2 and K2 >= 2 numbers
// would always cost more than a sequence of two moves consisting of taking 1
// number from both arrays and then taking K1 - 1 and K2 - 1. Naively trying all
// possible transitions gives an O(L^3) algorithm.
//
// Let dp[i][j] be the minimum cost starting from A[0...i-1] and B[0...j-1].
// Suppose we were to take 1 element from the former and K2 >= 2 elements from
// the latter. The cost of this would be the same as the cost of taking 1
// element from A[0...i-1] and K2 - 1 elements from B[0...j-2] and then adding
// A[i-1]*B[j-1]. But all possible values for K2 are already included in the
// computation of dp[i][j-1], so instead of iterating over all values of K2 >= 2
// we just take dp[i][j-1] + A[i-1]*B[j-1]. Similarly instead of iterating over
// all values of K1 >= 2 we just take dp[i-1][j] + A[i-1]*B[j-1]. And finally we
// take into account the case of K1 = K2 = 1.
#include <iostream>
#include <vector>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int M, N; cin >> M >> N;
    vector<int> A(M), B(N);
    for (int i = 0; i < M; i++) { cin >> A[i]; A[i]--; }
    for (int j = 0; j < N; j++) { cin >> B[j]; B[j]--; }
    vector<vector<long long>> dp(M + 1, vector<long long>(N + 1, 3e9));
    dp[0][0] = 0;
    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            dp[i][j] = dp[i - 1][j - 1];
            if (i > 1) dp[i][j] = min(dp[i][j], dp[i - 1][j]);
            if (j > 1) dp[i][j] = min(dp[i][j], dp[i][j - 1]);
            dp[i][j] += A[i - 1] * B[j - 1];
        }
    }
    cout << dp[M][N] << '\n';
}
