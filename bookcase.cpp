// 2025-08-16
// This problem looks easy but I got stuck because I thought the solution MUST
// have something to do with trying all possible combinations of shelf heights.
// So eventually I gave up and found the solution on a Chinese blog.  The idea
// is to compute dp[i][j][k], which is the minimum possible height of a
// two-shelved bookcase where the first shelf contains books with a total width
// of `j` and the second shelf contains books with a total width of `k`
// (exactly), and we can only use the first `i` books.  We loop over the books
// in reverse order of height and reuse a two-dimensional array.
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int N; cin >> N;
    vector<pair<int, int>> books(N);
    for (int i = 0; i < N; i++) {
        cin >> books[i].first >> books[i].second;
    }
    sort(books.begin(), books.end(), greater<>());
    vector<int> psum(N + 1);
    psum[0] = 0;
    for (int i = 0; i < N; i++) psum[i + 1] = psum[i] + books[i].second;
    constexpr int INF = 1e6;
    vector<vector<int>> dp(psum[N] + 1, vector<int>(psum[N] + 1, INF));
    dp[0][0] = 0;
    for (int i = 0; i < N; i++) {
        const int h = books[i].first;
        const int w = books[i].second;
        const int bound = psum[i];
        for (int j = bound; j >= 0; j--) {
            for (int k = bound - j; k >= 0; k--) {
                dp[j + w][k] = min(dp[j + w][k],
                                   dp[j][k] + (j == 0 ? h : 0));
                dp[j][k + w] = min(dp[j][k + w],
                                   dp[j][k] + (k == 0 ? h : 0));
            }
        }
    }
    int result = INF;
    for (int i = 1; i < psum[N]; i++) {
        for (int j = 1; i + j < psum[N]; j++) {
            if (dp[i][j] == INF) continue;
            const int width = max(max(i, j), psum[N] - i - j);
            const int height = dp[i][j] + books[0].first;
            result = min(result, width * height);
        }
    }
    cout << result << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
