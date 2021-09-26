// 2021-09-26
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
long long dijkstra(const vector<vector<pair<int, int>>>& adj) {
    const int V = adj.size();
    vector<int> dist(V, 1e9 + 1);
    priority_queue<pair<int, int>> Q;
    Q.emplace(0, 0);
    while (!Q.empty()) {
        auto p = Q.top(); Q.pop();
        if (dist[p.second] < -p.first) continue;
        dist[p.second] = -p.first;
        for (const auto& n : adj[p.second]) {
            Q.emplace(p.first - n.second, n.first);
        }
    }
    return accumulate(dist.begin(), dist.end(), 0LL);
}
void do_testcase() {
    int V, E; cin >> V >> E;
    vector<vector<pair<int, int>>> adj(V);
    vector<vector<pair<int, int>>> radj(V);
    while (E--) {
        int u, v, w; cin >> u >> v >> w;
        u--; v--;
        adj[u].emplace_back(v, w);
        radj[v].emplace_back(u, w);
    }
    cout << dijkstra(adj) + dijkstra(radj) << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    while (N--) {
        do_testcase();
    }
}
