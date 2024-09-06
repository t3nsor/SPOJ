// 2024-09-05
// According to the comments, some lines in the input have more than 2n
// integers. To get AC, we must ignore the extraneous integers on such lines.
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int a[100][100];
bool vis[100][100];
constexpr int dr[4] = {1, 0, -1, 0};
constexpr int dc[4] = {0, 1, 0, -1};
int dfs(int n, int r, int c) {
    if (vis[r][c]) return 0;
    vis[r][c] = true;
    int result = 1;
    for (int i = 0; i < 4; i++) {
        const int r2 = r + dr[i];
        const int c2 = c + dc[i];
        if (r2 < 0 || c2 < 0 || r2 >= n || c2 >= n) continue;
        if (a[r2][c2] == a[r][c]) {
            result += dfs(n, r2, c2);
        }
    }
    return result;
}
int main() {
    string s;
    for (;;) {
        getline(cin, s);
        istringstream ss1(s);
        int n; ss1 >> n;
        if (n == 0) return 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = n;
                vis[i][j] = false;
            }
        }
        for (int i = 1; i <= n - 1; i++) {
            getline(cin, s);
            istringstream ss2(s);
            for (int j = 0; j < n; j++) {
                int r, c; ss2 >> r >> c; --r; --c;
                a[r][c] = i;
            }
        }
        bool fail = false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (vis[i][j]) continue;
                if (dfs(n, i, j) != n) {
                    fail = true;
                    goto end;
                }
            }
        }
end:
        puts(fail ? "wrong" : "good");
    }
}
