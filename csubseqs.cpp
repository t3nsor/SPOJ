// 2026-09-30
// inclusion-exclusion bash
#include <iostream>
#include <string>
#include <string.h>
using namespace std;
using LL = long long;
/// P[i][j][k][l] is the sum of dp[i2][j2][k2][l2] where 0 <= i2 < i, etc.
LL P[52][52][52][52];
/// dp[i][j][k][l] is the number of common substrings whose last character is
/// at indices i-1,j-1,k-1,l-1 in the 4 strings, respectively, and that cannot
/// be formed as a common substring by using only earlier characters from one
/// string and earlier or equal characters from the other three strings
LL dp[51][51][51][51];
/// L[i][j] is 0 if s[i][j - 1] does not appear earlier in s[i], and otherwise
/// is 1 plus the last index at which it appears
int L[4][51];
string s[4];
int main() {
    for (int i = 0; i < 4; i++) {
        cin >> s[i];
        for (int j = 0; j < s[i].size(); j++) {
            for (int k = j - 1; k >= 0; k--) {
                if (s[i][j] == s[i][k]) { L[i][j + 1] = k + 1; break; }
            }
        }
    }
    dp[0][0][0][0] = 1;
    LL result = 0;
    for (int i = 1; i <= s[0].size(); i++) {
        for (int j = 1; j <= s[1].size(); j++) {
            for (int k = 1; k <= s[2].size(); k++) {
                for (int l = 1; l <= s[3].size(); l++) {
                    P[i][j][k][l] = dp[i-1][j-1][k-1][l-1]
                                  + P[i][j][k][l-1]
                                  + P[i][j][k-1][l]
                                  - P[i][j][k-1][l-1]
                                  + P[i][j-1][k][l]
                                  - P[i][j-1][k][l-1]
                                  - P[i][j-1][k-1][l]
                                  + P[i][j-1][k-1][l-1]
                                  + P[i-1][j][k][l]
                                  - P[i-1][j][k][l-1]
                                  - P[i-1][j][k-1][l]
                                  + P[i-1][j][k-1][l-1]
                                  - P[i-1][j-1][k][l]
                                  + P[i-1][j-1][k][l-1]
                                  + P[i-1][j-1][k-1][l]
                                  - P[i-1][j-1][k-1][l-1];
                    if (s[0][i - 1] == s[1][j - 1] &&
                        s[0][i - 1] == s[2][k - 1] &&
                        s[0][i - 1] == s[3][l - 1]) {
                        const int i2 = L[0][i];
                        const int j2 = L[1][j];
                        const int k2 = L[2][k];
                        const int l2 = L[3][l];
                        dp[i][j][k][l] = P[i][j][k][l]
                                       - P[i][j][k][l2]
                                       - P[i][j][k2][l]
                                       + P[i][j][k2][l2]
                                       - P[i][j2][k][l]
                                       + P[i][j2][k][l2]
                                       + P[i][j2][k2][l]
                                       - P[i][j2][k2][l2]
                                       - P[i2][j][k][l]
                                       + P[i2][j][k][l2]
                                       + P[i2][j][k2][l]
                                       - P[i2][j][k2][l2]
                                       + P[i2][j2][k][l]
                                       - P[i2][j2][k][l2]
                                       - P[i2][j2][k2][l]
                                       + P[i2][j2][k2][l2];
                        result += dp[i][j][k][l];
                    }
                }
            }
        }
    }
    cout << result << '\n';
}
