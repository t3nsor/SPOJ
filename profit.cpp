// 2023-11-02
// This is an instance of the closure problem:
// https://en.wikipedia.org/wiki/Closure_problem
// It's referred to as "Transportation network design" in the above article.
// If we construct a graph where each edge A_i <-> B_i is represented by a
// node with positive weight C_i, with an outward edge to each of the two nodes
// A_i, B_i (which have negative weight, -P_{A_i} and -P_{B_i}), then the max
// profit equals the maximum weight closure in this graph, which can be reduced
// to finding a minimum cut in a modified graph.
//
// Max flow implementation copied from fastflow.cpp. In this case the FIFO
// push-relabel performs much worse than Dinic's algorithm, but is still
// adequate to get AC. (You can see the version with Dinic's in the history for
// this file, but keep in mind that I'm not the author of that implementation.)
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int INF = 2000000000;
struct st {
    int v;
    int cap;
    int back;
};
long long get_max_flow(vector<vector<st>>&& adj, int s, int t) {
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
void add_edge(vector<vector<st>>& adj, int u, int v, int c) {
    adj[u].push_back(st{v, c, (int)adj[v].size()});
    adj[v].push_back(st{u, 0, (int)adj[u].size() - 1});
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int n, m; scanf("%d %d", &n, &m);
        vector<vector<st>> adj(n + m + 2);
        for (int i = 1; i <= n; i++) {
            int cost; scanf("%d", &cost);
            add_edge(adj, i, n + m + 1, cost);
        }
        int total = 0;
        for (int i = 1; i <= m; i++) {
            int a, b, profit; scanf("%d %d %d", &a, &b, &profit);
            total += profit;
            add_edge(adj, 0, n + i, profit);
            add_edge(adj, n + i, a, INF);
            add_edge(adj, n + i, b, INF);
        }
        printf("%lld\n", total - get_max_flow(move(adj), 0, n + m + 1));
    }
}
