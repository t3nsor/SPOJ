// 2024-01-05
// Menger's theorem: Let G be a graph with V vertices. If 0 <= k < V, then the
// following are equivalent:
// * G remains connected when any k vertices are removed
// * For each pair of different vertices x, y in G, there are at least k
//   internally vertex-disjoint paths between x and y (i.e., the paths do not
//   have any vertices in common other than x and y themselves).
// Because the number of vertices is small, we can test the second condition
// directly by doing a max flow for each such x, y pair. Max flow code copied
// from `fastflow.cpp`.
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
int get_max_flow(vector<vector<st>>&& adj, int s, int t) {
    const int V = adj.size();
    vector<int> excess(V, 0);
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
                const int x = min(excess[u], edge.cap);
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
int do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    vector<vector<int>> adj(V);
    for (int i = 0; i < E; i++) {
        // scanf with the format string "(%d,%d)" doesn't seem to skip initial
        // whitespace, so we have to skip it manually.
        char c;
        do {
            c = getchar();
        } while (c != '(');
        int u, v; scanf("%d,%d)", &u, &v);
        adj[u].push_back(v);
    }
    if (E == V * (V - 1) / 2) {
        // The complete graph is a special case.
        return V;
    }
    int minflow = 100;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            // construct the flow graph
            vector<vector<st>> fadj(2 * V);
            for (int k = 0; k < V; k++) {
                for (const int v : adj[k]) {
                    // 2x is the in vertex, 2x + 1 is the out vertex
                    fadj[2*k + 1].push_back(st{2*v,
                                               1,
                                               (int)fadj[2*v].size()});
                    fadj[2*v].push_back(st{2*k + 1,
                                           0,
                                           (int)fadj[2*k + 1].size() - 1});
                    fadj[2*v + 1].push_back(st{2*k,
                                               1,
                                               (int)fadj[2*k].size()});
                    fadj[2*k].push_back(st{2*v + 1,
                                           0,
                                           (int)fadj[2*v + 1].size() - 1});
                }
                fadj[2*k].push_back(st{2*k + 1, 1, (int)fadj[2*k + 1].size()});
                fadj[2*k + 1].push_back(st{2*k, 0, (int)fadj[2*k].size() - 1});
            }
            minflow = min(minflow, get_max_flow(move(fadj), 2*i + 1, 2*j));
        }
    }
    return minflow;
}
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        printf("%d\n", do_testcase());
    }
}
