// 2014-05-01
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
const int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
const int dy[8] = {1, 0, -1, 1, -1, 1, 0, -1};
int main() {
    for (int cs = 1;; cs++) {
        int R, C; cin >> R >> C; if (R==0) return 0;
        vector<string> V(R);
        vector<vector<int> > dp(R, vector<int>(C, -1e9));
        for (int i = 0; i < R; i++) {
            cin >> V[i];
        }
        int res = 0;
        for (char c = 'A'; c <= 'Z'; c++) {
            for (int i = 0; i < R; i++) {
                for (int j = 0; j < C; j++) {
                    if (V[i][j] != c) continue;
                    if (c == 'A') dp[i][j] = 1;
                    for (int k = 0; k < 8; k++) {
                        int r = i + dx[k], c = j + dy[k];
                        if (r >= 0 && c >= 0 && r < R && c < C &&
                            V[r][c] == V[i][j] - 1) {
                            dp[i][j] = max(dp[i][j], dp[r][c] + 1);
                        }
                    }
                    res = max(res, dp[i][j]);
                }
            }
        }
        cout << "Case " << cs << ": " << res << endl;
    }
}
