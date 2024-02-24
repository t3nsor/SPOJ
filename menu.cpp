// 2024-02-23
#include <algorithm>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
int dp[22][51][101];
void do_testcase(int K, int N, int M) {
    memset(dp, 0xc0, sizeof(dp));
    dp[0][N][0] = 0;  // imaginary distinct dish cooked on day 0
    vector<int> cost(N);
    vector<int> benefit(N);
    for (int i = 0; i < N; i++) {
        cin >> cost[i] >> benefit[i];
    }
    for (int i = 1; i <= K; i++) {
        for (int j = 0; j < N; j++) {
            for (int m = 0; m <= N; m++) {
                if (m == j) {
                    // We can always just re-cook the last dish and assume that
                    // it gives us no extra utility
                    for (int k = cost[j]; k <= M; k++) {
                        dp[i][j][k] = max(dp[i][j][k],
                                          dp[i - 1][j][k - cost[j]]);
                    }
                    continue;
                }
                for (int k = cost[j]; k <= M; k++) {
                    dp[i][j][k] = max(dp[i][j][k],
                                      dp[i - 1][m][k - cost[j]] + 2*benefit[j]);
                }
                if (i == 1) continue;
                for (int k = 2*cost[j]; k <= M; k++) {
                    dp[i][j][k] =
                      max(dp[i][j][k],
                          dp[i - 2][m][k - 2*cost[j]] + 3*benefit[j]);
                }
            }
        }
    }
    vector<int> menu(K);
    int max_benefit = -1;
    int min_cost;
    int last_dish;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= M; j++) {
            if (dp[K][i][j] > max_benefit ||
                (dp[K][i][j] == max_benefit && j < min_cost)) {
                max_benefit = dp[K][i][j];
                min_cost = j;
                last_dish = i;
            }
        }
    }
    if (max_benefit < 0) {
        cout << "0.0\n";
        return;
    }
    int day = K;
    while (day > 0) {
        menu[day - 1] = last_dish;
        if (dp[day][last_dish][min_cost] ==
            dp[day - 1][last_dish][min_cost - cost[last_dish]]) {
            min_cost -= cost[last_dish];
            --day;
            continue;
        }
        for (int i = 0; i <= N; i++) {
            if (i == last_dish) continue;
            if (dp[day][last_dish][min_cost] ==
                dp[day - 1][i][min_cost - cost[last_dish]] +
                  2*benefit[last_dish]) {
                --day;
                min_cost -= cost[last_dish];
                last_dish = i;
                break;
            }
            if (day >= 2 && min_cost >= 2*cost[last_dish] &&
                dp[day][last_dish][min_cost] ==
                dp[day - 2][i][min_cost - 2*cost[last_dish]] +
                  3*benefit[last_dish]) {
                menu[day - 2] = last_dish;
                day -= 2;
                min_cost -= 2*cost[last_dish];
                last_dish = i;
                break;
            }
        }
    }
    if (max_benefit % 2 == 0) {
        cout << max_benefit / 2 << ".0\n";
    } else {
        cout << max_benefit / 2 << ".5\n";
    }
    for (int i = 0; i < K; i++) {
        if (i > 0) cout << ' ';
        cout << menu[i] + 1;
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int k, n, m; cin >> k >> n >> m;
        if (k == 0) return 0;
        do_testcase(k, n, m);
        cout << '\n';
    }
}
