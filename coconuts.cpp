// 2025-06-01
// Max flow implementation copied from fastflow.cpp. This is a min s-t cut
// problem where we have an edge from the source to a node (representing a
// guard) if the guard's belief is 1 and otherwise an edge from the node to the
// sink. We also have a node from one guard to another (and back) if they are
// friends. Cutting an edge from s or to t corresponds to the guard lying about
// their belief while cutting the remaining edges is the cost of isolating the
// guards that say they believe from the ones who say they don't.
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
void do_testcase(int n, int m) {
    vector<vector<st>> adj(n + 2);
    for (int i = 0; i < n; i++) {
        int x; scanf("%d", &x);
        if (x) add_edge(adj, 0, i + 1, 1);
        else add_edge(adj, i + 1, n + 1, 1);
    }
    for (int i = 0; i < m; i++) {
        int u, v; scanf("%d %d", &u, &v);
        add_edge(adj, u, v, 1);
        add_edge(adj, v, u, 1);
    }
    printf("%lld\n", get_max_flow(move(adj), 0, n + 1));
}
int main() {
    for (;;) {
        int n, m; scanf("%d %d", &n, &m);
        if (n == 0) break;
        do_testcase(n, m);
    }
}
