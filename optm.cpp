// 2024-09-01
// First, notice that each bit position can be considered independently. So we
// have 31 subproblems where in each subproblem we have some nodes initially
// coloured 0 and some initially coloured 1. Consider the set S of nodes that
// will be coloured 1 (after all nodes are coloured) and the set T of nodes that
// will be coloured 0. Each edge between S and T has unit cost. This cost is
// equal to the number of edges that must be cut to disconnect S and T. The
// optimal partitioning thus corresponds to an optimal cut of the graph such
// that all nodes initially coloured 0 are unreachable from those initially
// coloured 1. This can be reformulated as an s-t min cut problem where we add
// a new vertex s that has an infinite cost edge to every node initially
// coloured 1 and a new vertex t that has an infinite cost edge incoming from
// every node initially coloured 0.
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
// max flow implementation taken from fastflow.cpp
struct st {
    int v;
    int cap;
    int back;
};
long long get_max_flow(vector<vector<st>>& adj, int s, int t) {
    const int V = adj.size();
    vector<long long> excess(V, 0);
    vector<int> active(V, 0);
    vector<int> height(V, 0);
    vector<int> hcnt(2*V + 1, 0);
    queue<int> Q;
    height[s] = V;
    hcnt[V] = 1;
    hcnt[0] = V - 1;
    for (int i = 0; i < V; i++) {
        if (i == s || i == t) continue;
        active[i] = 1;
        Q.push(i);
    }
    for (auto& edge : adj[s]) {
        excess[edge.v] += edge.cap;
        adj[edge.v][edge.back].cap = edge.cap;
        edge.cap = 0;
    }
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        active[u] = 0;
        int best = 1e9;
        for (auto& edge : adj[u]) {
            if (!edge.cap) continue;
            if (height[u] == height[edge.v] + 1) {
                const int x = min<long long>(excess[u], edge.cap);
                excess[u] -= x;
                excess[edge.v] += x;
                edge.cap -= x;
                adj[edge.v][edge.back].cap += x;
                if (edge.v != s && edge.v != t && !active[edge.v]) {
                    active[edge.v] = 1;
                    Q.push(edge.v);
                }
                if (excess[u] == 0) break;
            } else {
                best = min(best, height[edge.v]);
            }
        }
        if (excess[u] > 0) {
            const int oldh = height[u];
            height[u] = best + 1;
            hcnt[best + 1]++;
            if (0 == --hcnt[oldh] && oldh < V) {
                for (int i = 0; i < V; i++) {
                    if (i != s && i != t &&
                        height[i] > oldh && height[i] <= V) {
                        hcnt[height[i]]--;
                        height[i] = V + 1;
                    }
                }
            }
            if (!active[u]) {
                active[u] = 1;
                Q.push(u);
            }
        }
    }
    return excess[t];
}
void do_testcase() {
    int N, M; cin >> N >> M;
    vector<vector<int>> adj(N);
    for (int i = 0; i < M; i++) {
        int u, v; cin >> u >> v; u--; v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int K; cin >> K;
    vector<pair<int, unsigned>> given(K);
    for (int i = 0; i < K; i++) {
        cin >> given[i].first >> given[i].second; --given[i].first;
    }
    vector<unsigned> mark(N);
    for (int bit = 0; bit < 31; bit++) {
        vector<vector<st>> network(N + 2);
        for (int u = 0; u < N; u++) {
            for (int v : adj[u]) {
                if (u > v) continue;
                network[u + 1].push_back({v + 1, 1,
                                          (int)network[v + 1].size()});
                network[v + 1].push_back({u + 1, 1,
                                          (int)network[u + 1].size() - 1});
            }
        }
        for (const auto& p : given) {
            const int v = p.first;
            if ((p.second & (1U << bit)) != 0) {
                network[0].push_back({v + 1, 10000,
                                      (int)network[v + 1].size()});
                network[v + 1].push_back({0, 0, (int)network[0].size() - 1});
            } else {
                network[v + 1].push_back({N + 1, 10000,
                                          (int)network[N + 1].size()});
                network[N + 1].push_back({v + 1, 0,
                                          (int)network[v + 1].size() - 1});
            }
        }
        get_max_flow(network, 0, N + 1);
        // check which nodes are still reachable from the source; they are on
        // the source side of the cut and are thus optimally coloured 1
        vector<char> vis(N + 2, 0);
        queue<int> Q;
        Q.push(0);
        while (!Q.empty()) {
            const int v = Q.front();
            Q.pop();
            if (vis[v]) continue;
            vis[v] = 1;
            if (v >= 1 && v <= N) {
                mark[v - 1] |= (1U << bit);
            }
            for (const auto& st : network[v]) {
                if (st.cap > 0) Q.push(st.v);
            }
        }
    }
    for (int i = 0; i < N; i++) {
        cout << mark[i] << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T; while (T--) do_testcase();
}
