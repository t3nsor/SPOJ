// 2026-09-02
// It turns out that a much simpler approach than puzzle2-1.cpp is possible if
// you don't try to find the lexicographically minimal representative.  It was
// actually my student who found this.  Consider each column as a bit vector.
// The two allowable operations are to swap columns or to flip a particular bit
// position in every column simultaneously.  In other words, if we do all the
// flips first, then all sequences of operations boil down to the following:
// fix some mask M with which to XOR all columns, then permute the columns.
// Furthermore, the only values of M that could possibly lead to a solution are
// those that, when applied to the first column in one of the arrangements,
// results in one of the columns in the other arrangement.  In other words there
// are not more than C possible masks to try, where C is the number of columns.
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
bool do_testcase() {
    int R, C; cin >> R >> C;
    string col[3][100];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < R; j++) {
            for (int k = 0; k < C; k++) {
                string w; cin >> w;
                col[i][k].push_back(int(w == "RED"));
            }
        }
    }
    sort(col[0], col[0] + C);
    for (int i = 0; i < C; i++) {
        for (int j = 0; j < C; j++) col[2][j] = col[1][j];
        // match col[1][0] to col[0][i]
        for (int j = 0; j < R; j++) {
            if (col[1][0][j] != col[0][i][j]) {
                for (int k = 0; k < C; k++) col[2][k][j] ^= 1;
            }
        }
        sort(col[2], col[2] + C);
        if (equal(col[0], col[0] + C, col[2])) return true;
    }
    return false;
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T; while (T--) cout << (do_testcase() ? "YES" : "NO") << '\n';
}
