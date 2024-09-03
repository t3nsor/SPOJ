// 2024-09-02
// The problem is easy once you figure out what the problem statement means. A
// cell is considered to uniquely identify a grid if the grid has ink on that
// cell and no other grid has ink on that cell. A pair of cells is considered to
// uniquely identify a grid if the grid has ink on both of those cells and no
// other grid has ink on both of those cells. (This doesn't really make sense;
// since the machine can't see the ink, there's no reason why the polymer
// shouldn't be placed under a cell that is inked on another grid as long as
// that other grid doesn't also have polymer placed there. But it is what it
// is.)
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
void do_testcase(int n, int r, int c) {
    vector<vector<string>> grids(n, vector<string>(r));
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grids[j][i];
        }
    }
    for (int i = 0; i < grids.size(); i++) {
        vector<pair<int, int>> ink;
        for (int j = 0; j < r; j++) {
            for (int k = 0; k < c; k++) {
                if (grids[i][j][k] == 'o') ink.emplace_back(j, k);
            }
        }
        // try with one
        for (const auto& p : ink) {
            for (int j = 0; j < grids.size(); j++) {
                if (j == i) continue;
                if (grids[j][p.first][p.second] != '.') goto fail1;
            }
            grids[i][p.first][p.second] = '#';
            goto success;
fail1:
            ;
        }
        for (int i1 = 0; i1 < ink.size(); i1++) {
            for (int i2 = i1 + 1; i2 < ink.size(); i2++) {
                const auto p1 = ink[i1];
                const auto p2 = ink[i2];
                for (int j = 0; j < grids.size(); j++) {
                    if (j == i) continue;
                    if (grids[j][p1.first][p1.second] != '.' &&
                        grids[j][p2.first][p2.second] != '.') goto fail2;
                }
                grids[i][p1.first][p1.second] = '#';
                grids[i][p2.first][p2.second] = '#';
                goto success;
fail2:
                ;
            }
        }
        cout << "impossible\n";
        return;
success:
        ;
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < n; j++) {
            cout << grids[j][i];
            cout << (j == n - 1 ? '\n' : ' ');
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    int n, r, c;
    int tc = 1;
    for (;;) {
        cin >> n >> r >> c;
        if (n == 0) return 0;
        cout << "Test " << tc++ << '\n';
        do_testcase(n, r, c);
    }
}
