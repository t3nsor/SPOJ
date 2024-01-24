// 2024-01-23
// After compressing coordinates, this is a run of the mill 2D DP where the
// state is (i, j) where all cows in columns up to `i` have been covered, using
// `j` barns. ... Except that we also need a third piece of information in the
// DP state that describes the geometry of the last column covered: does a
// single barn cover both cells in the column, or are both cells covered by
// different barns, or is only one of the two cells covered? That way we know
// the cost to possibly extend the one or two barns from the last column when
// performing the transition.
#include <algorithm>
#include <array>
#include <stdio.h>
#include <vector>
enum EndType {
    TOP_ONLY,
    BOTTOM_ONLY,
    TOGETHER,
    SEPARATE,
    NUM_END_TYPES
};
using namespace std;

template <class T, class U>
void assign_min(T& x, U y) {
    if (y < x) x = y;
}

void do_testcase() {
    int N, K, B; scanf("%d %d %d", &N, &K, &B);
    vector<int> row(N);
    vector<int> orig_col(N);
    // compress coordinates
    vector<pair<int, int>> v;
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &row[i], &orig_col[i]);
        v.emplace_back(orig_col[i], i);
    }
    sort(v.begin(), v.end());
    vector<int> a[2];
    vector<int> uncompress;
    for (int i = 0; i < N; i++) {
        if (i == 0 || v[i].first > v[i - 1].first) {
            a[0].push_back(0);
            a[1].push_back(0);
            uncompress.push_back(v[i].first);
        }
        a[row[v[i].second] - 1].back() = 1;
    }
    N = uncompress.size();
    using EndTypeArray = array<int, NUM_END_TYPES>;
    EndTypeArray infinite_array;
    fill(infinite_array.begin(), infinite_array.end(), 1e8);
    vector<vector<EndTypeArray>> dp(N, vector<EndTypeArray>(K + 1,
                                                            infinite_array));
    if (a[0][0] && a[1][0]) {
        dp[0][1][TOGETHER] = 2;
        if (K >= 2) {
            dp[0][2][SEPARATE] = 2;
        }
    } else if (a[0][0]) {
        dp[0][1][TOP_ONLY] = 1;
        dp[0][1][TOGETHER] = 2;
        if (K >= 2) {
            dp[0][2][SEPARATE] = 2;
        }
    } else {
        dp[0][1][BOTTOM_ONLY] = 1;
        dp[0][1][TOGETHER] = 2;
        if (K >= 2) {
            dp[0][2][SEPARATE] = 2;
        }
    }
    for (int i = 1; i < N; i++) {
        const int width = uncompress[i] - uncompress[i - 1];
        for (int j = 1; j <= K; j++) {
            const int min1 = min({dp[i - 1][j - 1][TOP_ONLY],
                                  dp[i - 1][j - 1][BOTTOM_ONLY],
                                  dp[i - 1][j - 1][TOGETHER],
                                  dp[i - 1][j - 1][SEPARATE]});
            // start a new rectangle of height 2
            assign_min(dp[i][j][TOGETHER], 2 + min1);
            // extend a rectangle of height 2
            assign_min(dp[i][j][TOGETHER],
                       2 * width + dp[i - 1][j][TOGETHER]);
            if (j >= 2) {
                // start 2 new rectangles
                assign_min(dp[i][j][SEPARATE],
                           2 +  min({dp[i - 1][j - 2][TOP_ONLY],
                                     dp[i - 1][j - 2][BOTTOM_ONLY],
                                     dp[i - 1][j - 2][TOGETHER],
                                     dp[i - 1][j - 2][SEPARATE]}));
            }
            // extend 2 existing rectangles
            assign_min(dp[i][j][SEPARATE], 2 * width + dp[i - 1][j][SEPARATE]);
            // extend existing rectangle from top or bottom and start 1 new
            // rectangle at bottom or top (respectively)
            assign_min(dp[i][j][SEPARATE],
                       1 + width + min({dp[i - 1][j - 1][TOP_ONLY],
                                        dp[i - 1][j - 1][BOTTOM_ONLY],
                                        dp[i - 1][j - 1][SEPARATE]}));
            if (!a[0][i]) {
                // start a new rectangle only at the bottom
                assign_min(dp[i][j][BOTTOM_ONLY], 1 + min1);
                // extend existing rectangle at bottom
                assign_min(dp[i][j][BOTTOM_ONLY],
                           width + min(dp[i - 1][j][BOTTOM_ONLY],
                                       dp[i - 1][j][SEPARATE]));
            }
            if (!a[1][i]) {
                // start a new rectangle only at the top
                assign_min(dp[i][j][TOP_ONLY], 1 + min1);
                // extend existing rectangle at bottom
                assign_min(dp[i][j][TOP_ONLY],
                           width + min(dp[i - 1][j][TOP_ONLY],
                                       dp[i - 1][j][SEPARATE]));
            }
        }
    }
    int best = 1e8;
    for (int j = 0; j <= K; j++) {
        assign_min(best, dp[N - 1][j][TOP_ONLY]);
        assign_min(best, dp[N - 1][j][BOTTOM_ONLY]);
        assign_min(best, dp[N - 1][j][TOGETHER]);
        assign_min(best, dp[N - 1][j][SEPARATE]);
    }
    printf("%d\n", best);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}

