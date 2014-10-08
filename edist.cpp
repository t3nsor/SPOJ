// 2014-04-25
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int T;
    cin >> T;
    while (T--) {
        string a, b;
        cin >> a >> b;
        vector<vector<int> > dp(a.length() + 1, vector<int>(b.length() + 1));
        for (int i = 0; i <= a.length(); i++) {
            for (int j = 0; j <= b.length(); j++) {
                if (i == 0) dp[i][j] = j;
                else if (j == 0) dp[i][j] = i;
                else if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1];
                else {
                    dp[i][j] = 1 + min(min(dp[i-1][j-1], dp[i][j-1]), dp[i-1][j]);
                }
            }
        }
        cout << dp[a.length()][b.length()] << endl;
    }
}
