// 2024-08-19
// dp[i][j] (where i < j) is the minimum total cost such that one employee is at
// the currently required location while the other two are at locations i, j.
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
void minimize(int& x, int y) {
    if (x > y) x = y;
}
void do_testcase() {
    int L, N; cin >> L >> N;
    vector<vector<int>> dp(L, vector<int>(L, 3e6));
    vector<vector<int>> C(L, vector<int>(L));
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            cin >> C[i][j];
        }
    }
    int l; cin >> l; --l;
    if (l != 1 && l != 2) dp[1][2] = C[0][l];
    if (l != 0 && l != 2) dp[0][2] = C[1][l];
    if (l != 0 && l != 1) dp[0][1] = C[2][l];
    for (int i = 1; i < N; i++) {
        int l2; cin >> l2; --l2;
        if (l2 != l) {
            vector<vector<int>> dp2(L, vector<int>(L, 3e6));
            for (int j = 0; j < L; j++) {
                if (j == l) continue;
                for (int k = 0; k < j; k++) {
                    if (k == l) continue;
                    // transition from (j, k, l) to:
                    // * (l2, k, l)
                    // * (j, l2, l)
                    // * (j, k, l2)
                    if (k != l2) {
                        minimize(dp2[min(k, l)][max(k, l)],
                                 dp[k][j] + C[j][l2]);
                    }
                    if (j != l2) {
                        minimize(dp2[min(j, l)][max(j, l)],
                                 dp[k][j] + C[k][l2]);
                    }
                    if (k != l2 && j != l2) {
                        minimize(dp2[k][j], dp[k][j] + C[l][l2]);
                    }
                }
            }
            swap(dp, dp2);
        }
        // else: there is already someone at this location; a second employee
        // may not move to this location and "They can only move on request"
        // suggests that an employee cannot start moving now in an attempt to
        // reduce the cost of getting to the next request after this one
        l = l2;
    }
    int result = 3e6;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < i; j++) {
            minimize(result, dp[j][i]);
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
