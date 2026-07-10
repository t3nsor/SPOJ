// 2026-07-10
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
using LL = long long;
// `dp[i][j]` is the number of subtrees with diameter at most `D` (note that `D`
// was called `K` in the problem statement), whose most shallow node is `i`, and
// whose height (counting from node `i`) is exactly `j`.
LL dp[60][60];
void dfs(const vector<vector<int>>& adj, int D, int p, int u) {
    for (int h = 0; h <= D; h++) {
        dp[u][h] = (h == 0);
    }
    vector<LL> vm(60, 0);  // number of nonempty combinations of subtrees rooted
                           // at children of `u`, where each such subtree has
                           // diameter at most `D`, the maximum height among
                           // them is the index, and the sum of the largest two
                           // heights is at most D - 2 (so that when the root is
                           // added, the diameter will not exceed D)
    for (const int v : adj[u]) {
        if (v == p) continue;
        dfs(adj, D, u, v);
        vector<LL> vm2 = vm;
        for (int h = 0; h <= D - 1; h++) {
            // `h` is the height of the tree rooted at `v`
            dp[u][h + 1] += dp[v][h];
            vm2[h] += dp[v][h];
            for (int h2 = 0; h2 + h + 2 <= D; h2++) {
                dp[u][max(h, h2) + 1] += vm[h2]*dp[v][h];
                vm2[max(h, h2)] += vm[h2]*dp[v][h];
            }
        }
        swap(vm, vm2);
    }

}
void do_testcase() {
    int V, D; cin >> V >> D;
    vector<vector<int>> adj(V);
    for (int i = 0; i < V - 1; i++) {
        int u, v; cin >> u >> v;
        // a rare problem where the vertex IDs are zero-based, so we don't need
        // to subtract 1!
        adj[u].push_back(v); adj[v].push_back(u);
    }
    dfs(adj, D, -1, 0);
    LL result = 0;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j <= D; j++) result += dp[i][j];
    }
    cout << result << '\n';
}
int main() {
    int T; cin >> T; while (T--) do_testcase();
}
