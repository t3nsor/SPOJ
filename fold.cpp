// 2025-08-03
// I found this problem pretty hard to think about, but easy to code.
// Let dp[i][j] be the minimum number of folds required to produce the turns i,
// i + 1, ..., j - 1. To compute dp[i][j] we try all possible locations of the
// first fold. After the first fold, there will be two layers, one of which may
// be shorter than the other. Henceforth, whatever we do to the common part, the
// turns produced in the shorter layer will be opposite to those produced in the
// longer layer (and also reversed in order). This implies that the choice of
// the initial fold location is admissible only if it splits the sequence into
// two sequences where the shorter one, reversed, equals the substring of the
// same length of the longer one that is closest to the split point, and the
// remaining number of folds required equals the number of folds required to
// produce the longer part.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int dp[200][200];
bool match[200][200][200];  // length and starting indices
void do_testcase(string s) {
    int n = s.length();
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            match[0][i][j] = true;
        }
    }
    for (int l = 1; l <= n; l++) {
        for (int i = 0; i + l <= n; i++) {
            for (int j = 0; j + l <= n; j++) {
                match[l][i][j] = match[l-1][i+1][j] && s[i] != s[j+l-1];
            }
        }
    }
    for (int i = 0; i <= n; i++) {
        dp[i][i] = 0;
    }
    for (int l = 1; l <= n; l++) {
        for (int i = 0; i + l <= n; i++) {
            const int j = i + l;
            dp[i][j] = 200;
            for (int k = i; k < j; k++) {
                const int l1 = k - i;
                const int l2 = l - l1 - 1;
                if (l1 <= l2 && match[l1][i][k+1]) {
                    dp[i][j] = min(dp[i][j], 1 + dp[k+1][j]);
                } else if (l1 > l2 && match[l2][k+1][k-l2]) {
                    dp[i][j] = min(dp[i][j], 1 + dp[i][k]);
                }
            }
        }
    }
    cout << dp[0][n] << '\n';
}
int main() {
    string s;
    while (cin >> s) {
        do_testcase(s);
    }
}
