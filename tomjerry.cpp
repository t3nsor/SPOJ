// 2025-05-09
// The only hard thing about the problem is understanding the problem statement.
// Jerry moves twice in each time step, while Tom moves once (after both of
// Jerry's moves). If at the end of any of those three moves Jerry is at the
// same spot as Tom, the entire time step is considered to be used up.
//
// Because Jerry moves twice while Tom moves once, it is guaranteed that Jerry
// will catch Tom within ceil(V/3) time steps. This allows a straighforward
// recursive simulation (with memoization) to pass. If Jerry moved only once in
// each time step then the problem would be much harder.
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
void bfs(const vector<vector<int>>& adj,
         vector<int>& dist,
         int u) {
    queue<pair<int, int>> Q;
    Q.emplace(u, 0);
    while (!Q.empty()) {
        const auto p = Q.front();
        Q.pop();
        if (dist[p.first] <= p.second) continue;
        dist[p.first] = p.second;
        for (const auto v : adj[p.first]) {
            Q.emplace(v, p.second + 1);
        }
    }
}
double rec(vector<vector<double>>& memo,
           const vector<vector<int>>& adj,
           const vector<vector<int>>& step,
           int P, int Q) {
    if (memo[P][Q] < 0) memo[P][Q] = [&] () -> double {
        if (P == Q) return 0;
        if (step[P][Q] == Q || step[step[P][Q]][Q] == Q) return 1;
        const int P2 = step[step[P][Q]][Q];
        double res = rec(memo, adj, step, P2, Q);
        for (const auto v : adj[Q]) {
            res += rec(memo, adj, step, P2, v);
        }
        return 1 + res / (1 + adj[Q].size());
    }();
    return memo[P][Q];
}
void do_testcase() {
    int V, E; cin >> V >> E;
    int P, Q; cin >> P >> Q; --P; --Q;
    vector<vector<int>> adj(V);
    for (int i = 0; i < E; i++) {
        int u, v; cin >> u >> v; --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 0; i < V; i++) {
        sort(adj[i].begin(), adj[i].end());
    }
    vector<vector<int>> dist(V, vector<int>(V, 1e6));
    for (int i = 0; i < V; i++) {
        bfs(adj, dist[i], i);
    }
    vector<vector<int>> step(V, vector<int>(V));
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            int bdist = 1e6;
            step[i][j] = -1;
            if (j == i) continue;
            for (auto v : adj[i]) {
                if (dist[v][j] < bdist) {
                    bdist = dist[v][j];
                    step[i][j] = v;
                }
            }
        }
    }
    vector<vector<double>> memo(V, vector<double>(V, -1));
    printf("%.3f\n", rec(memo, adj, step, P, Q));
}
int main() {
    int T; cin >> T; while (T--) do_testcase();
}
