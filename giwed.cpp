// 2025-05-31
// This is a bog-standard coordinate compression problem, so I don't know why it
// has so few accepted users.
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};
int dfs(vector<vector<int>>& grid, int x, int y) {
    if (grid[x][y] < 0) return 0;
    int result = grid[x][y];
    grid[x][y] = -1;
    for (int i = 0; i < 4; i++) {
        const int x2 = x + dx[i];
        const int y2 = y + dy[i];
        if (x2 < 0 || x2 >= grid.size() -1 ||
            y2 < 0 || y2 >= grid[0].size() - 1) continue;
        result += dfs(grid, x2, y2);
    }
    return result;
}
void do_testcase() {
    int M, N, K, L; cin >> M >> N >> K >> L;
    vector<int> allx = {0, M};
    vector<int> ally = {0, N};
    vector<int> x1(K), y1(K), x2(K), y2(K);
    for (int i = 0; i < K; i++) {
        cin >> x1[i] >> y1[i] >> x2[i] >> y2[i];
        allx.push_back(x1[i]);
        allx.push_back(x2[i]);
        ally.push_back(y1[i]);
        ally.push_back(y2[i]);
    }
    sort(allx.begin(), allx.end());
    allx.erase(unique(allx.begin(), allx.end()), allx.end());
    sort(ally.begin(), ally.end());
    ally.erase(unique(ally.begin(), ally.end()), ally.end());
    vector<vector<int>> grid(allx.size(), vector<int>(ally.size(), 0));
    for (int i = 0; i < K; i++) {
        const int X1 =
          lower_bound(allx.begin(), allx.end(), x1[i]) - allx.begin();
        const int X2 =
          lower_bound(allx.begin(), allx.end(), x2[i]) - allx.begin();
        const int Y1 =
          lower_bound(ally.begin(), ally.end(), y1[i]) - ally.begin();
        const int Y2 =
          lower_bound(ally.begin(), ally.end(), y2[i]) - ally.begin();
        grid[X1][Y1]++;
        grid[X1][Y2]--;
        grid[X2][Y1]--;
        grid[X2][Y2]++;
    }
    for (int i = 0; i < allx.size(); i++) {
        for (int j = 1; j < ally.size(); j++) {
            grid[i][j] += grid[i][j - 1];
        }
    }
    for (int i = 1; i < allx.size(); i++) {
        for (int j = 0; j < ally.size(); j++) {
            grid[i][j] += grid[i - 1][j];
        }
    }
    for (int i = 0; i < allx.size() - 1; i++) {
        for (int j = 0; j < ally.size() - 1; j++) {
            if (grid[i][j] == 0) {
                grid[i][j] = (allx[i + 1] - allx[i]) * (ally[j + 1] - ally[j]);
            } else {
                grid[i][j] = -1;
            }
        }
    }
    vector<int> areas;
    for (int i = 0; i < allx.size() - 1; i++) {
        for (int j = 0; j < ally.size() - 1; j++) {
            if (grid[i][j] > 0) {
                areas.push_back(dfs(grid, i, j));
            }
        }
    }
    sort(areas.begin(), areas.end());
    int result = 0;
    for (int i = areas.size() - 1;
         i >= max<int>(0, areas.size() - L);
         i--) {
        result += areas[i];
    }
    cout << result << '\n';
}
int main() {
    int T; cin >> T; while (T--) do_testcase();
}
