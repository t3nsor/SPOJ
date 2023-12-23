// 2023-12-23
// This is a naive implementation of the push-relabel method with FIFO vertex
// selection and gap heuristic. Surprisingly, it performs much better than
// relabel-to-front (seems I was lied to) and even the Dinic's implementation I
// got from the Stanford ACM team notebook. For the gap heuristic, it's possible
// to avoid the loop over all vertices by keeping vertices in a linked list and
// maintaining pointers to the last vertex of each height, but this actually
// makes the program slower.
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
    int N, M; scanf("%d %d", &N, &M);
    vector<vector<st>> adj(N);
    while (M--) {
        int A, B, C;
        scanf("%d %d %d", &A, &B, &C); --A; --B;
        if (A == B) continue;
        adj[A].push_back(st{B, C, (int)adj[B].size()});
        adj[B].push_back(st{A, C, (int)adj[A].size() - 1});
    }
    printf("%lld\n", get_max_flow(move(adj), 0, N - 1));
    return 0;
}
