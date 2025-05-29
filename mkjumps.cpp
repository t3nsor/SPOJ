// 2025-05-29
#include <iostream>
using namespace std;
const int dr[8] = {1, 1, -1, -1, 2, 2, -2, -2};
const int dc[8] = {2, -2, 2, -2, 1, -1, 1, -1};
void rec(int& best, char (*grid)[10], int r, int c, int cur, int remaining) {
    if (cur > best) best = cur;
    if (cur + remaining < best) return;
    for (int i = 0; i < 8; i++) {
        const int r2 = r + dr[i];
        const int c2 = c + dc[i];
        if (r2 < 0 || c2 < 0 || r2 >= 10 || c2 >= 10 || !grid[r2][c2]) continue;
        grid[r2][c2] = 0;
        rec(best, grid, r2, c2, cur + 1, remaining - 1);
        grid[r2][c2] = 1;
    }
}
void do_testcase(int cs, int R) {
    char grid[10][10];
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            grid[r][c] = 0;
        }
    }
    int size = 0;
    int r0 = 0, c0;
    for (int i = 0; i < R; i++) {
        int ofs, len; cin >> ofs >> len;
        if (i == 0) c0 = ofs;
        size += len;
        for (int j = ofs; j < ofs + len; j++) grid[i][j] = 1;
    }
    int best = 0;
    grid[r0][c0] = 0;
    rec(best, grid, r0, c0, 1, size - 1);
    cout << "Case " << cs << ", " << size - best
         << (size - best == 1 ? " square" : " squares")
         << " can not be reached.\n";
}
int main() {
    for (int testcase = 1;; testcase++) {
        int R; cin >> R;
        if (R == 0) return 0;
        do_testcase(testcase, R);
    }
}
