// 2024-02-13
// Flow implementation taken from fastflow.cpp
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <utility>
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
    int p, t, s; scanf("%d %d %d", &p, &t, &s);
    long long c; scanf("%lld", &c);
    vector<vector<st>> adj(p + t + 2);
    vector<pair<int, int>> ppos, tpos;
    for (int i = 0; i < p; i++) {
        int x, y; scanf("%d %d", &x, &y);
        ppos.emplace_back(x, y);
        adj[0].push_back(st{1 + i, 1, (int)adj[1 + i].size()});
        adj[1 + i].push_back(st{0, 0, (int)adj[0].size() - 1});
    }
    for (int i = 0; i < t; i++) {
        int x, y; scanf("%d %d", &x, &y);
        tpos.emplace_back(x, y);
        adj[1 + p + i].push_back(st{1 + p + t, 1, (int)adj[1 + p + t].size()});
        adj[1 + p + t].push_back(st{1 + p + i, 0,
                                    (int)adj[1 + p + i].size() - 1});
    }
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < t; j++) {
            const auto dist = 200ll * (abs(ppos[i].first - tpos[j].first) +
                                       abs(ppos[i].second - tpos[j].second));
            if (c * s >= dist) {
                adj[1 + i].push_back(st{1 + p + j, 1,
                                        (int)adj[1 + p + j].size()});
                adj[1 + p + j].push_back(st{1 + i, 0,
                                         (int)adj[1 + i].size() - 1});
            }
        }
    }
    printf("%lld\n", get_max_flow(move(adj), 0, 1 + p + t));
}
int main() {
    int k; scanf("%d", &k);
    while (k--) do_testcase();
}
