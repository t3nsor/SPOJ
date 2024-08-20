// 2024-08-20
// Let `x` be a positive integer that's divisible by neither 2 nor 3. Let S(x)
// denote the set of all 2^p 3^q x (where p, q are nonnegative integers). For
// each `x`, the set S(x) intersect {1, 2, ..., n} can be considered
// independently and we multiply to get the final result.
//
// We can visualize a single one of these subsets as a Young diagram where the
// first row is {x, 3x, 9x, ...} and each subsequent row is the previous row
// multiplied by 2 (with values greater than n removed). What we are trying to
// compute is the number of ways to colour this diagram black and white such
// that no two cells that share an edge are both black. This can be done using
// dynamic programming where dp[i] is the number of ways to colour the current
// row according to the mask `i`. To optimize, we precompute all valid masks up
// to the max row size (11). The value of dp[i] is given by the sum of all dp[j]
// from the previous row where `i` and `j` have no common bits.
//
// Note that choosing `x` times powers of 3 to form the initial row minimizes
// the length of the row (compared with powers of 2), which is important because
// the dp complexity is proportional to 2 to the power of the row length.
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
const int MOD = 1000000001;
vector<vector<int>> masks(12);
void calc_masks(int l) {
    for (int i = 0; i < (1 << l); i++) {
        bool fail = false;
        for (int j = 0; j + 1 < l; j++) {
            if ((3 << j) == (i & (3 << j))) {
                fail = true;
                break;
            }
        }
        if (!fail) masks[l].push_back(i);
    }
}
int calc(int x, int n, vector<int>& vis) {
    vector<int> row = {x};
    vis[x] = 1;
    while (row.back() * 3 <= n) {
        row.push_back(row.back() * 3);
        vis[row.back()] = 1;
    }
    vector<int> dp(1 << (row.size()), 0);
    for (int mask : masks[row.size()]) {
        dp[mask] = 1;
    }
    while (2 * row.front() <= n) {
        vector<int> row2;
        for (int i = 0; i < row.size() && 2*row[i] <= n; i++) {
            row2.push_back(2 * row[i]);
            vis[row2.back()] = 1;
        }
        vector<int> dp2(1 << (row2.size()), 0);
        for (int mask2 : masks[row2.size()]) {
            for (int mask : masks[row.size()]) {
                if (mask & mask2) continue;
                dp2[mask2] = (dp2[mask2] + dp[mask]) % MOD;
            }
        }
        swap(row, row2);
        swap(dp, dp2);
    }
    return accumulate(dp.begin(), dp.end(), 0LL) % MOD;
}
void do_testcase() {
    int n; cin >> n;
    vector<int> vis(n + 1, 0);
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            result = (result * calc(i, n, vis)) % MOD;
        }
    }
    cout << result << '\n';
}
int main() {
    for (int i = 1; i <= 11; i++) calc_masks(i);
    int T; cin >> T;
    while (T--) do_testcase();
}
