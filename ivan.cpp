// 2024-08-31
// We start by subtracting 1 from each number, so the cost of a move is just
// S1 * S2. Then, the basic observation is that an optimal move never takes more
// than 1 number from both arrays, because taking K1 >= 2 and K2 >= 2 numbers
// would always cost more than a sequence of two moves consisting of taking 1
// number from both arrays and then taking K1 - 1 and K2 - 1. This gives an
// O(L^3) algorithm, and by doing just a bit of algebra (explained below) we can
// optimize the transition to O(L^2) which will pass.
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
    dp[1][1] = A[0] * B[0];
    for (int j = 2; j <= N; j++) {
        dp[1][j] = dp[1][j - 1] + A[0] * B[j - 1];
    }
    vector<long long> C(N + 1, 3e9);
    for (int i = 2; i <= M; i++) {
        dp[i][1] = dp[i - 1][1] + A[i - 1] * B[0];
        long long D = 3e9;
        for (int j = 2; j <= N; j++) {
            // To determine
            // min_{1 <= i' < i} (dp[i'][j - 1] +
            //                    B[j - 1] sum_{i' <= i'' < i} A[i''])
            // we call this quantity C[i][j] and observe that
            // C[i][j] = min(C[i - 1][j] + A[i - 1] B[j - 1],
            //               dp[i - 1][j - 1] + A[i - 1] B[j - 1])
            C[j] = min(C[j], dp[i - 1][j - 1]) + A[i - 1] * B[j - 1];
            // Similarly, to determine
            // min_{1 <= j' < j} (dp[i - 1][j'] +
            //                    A[i - 1] sum_{j' <= j'' < j} B[j''])
            // we call this quantity D[i][j] and observe that
            // D[i][j] = min(D[i][j - 1] + A[i - 1] B[j - 1],
            //               dp[i - 1][j - 1] + A[i - 1] B[j - 1])
            D = min(D, dp[i - 1][j - 1]) + A[i - 1] * B[j - 1];
            dp[i][j] = min(C[j], D);
        }
    }
    cout << dp[M][N] << '\n';
}
