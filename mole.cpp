// 2025-05-29
// This problem is easy once you know the trick that's explained in the comments
// section of the problem description. The hammer is allowed to go outside the
// n by n rectangle that the moles appear in. It's easy to convince yourself
// that an optimal solution can always be obtained without having to move the
// hammer more than d distance units away from the n by n rectangle: if you ever
// moved the hammer so far away that it wouldn't be able to reach any mole in
// next time step, there is an alternative path that whacks the same number of
// moles where the hammer doesn't leave the expanded rectangle but instead
// always goes to the point lying inside the expanded rectangle that is closest
// to where it would have been outside the expanded rectangle.
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
int gcd(int x, int y) {
    if (!x) return y; else return gcd(y%x, x);
}
void do_testcase(int n, int d, int m) {
    vector<vector<pair<int, int>>> paths;
    for (int i = -d; i <= d; i++) {
        for (int j = -d; j <= d; j++) {
            if (i*i + j*j > d*d || gcd(abs(i), abs(j)) != 1) continue;
            vector<pair<int, int>> path = {{0, 0}};
            int ii = i, jj = j;
            do {
                path.emplace_back(ii, jj);
                ii += i;
                jj += j;
            } while (ii*ii + jj*jj <= d*d);
            paths.push_back(move(path));
        }
    }
    const int N = n + 2*d;
    vector<vector<vector<int>>> mole(11);
    for (int i = 0; i <= 10; i++) {
        mole[i].assign(N, vector<int>(N, 0));
    }
    while (m--) {
        int x, y, t; cin >> x >> y >> t;
        x += d; y += d;
        mole[t][x][y] = 1;
    }
    vector<vector<int>> dp(N, vector<int>(N, 0));
    for (int i = 1; i <= 10; i++) {
        vector<vector<int>> dp2(N, vector<int>(N, 0));
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                for (const auto& path : paths) {
                    int val = dp[r][c];
                    for (const auto& vec : path) {
                        const int r2 = r + vec.first;
                        const int c2 = c + vec.second;
                        if (r2 >= 0 && r2 < N && c2 >= 0 && c2 < N) {
                            val += mole[i][r2][c2];
                            dp2[r2][c2] = max(dp2[r2][c2], val);
                        } else break;
                    }
                }
            }
        }
        swap(dp, dp2);
    }
    int result = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result = max(result, dp[i][j]);
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int n, d, m; cin >> n >> d >> m;
        if (n == 0) return 0;
        do_testcase(n, d, m);
    }
}
