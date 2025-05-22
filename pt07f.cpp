// 2025-05-21
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void dfs1(const vector<vector<int>>& adj, int p, int u,
          vector<int>& dp1, vector<int>& child1,
          vector<int>& dp2, vector<pair<int, int>>& child2) {
    int nchild = 0;
    int s = 0;
    int m1 = -1, m2 = -1;
    for (const auto v : adj[u]) {
        if (v == p) continue;
        dfs1(adj, u, v, dp1, child1, dp2, child2);
        ++nchild;
        s += dp2[v];
        const int diff = dp1[v] - dp2[v];
        if (m1 == -1) m1 = v;
        else if (m2 == -1 || diff < dp1[m2] - dp2[m2]) {
            m2 = v;
            if (dp1[m2] - dp2[m2] < dp1[m1] - dp2[m1]) swap(m1, m2);
        }
    }
    if (0 == nchild) {
        dp1[u] = dp2[u] = 1;
        child1[u] = -1;
        return;
    }
    if (1 == nchild) {
        dp1[u] = dp2[u] = dp1[m1];
        child1[u] = m1;
        return;
    }
    dp1[u] = s + dp1[m1] - dp2[m1];
    child1[u] = m1;
    if (dp1[u] > 1 + s) {
        dp1[u] = 1 + s;
        child1[u] = -1;
    }
    dp2[u] = s + dp1[m1] - dp2[m1] + dp1[m2] - dp2[m2] - 1;
    child2[u] = make_pair(m1, m2);
    if (dp2[u] > dp1[u]) dp2[u] = dp1[u];
}
void dfs2(const vector<vector<int>>& adj, int p, int u,
          vector<int>& dp1, vector<int>& child1,
          vector<int>& dp2, vector<pair<int, int>>& child2,
          vector<int>& vis, vector<vector<int>>& paths) {
    if (!vis[u]) {
        if (dp1[u] <= dp2[u]) {
            vector<int> path = {u};
            int u2 = u;
            while (child1[u2] != -1) {
                u2 = child1[u2];
                vis[u2] = 1;
                path.push_back(u2);
            }
            paths.push_back(move(path));
        } else {
            int u2 = child2[u].first;
            vector<int> path = {u};
            while (u2 != -1) {
                vis[u2] = 1;
                path.push_back(u2);
                u2 = child1[u2];
            }
            reverse(path.begin(), path.end());
            u2 = child2[u].second;
            while (u2 != -1) {
                vis[u2] = 1;
                path.push_back(u2);
                u2 = child1[u2];
            }
            paths.push_back(move(path));
        }
    }
    for (const auto v : adj[u]) {
        if (v == p) continue;
        dfs2(adj, u, v, dp1, child1, dp2, child2, vis, paths);
    }
}
void do_testcase() {
    int V; scanf("%d", &V);
    vector<vector<int>> adj(V);
    for (int i = 0; i < V - 1; i++) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> dp1(V);
    vector<int> child1(V);
    vector<int> dp2(V);
    vector<pair<int, int>> child2(V);
    dfs1(adj, -1, 0, dp1, child1, dp2, child2);
    vector<int> vis(V);
    vector<vector<int>> paths;
    dfs2(adj, -1, 0, dp1, child1, dp2, child2, vis, paths);
    printf("%d\n", (int)paths.size());
    for (const auto& path : paths) {
        printf("%d", path[0] + 1);
        for (int i = 1; i < path.size(); i++) {
            printf(" %d", path[i] + 1);
        }
        putchar('\n');
    }
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
