// 2023-12-23
// This max flow implementation was copied from fastflow.cpp.
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <vector>
using namespace std;
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
void do_testcase() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<int> customers, U, V;
    for (int i = 0; i < k; i++) {
        int h; scanf("%d", &h); customers.push_back(h - 1);
    }
    for (int i = 0; i < m; i++) {
        int u, v; scanf("%d %d", &u, &v);
        U.push_back(u - 1); V.push_back(v - 1);
    }
    int lo = 0, hi = k;
    while (hi > lo) {
        const int mid = (lo + hi) / 2;
        vector<vector<st>> adj(n + 1);
        for (const int h : customers) {
            adj[h].push_back(st{n, 1, (int)adj[n].size()});
            adj[n].push_back(st{h, 0, (int)adj[h].size() - 1});
        }
        for (int i = 0; i < m; i++) {
            const int u = U[i];
            const int v = V[i];
            adj[u].push_back(st{v, mid, (int)adj[v].size()});
            adj[v].push_back(st{u, mid, (int)adj[u].size() - 1});
        }
        if (get_max_flow(move(adj), 0, n) >= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    printf("%d\n", lo);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
