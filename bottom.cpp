// 2025-07-04
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void dfs1(int u, const vector<vector<int>>& radj,
          vector<int>& vis, vector<int>& post) {
    if (vis[u]) return;    
    vis[u] = 1;
    for (const auto v : radj[u]) dfs1(v, radj, vis, post);
    post.push_back(u);
}
void dfs2(int u, const vector<vector<int>>& adj,
          vector<int>& sccid, int nscc) {
    if (sccid[u] >= 0) return;
    sccid[u] = nscc;
    for (const auto v : adj[u]) dfs2(v, adj, sccid, nscc);
}
void do_testcase(int V) {
    int E; cin >> E;
    vector<vector<int>> adj(V), radj(V);
    while (E--) {
        int u, v; cin >> u >> v; --u; --v;
        adj[u].push_back(v);
        radj[v].push_back(u);
    }
    vector<int> vis(V, 0);
    vector<int> post;
    for (int i = 0; i < V; i++) dfs1(i, radj, vis, post);
    reverse(post.begin(), post.end());
    int nscc = 0;
    vector<int> sccid(V, -1);
    for (const auto u : post) {
        if (sccid[u] < 0) {
            dfs2(u, adj, sccid, nscc++);
        }
    }
    vector<int> is_sink(nscc, 1);
    for (int u = 0; u < V; u++) {
        for (const auto v : adj[u]) {
            if (sccid[u] != sccid[v]) {
                is_sink[sccid[u]] = 0;
            }
        }
    }
    vector<int> result;
    for (int u = 0; u < V; u++) {
        if (is_sink[sccid[u]]) result.push_back(u);
    }
    sort(result.begin(), result.end());
    bool first = true;
    for (const auto u : result) {
        if (!first) cout << ' ';
        first = false;
        cout << u + 1;
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int V; cin >> V; if (V == 0) return 0;
        do_testcase(V);
    }
}
