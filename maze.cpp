// 2023-04-04
#include <array>
#include <cctype>
#include <cstdio>
#include <vector>
using namespace std;
enum Pattern {
    EMPTY = 0,
    STRAIGHT = 1,
    BENT = 2,
};
enum Edge {
    TOP,     // |->
             // |
             // |

    MIDDLE,  // |
             // |->
             // |
             
    BOTTOM,  // |
             // |
             // |->

    SNAKE,   // |->
             // |<-
             // |->
};
void do_testcase() {
    int n; scanf("%d", &n);
    vector<array<Pattern, 3>> grid(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            char c;
            do {
                c = getchar();
            } while (isspace(c));
            grid[i][j] = static_cast<Pattern>(c - '0');
        }
    }
    vector<array<bool, 4>> dp(n + 1);
    dp[0][TOP] = dp[0][MIDDLE] = dp[0][BOTTOM] = true;
    dp[0][SNAKE] = false;
    for (int i = 1; i <= n; i++) {
        dp[i][TOP] = (dp[i-1][TOP] && grid[i-1][0] == STRAIGHT) ||
                     (dp[i-1][MIDDLE] && grid[i-1][0] == BENT &&
                                         grid[i-1][1] == BENT) ||
                     (dp[i-1][BOTTOM] && grid[i-1][0] == BENT &&
                                         grid[i-1][1] == STRAIGHT &&
                                         grid[i-1][2] == BENT) ||
                     (dp[i-1][SNAKE] && grid[i-1][0] == STRAIGHT &&
                                        grid[i-1][1] == BENT &&
                                        grid[i-1][2] == BENT);
        dp[i][MIDDLE] = (dp[i-1][TOP] && grid[i-1][0] == BENT &&
                                         grid[i-1][1] == BENT) ||
                        (dp[i-1][MIDDLE] && grid[i-1][1] == STRAIGHT) ||
                        (dp[i-1][BOTTOM] && grid[i-1][1] == BENT &&
                                            grid[i-1][2] == BENT);
        dp[i][BOTTOM] = (dp[i-1][TOP] && grid[i-1][0] == BENT &&
                                         grid[i-1][1] == STRAIGHT &&
                                         grid[i-1][2] == BENT) ||
                        (dp[i-1][MIDDLE] && grid[i-1][1] == BENT &&
                                            grid[i-1][2] == BENT) ||
                        (dp[i-1][BOTTOM] && grid[i-1][2] == STRAIGHT) ||
                        (dp[i-1][SNAKE] && grid[i-1][0] == BENT &&
                                           grid[i-1][1] == BENT &&
                                           grid[i-1][2] == STRAIGHT);
        dp[i][SNAKE] = (dp[i-1][TOP] && grid[i-1][0] == STRAIGHT &&
                                        grid[i-1][1] == BENT &&
                                        grid[i-1][2] == BENT) ||
                       (dp[i-1][BOTTOM] && grid[i-1][0] == BENT &&
                                           grid[i-1][1] == BENT &&
                                           grid[i-1][2] == STRAIGHT) ||
                       (dp[i-1][SNAKE] && grid[i-1][0] == STRAIGHT &&
                                          grid[i-1][1] == STRAIGHT &&
                                          grid[i-1][2] == STRAIGHT);
    }
    if (dp[n][TOP] || dp[n][MIDDLE] || dp[n][BOTTOM]) {
        puts("yes");
    } else {
        puts("no");
    }
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
