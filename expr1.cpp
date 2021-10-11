// 2021-10-10
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void do_testcase() {
    int n, k; cin >> n >> k; n /= 2;
    string expr; cin >> expr;
    // dp[i][j][m]: minimum number of left parens required to get a sum of m,
    // assuming that we've used the first i operands, and that we have a
    // current nesting depth of j (i.e., left parens minus right parens)
    vector<vector<vector<int>>> dp(n + 1);
    for (int i = 0; i <= n; i++) {
        dp[i].assign(n + 1, vector<int>(361, 1000));
        if (i == 0) {
            dp[i][0][180] = 0;
            continue;
        }
        const bool minus = expr[2 * i - 2] == '-';
        // let j be the nesting depth after the first i - 1 operands; then the
        // current operand is negated if j is odd
        for (int j = 0; j <= n; j++) {
            int factor = minus ? -1 : +1;
            if (j % 2) {
                factor *= -1;
            }
            const int inc = factor * (expr[2 * i - 1] - '0');
            for (int m = 0; m <= 360; m++) {
                if (m - inc < 0 || m - inc > 360) continue;
                dp[i][j][m] = min(dp[i][j][m], dp[i - 1][j][m - inc]);
            }
        }
        // we only need to add 1 nesting level at a time, and only if -
        if (minus) {
            for (int j = 1; j <= n; j++) {
                for (int m = 0; m <= 360; m++) {
                    dp[i][j][m] = min(dp[i][j][m], dp[i][j - 1][m] + 1);
                }
            }
        }
        // add any number of right parens
        for (int m = 0; m <= 360; m++) {
            int best = 2000;
            for (int j = n; j >= 0; j--) {
                best = min(best, dp[i][j][m]);
                dp[i][j][m] = best;
            }
        }
    }
    if (dp[n][0][k + 180] < 1000) {
        cout << dp[n][0][k + 180];
    } else {
        cout << "NO";
    }
    cout << '\n';
}
int main() {
    int d; cin >> d;
    while (d--) do_testcase();
}
