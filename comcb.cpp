// 2023-12-25
#include <iostream>
#include <string>
#include <string.h>
using namespace std;
const int dx[8]={-2,-2,-1,-1, 1, 1, 2, 2};
const int dy[8]={-1, 1,-2, 2,-2, 2,-1, 1};
template <class Matrix>
bool rec(int idx, int x, int y, int X, int Y, Matrix& vis) {
    if (idx > X * Y) return true;
    if (vis[x][y]) return false;
    vis[x][y] = idx;
    for (int k = 0; k < 8; k++) {
        const int x2 = x + dx[k];
        const int y2 = y + dy[k];
        if (x2 >= 0 && y2 >= 0 && x2 < X && y2 < Y) {
            if (rec(idx + 1, x2, y2, X, Y, vis)) {
                return true;
            }
        }
    }
    vis[x][y] = 0;
    return false;
}
void do_testcase() {
    // The problem description says X is the number of rows, which is silly. So
    // we relabel X as Y and vice versa.
    int X, Y; scanf("%d %d", &Y, &X);
    if (X == 1 && Y == 1) {
        cout << "A1\n";
        return;
    }
    int vis[26][26];
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (rec(1, i, j, X, Y, vis)) {
                string result(2*X*Y, 0);
                for (int k = 0; k < X; k++) {
                    for (int m = 0; m < Y; m++) {
                        result[2 * vis[k][m] - 2] = k + 'A';
                        result[2 * vis[k][m] - 1] = m + '1';
                    }
                }
                cout << result << '\n';
                return;
            }
        }
    }
    puts("-1");
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
