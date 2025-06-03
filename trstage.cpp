// 2025-06-03
#include <iostream>
#include <functional>
#include <queue>
#include <vector>
using namespace std;
void do_testcase(int t, int v, int e, int src, int dest) {
    const int V = v << t;
    vector<int> horses(t);
    for (int i = 0; i < t; i++) {
        cin >> horses[i];
    }
    vector<vector<pair<double, int>>> adj(V);
    while (e--) {
        int u, v; cin >> u >> v; --u; --v;
        double d; cin >> d;
        for (int mask = 1; mask < (1 << t); ++mask) {
            for (int i = 0; i < t; i++) {
                if (!(mask & (1 << i))) continue;
                adj[(u << t) + mask].emplace_back(
                  d / horses[i],
                  (v << t) + (mask - (1 << i))
                );
                adj[(v << t) + mask].emplace_back(
                  d / horses[i],
                  (u << t) + (mask - (1 << i))
                );
            }
        }
    }
    priority_queue<pair<double, int>,
                   vector<pair<double, int>>,
                   greater<pair<double, int>>> Q;
    vector<double> dist(V, 1e6);
    Q.emplace(0, (src << t) + (1 << t) - 1);
    while (!Q.empty()) {
        const auto p = Q.top();
        Q.pop();
        if (dest == (p.second >> t)) {
            cout << p.first << '\n';
            return;
        }
        if (dist[p.second] <= p.first) continue;
        dist[p.second] = p.first;
        for (const auto p2 : adj[p.second]) {
            Q.emplace(p2.first + p.first, p2.second);
        }
    }
    cout << "Impossible\n";
}
int main() {
    for (;;) {
        int t, V, E, src, dest; cin >> t >> V >> E >> src >> dest;
        if (V == 0) break;
        do_testcase(t, V, E, src - 1, dest - 1);
    }
}
