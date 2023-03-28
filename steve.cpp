// 2023-03-27
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int n, m; cin >> n >> m;
    // The game state is a triple (p, i, j, k) where
    // * `i` is the number of donuts that the game started with (can be less
    //   than `n` since the game can restart)
    // * `j` is the number of donuts gathered on Steve's side
    // * `k` is the number of donuts gathered on Digit's side
    // We use dp1[i][j][k] to represent the max number of donuts Steve will eat
    // if it's currently his turn, and dp2[i][j][k] the max number of donuts
    // Steve will eat if it's currently Digit's turn.
    vector<vector<vector<int>>> dp1(n + 1,
                                    vector<vector<int>>(n + 1,
                                                        vector<int>(n + 1,
                                                                    0)));
    auto dp2 = dp1;
    for (int i = 1; i <= n; i++) {
        for (int r = 0; r <= i; r++) {
            // r is the number of remaining donuts
            for (int j = 0; j + r <= i; j++) {
                const int k = i - j - r;
                if (r == 0) {
                    dp1[i][j][k] = dp1[j][0][0];
                    dp2[i][j][k] = j + dp2[k][0][0];
                } else {
                    dp2[i][j][k] = 1000;
                }
                for (int q = 1; q <= m && q <= r; q++) {
                    dp1[i][j][k] = max(dp1[i][j][k],
                                       dp2[i][j+q][k]);
                    dp2[i][j][k] = min(dp2[i][j][k],
                                       dp1[i][j][k+q]);
                }
            }
        }
    }
    cout << dp1[n][0][0] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
