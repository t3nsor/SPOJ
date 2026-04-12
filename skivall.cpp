// 2026-04-12
// At first I thought "oh, this is just mincost maxflow" but then I realized
// that while mincost maxflow works if you know the lowest point in advance,
// you're not gonna get a fast enough algorithm if you run it once for every
// possible lowest point, and I can't find a way to make it work if you run it
// only a single time because I don't see how you can enforce the constraint
// that all the flow starts by going through one single node (the lowest point)
// before diverging into two increasing-height paths, rather than going through
// two *disjoint* increasing paths.  Anyway, it turns out the actual solution is
// a simple DP where the state is the two endpoints of a valid ski-valley; to
// avoid duplicates, we can only consider transitions from state s1 to s2 if the
// endpoint that changed between s1 and s2 is the higher endpoint in s2.
#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
constexpr int dr[4] = {1, 0, -1, 0};
constexpr int dc[4] = {0, 1, 0, -1};
int main() {
    int R, C; cin >> R >> C;
    vector<vector<int>> grid(R, vector<int>(C));
    vector<tuple<int, int, int>> v;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            cin >> grid[i][j];
            v.emplace_back(grid[i][j], i, j);
        }
    }
    sort(v.begin(), v.end());
    vector<vector<int>> idx(R, vector<int>(C));
    for (int i = 0; i < R*C; i++) {
        idx[get<1>(v[i])][get<2>(v[i])] = i;
    }
    vector<vector<int>> adj(v.size());
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            for (int k = 0; k < 4; k++) {
                const int i2 = i + dr[k];
                const int j2 = j + dc[k];
                if (i2 < 0 || i2 >= R || j2 < 0 || j2 >= C) continue;
                if (grid[i2][j2] < grid[i][j]) {
                    adj[idx[i][j]].push_back(idx[i2][j2]);
                }
            }
        }
    }
    vector<vector<int>> dp(v.size(), vector<int>(v.size(), -1e6));
    vector<vector<int>> pred(v.size(), vector<int>(v.size()));
    int besti = 0, bestj = 0, best = 1;
    for (int i = 0; i < v.size(); i++) {
        dp[i][i] = 1;
        for (int j = i + 1; j < v.size(); j++) {
            for (const int k : adj[j]) {
                int i2 = i;
                int j2 = k;
                if (i2 > j2) swap(i2, j2);
                if (1 + dp[i2][j2] > dp[i][j]) {
                    dp[i][j] = 1 + dp[i2][j2];
                    pred[i][j] = k;
                    if (dp[i][j] > best) {
                        best = dp[i][j];
                        besti = i;
                        bestj = j;
                    }
                }
            }
        }
    }
    cout << best << '\n';
    vector<pair<int, int>> p1;
    vector<pair<int, int>> p2;
    p1.emplace_back(get<1>(v[besti]), get<2>(v[besti]));
    p2.emplace_back(get<1>(v[bestj]), get<2>(v[bestj]));
    while (besti != bestj) {
        if (besti > bestj) {
            swap(besti, bestj);
            swap(p1, p2);
        }
        bestj = pred[besti][bestj];
        p2.emplace_back(get<1>(v[bestj]), get<2>(v[bestj]));
    }
    for (int i = 0; i < p1.size(); i++) {
        cout << p1[i].first + 1 << ' ' << p1[i].second + 1 << '\n';
    }
    for (int i = p2.size() - 2; i >= 0; i--) {
        cout << p2[i].first + 1 << ' ' << p2[i].second + 1 << '\n';
    }
}
