#include <cstring>
#include <iostream>
#include <utility>
using namespace std;
void do_testcase() {
    int N, M; cin >> N >> M;
    int dim[100][3][3];
    for (int i = N - 1; i >= 0; i--) {
        auto& r = dim[i];
        cin >> r[0][0] >> r[0][1] >> r[0][2];
        r[2][0] = r[1][1] = r[0][2];
        r[2][1] = r[1][2] = r[0][0];
        r[2][2] = r[1][0] = r[0][1];
        if (r[0][0] > r[0][1]) swap(r[0][0], r[0][1]);
        if (r[1][0] > r[1][1]) swap(r[1][0], r[1][1]);
        if (r[2][0] > r[2][1]) swap(r[2][0], r[2][1]);
    }
    int dp[101][101][3];  // # of cuboids, # of piles, last orientation
    memset(dp, 0x80, sizeof(dp));
    dp[0][0][0] = dp[0][0][1] = dp[0][0][2] = 0;
    int best = -1e9;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            // try starting new pile
            int x = -1e9;
            for (int k = 0; k < i; k++) {
                for (int q = 0; q < 3; q++) {
                    x = max(x, dp[k][j - 1][q]);
                }
            }
            for (int p = 0; p < 3; p++) {
                dp[i][j][p] = max(dp[i][j][p], x + dim[i - 1][p][2]);
            }
            // try adding onto existing pile; we must be larger
            for (int k = 1; k < i; k++) {
                for (int p = 0; p < 3; p++) {
                    for (int q = 0; q < 3; q++) {
                        if (dim[i - 1][p][0] >= dim[k - 1][q][0] &&
                            dim[i - 1][p][1] >= dim[k - 1][q][1]) {
                            dp[i][j][p] = max(dp[i][j][p],
                                              dp[k][j][q] + dim[i - 1][p][2]);
                        }
                    }
                }
            }
        }
        for (int p = 0; p < 3; p++) best = max(best, dp[i][M][p]);
    }
    cout << best << '\n';
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
