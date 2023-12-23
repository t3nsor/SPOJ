// 2023-12-23
// Max flow algorithm copied from fastflow.cpp. This algorithm is a bit slower
// than the Dinic's implementation from the Stanford ACM team notebook (see the
// history of this file if interested).
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
int main() {
    int N, M, P; scanf("%d %d %d", &N, &M, &P);
    vector<vector<st>> adj(N + M + 2);
    for (int i = 1; i <= N; i++) {
        adj[0].push_back(st{i, 1, (int)adj[i].size()});
        adj[i].push_back(st{0, 0, (int)adj[0].size() - 1});
    }
    for (int i = 1; i <= M; i++) {
        adj[N + i].push_back(st{N + M + 1, 1, (int)adj[N + M + 1].size()});
        adj[N + M + 1].push_back(st{N + i, 0, (int)adj[N + i].size() - 1});
    }
    for (int i = 0; i < P; i++) {
        int A, B; scanf("%d %d", &A, &B);
        adj[A].push_back(st{N + B, 1, (int)adj[N + B].size()});
        adj[N + B].push_back(st{A, 0, (int)adj[A].size() - 1});
    }
    printf("%lld\n", get_max_flow(move(adj), 0, N + M + 1));
    return 0;
}
