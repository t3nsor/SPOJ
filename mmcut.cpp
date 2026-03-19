// 2026-03-18
#include <iostream>
#include <vector>
using namespace std;
bool dfs(const vector<vector<int>>& adj, vector<int>& state,
         int p, int u, int s) {
    if (state[u] >= 0) return state[u] == s;
    state[u] = s;
    for (const auto v : adj[u]) {
        if (v != p) {
            if (!dfs(adj, state, u, v, 1 - s)) return false;
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    int V, M; cin >> V >> M;
    vector<vector<int>> adj(V);
    vector<int> state(V, -1);
    vector<int> first;
    while (M--) {
        int u, v; cin >> u >> v;
        if (u == V - 1 || v == V - 1) {
            const int other = u + v - (V - 1);
            first.push_back(other);
        } else {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
    for (const auto u : first) {
        if (state[u] == 0 || !dfs(adj, state, -1, u, 1)) {
            cout << "2\n";
            return 0;
        }
    }
    for (int u = 0; u < V - 1; u++) {
        if (state[u] < 0) {
            if (!dfs(adj, state, -1, u, 1)) {
                cout << "2\n";
                return 0;
            }
        }
    }
    cout << "1\n";
}
