// 2025-06-04
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
constexpr int dr[4] = {0, 1, 0, -1};
constexpr int dc[4] = {1, 0, -1, 0};
vector<vector<int>> bfs(const vector<string>& grid, int r, int c) {
    const int R = grid.size();
    const int C = grid[0].size();
    vector<vector<int>> dist(R, vector<int>(C, 1000));
    queue<pair<int, int>> Q;
    dist[r][c] = 0;
    Q.emplace(r, c);
    while (!Q.empty()) {
        const auto pos = Q.front();
        Q.pop();
        for (int i = 0; i < 4; i++) {
            const int r2 = pos.first + dr[i];
            const int c2 = pos.second + dc[i];
            if (r2 < 0 || r2 >= R || c2 < 0 || c2 >= C || grid[r2][c2] == 'x')
                continue;
            if (dist[r2][c2] <= 1 + dist[pos.first][pos.second]) continue;
            dist[r2][c2] = 1 + dist[pos.first][pos.second];
            Q.emplace(r2, c2);
        }
    }
    return dist;
}
int do_testcase(int C, int R) {
    vector<string> grid(R);
    int rr, rc;
    vector<pair<int, int>> dirty;
    for (int i = 0; i < R; i++) {
        cin >> grid[i];
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 'o') {
                rr = i;
                rc = j;
            } else if (grid[i][j] == '*') dirty.emplace_back(i, j);
        }
    }
    if (dirty.empty()) return 0;
    const auto rdist = bfs(grid, rr, rc);
    for (const auto& dpos : dirty) {
        if (rdist[dpos.first][dpos.second] > 400) return -1;
    }
    vector<vector<vector<int>>> ddist;
    for (const auto& dpos : dirty) {
        ddist.push_back(bfs(grid, dpos.first, dpos.second));
    }
    vector<vector<int>> dp(1 << dirty.size(), vector<int>(dirty.size(), 1e6));
    for (int i = 0; i < dirty.size(); i++) {
        dp[1 << i][i] = rdist[dirty[i].first][dirty[i].second];
    }
    for (int i = 1; i < (1 << dirty.size()); i++) {
        for (int j = 0; j < dirty.size(); j++) {
            if (!(i & (1 << j))) continue;
            for (int k = 0; k < dirty.size(); k++) {
                if (i & (1 << k)) continue;
                dp[i + (1 << k)][k] =
                  min(dp[i + (1 << k)][k],
                      dp[i][j] + ddist[j][dirty[k].first][dirty[k].second]);
            }
        }
    }
    const auto& all = dp[(1 << dirty.size()) - 1];
    return *min_element(all.begin(), all.end());
}
int main() {
    for (;;) {
        int C, R; cin >> C >> R;
        if (C == 0) break;
        cout << do_testcase(C, R) << '\n';
    }
}
