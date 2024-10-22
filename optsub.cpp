// 2024-10-22
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
void dfs(int parent, int u, const vector<vector<int>>& adj,
         const vector<int>& C, vector<int>& dp0, vector<int>& dp1) {
    dp0[u] = dp1[u] = C[u];
    for (const auto v : adj[u]) {
        if (v == parent) continue;
        dfs(u, v, adj, C, dp0, dp1);
        dp0[u] = max(dp0[u], dp0[v]);
        dp1[u] += max(dp1[v], 0);
        dp0[u] = max(dp0[u], dp1[u]);
    }
}
void do_testcase() {
    int V; cin >> V;
    vector<int> X(V), Y(V), C(V);
    unordered_map<long long, int> M;
    for (int i = 0; i < V; i++) {
        cin >> X[i] >> Y[i] >> C[i];
        M[((long long)X[i] << 32) + Y[i]] = i;
    }
    vector<vector<int>> adj(V);
    for (int i = 0; i < V; i++) {
        auto it = M.find(((long long)X[i] << 32) + Y[i] + 1);
        if (it != M.end()) {
            adj[i].push_back(it->second);
            adj[it->second].push_back(i);
        }
        it = M.find(((long long)(X[i] + 1) << 32) + Y[i]);
        if (it != M.end()) {
            adj[i].push_back(it->second);
            adj[it->second].push_back(i);
        }
    }
    vector<int> dp0(V);
    vector<int> dp1(V);
    dfs(-1, 0, adj, C, dp0, dp1);
    cout << dp0[0] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
