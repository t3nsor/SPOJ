// 2025-06-06
#include <iostream>
#include <vector>
using namespace std;
constexpr int dr[8] = {1, 1, -1, -1, 2, 2, -2, -2};
constexpr int dc[8] = {2, -2, 2, -2, 1, -1, 1, -1};
int do_testcase(int R, int C) {
    vector<vector<int>> grid(R, vector<int>(C, 0));
    vector<vector<int>> safe(R, vector<int>(C, 1));
    int nq; cin >> nq;
    while (nq--) {
        int r, c; cin >> r >> c; --r; --c;
        grid[r][c] = 1;
        safe[r][c] = 0;
    }
    int nk; cin >> nk;
    while (nk--) {
        int r, c; cin >> r >> c; --r; --c;
        grid[r][c] = -1;
        safe[r][c] = 0;
        for (int i = 0; i < 8; i++) {
            const int r2 = r + dr[i];
            const int c2 = c + dc[i];
            if (r2 >= 0 && r2 < R && c2 >= 0 && c2 < C) {
                safe[r2][c2] = 0;
            }
        }
    }
    int np; cin >> np;
    while (np--) {
        int r, c; cin >> r >> c; --r; --c;
        grid[r][c] = -1;
        safe[r][c] = 0;
    }
    for (int i = 0; i < R; i++) {
        // scan row forward
        bool attack = false;
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 1) attack = true;
            if (grid[i][j] == -1) attack = false;
            if (attack) safe[i][j] = 0;
        }
        // backward
        attack = false;
        for (int j = C - 1; j >= 0; j--) {
            if (grid[i][j] == 1) attack = true;
            if (grid[i][j] == -1) attack = false;
            if (attack) safe[i][j] = 0;
        }
    }
    for (int j = 0; j < C; j++) {
        // scan column forward
        bool attack = false;
        for (int i = 0; i < R; i++) {
            if (grid[i][j] == 1) attack = true;
            if (grid[i][j] == -1) attack = false;
            if (attack) safe[i][j] = 0;
        }
        // backward
        attack = false;
        for (int i = R - 1; i >= 0; i--) {
            if (grid[i][j] == 1) attack = true;
            if (grid[i][j] == -1) attack = false;
            if (attack) safe[i][j] = 0;
        }
    }
    // positive slope diagonals
    for (int k = -R + 1; k <= C - 1; k++) {
        bool attack = false;
        int r = max(0, -k), c = max(0, k);
        while (r < R && c < C) {
            if (grid[r][c] == 1) attack = true;
            if (grid[r][c] == -1) attack = false;
            if (attack) safe[r][c] = 0;
            ++r; ++c;
        }
        attack = false;
        --r; --c;
        while (r >= 0 && c >= 0) {
            if (grid[r][c] == 1) attack = true;
            if (grid[r][c] == -1) attack = false;
            if (attack) safe[r][c] = 0;
            --r; --c;
        }
    }
    // negative slope diagonals
    for (int k = 0; k <= R + C - 2; k++) {
        bool attack = false;
        int r = max(0, min(R - 1, k - (C - 1)));
        int c = k - r;
        while (r < R && c >= 0) {
            if (grid[r][c] == 1) attack = true;
            if (grid[r][c] == -1) attack = false;
            if (attack) safe[r][c] = 0;
            ++r; --c;
        }
        attack = false;
        --r; ++c;
        while (r >= 0 && c < C) {
            if (grid[r][c] == 1) attack = true;
            if (grid[r][c] == -1) attack = false;
            if (attack) safe[r][c] = 0;
            --r; ++c;
        }
    }
    int result = 0;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            result += safe[i][j];
        }
    }
    return result;
}
int main() {
    ios::sync_with_stdio(false);
    for (int cs = 1;; ++cs) {
        int R, C; cin >> R >> C;
        if (R == 0) break;
        cout << "Board " << cs << " has " << do_testcase(R, C)
             << " safe squares.\n";
    }
}
