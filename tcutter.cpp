// 2024-03-09
// Assume the sheet of tin is initially infinite. After coordinate compression,
// we get a grid of X by Y, where X is 2 plus the number of distinct
// x-coordinates in the input, and Y is 2 pus the number of distinct
// y-coordinates, and each square of the grid has a cut on between 0 and 4 of
// its sides (except the squares on the border, which never have a cut on their
// exterior sides). By flood filling from a square that is known to be outside
// all holes (e.g. the upper left square) and not crossing any cuts, we obtain
// the set of squares that are not part of any hole. The connected components of
// the remaining squares in the grid are the holes.
//
// There's probably an O(N log N) algorithm but I'm too lazy to think about it.
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
void dfs1(vector<vector<int>>& grid,
          const vector<vector<int>>& canleft,
          const vector<vector<int>>& canup,
          int x, int y) {
    grid[x][y] = 1;
    const int X = grid.size();
    const int Y = grid[0].size();
    if (x > 0 && canleft[x][y] && grid[x - 1][y] == 0) {
        dfs1(grid, canleft, canup, x - 1, y);
    }
    if (y > 0 && canup[x][y] && grid[x][y - 1] == 0) {
        dfs1(grid, canleft, canup, x, y - 1);
    }
    if (x + 1 < X && canleft[x + 1][y] && grid[x + 1][y] == 0) {
        dfs1(grid, canleft, canup, x + 1, y);
    }
    if (y + 1 < Y && canup[x][y + 1] && grid[x][y + 1] == 0) {
        dfs1(grid, canleft, canup, x, y + 1);
    }
}
void dfs2(vector<vector<int>>& grid, int x, int y) {
    grid[x][y] = 1;
    const int X = grid.size();
    const int Y = grid[0].size();
    if (x > 0 && grid[x - 1][y] == 0) {
        dfs2(grid, x - 1, y);
    }
    if (y > 0 && grid[x][y - 1] == 0) {
        dfs2(grid, x, y - 1);
    }
    if (x + 1 < X && grid[x + 1][y] == 0) {
        dfs2(grid, x + 1, y);
    }
    if (y + 1 < Y && grid[x][y + 1] == 0) {
        dfs2(grid, x, y + 1);
    }
}
void do_testcase(int N) {
    vector<int> X1, Y1, X2, Y2;
    vector<int> allx, ally;
    for (int i = 0; i < N; i++) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        X1.push_back(x1); Y1.push_back(y1);
        X2.push_back(x2); Y2.push_back(y2);
        allx.push_back(x1); allx.push_back(x2);
        ally.push_back(y1); ally.push_back(y2);
    }
    sort(allx.begin(), allx.end());
    allx.erase(unique(allx.begin(), allx.end()), allx.end());
    sort(ally.begin(), ally.end());
    ally.erase(unique(ally.begin(), ally.end()), ally.end());
    const int X = allx.size() + 2;
    const int Y = ally.size() + 2;
    vector<vector<int>> grid(X, vector<int>(Y, 0));
    vector<vector<int>> canleft(X, vector<int>(Y, 1));
    vector<vector<int>> canup(X, vector<int>(Y, 1));
    for (int i = 0; i < N; i++) {
        X1[i] = lower_bound(allx.begin(), allx.end(), X1[i]) - allx.begin() + 1;
        X2[i] = lower_bound(allx.begin(), allx.end(), X2[i]) - allx.begin() + 1;
        Y1[i] = lower_bound(ally.begin(), ally.end(), Y1[i]) - ally.begin() + 1;
        Y2[i] = lower_bound(ally.begin(), ally.end(), Y2[i]) - ally.begin() + 1;
        if (X1[i] == X2[i]) {
            // vertical line
            for (int j = Y1[i]; j < Y2[i]; j++) canleft[X1[i]][j] = 0;
        } else {
            // horizontal line
            for (int j = X1[i]; j < X2[i]; j++) canup[j][Y1[i]] = 0;
        }
    }
    dfs1(grid, canleft, canup, 0, 0);
    int holes = 0;
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (grid[i][j] == 0) {
                // it is not the outside, so it's part of a hole
                ++holes;
                dfs2(grid, i, j);
            }
        }
    }
    cout << holes << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int N;
    for (;;) {
        cin >> N;
        if (N == 0) return 0;
        do_testcase(N);
    }
}
