// 2025-07-25
// To find the minimum even simple cycle we BFS from each vertex and from that
// vertex an even cycle is found when we find a vertex `u` with a neighbour `w`
// that has already been enqueued with a distance greater than `u` (for example,
// if `u` is at distance 2 from the source and `w` has been enqueued with
// distance 3, there is a cycle of length 6 that goes from the starting vertex
// to `u`, then to `w`, then back to the starting vertex). For a given starting
// vertex, the BFS is terminated when an even cycle is found, but that even
// cycle isn't necessarily simple. However, the shortest cycle found this way
// across all starting vertices will be simple. This basic idea was discovered
// by Itai and Rodeh (1978).
//
// This algorithm takes O(VE) time and we need some constant optimization to
// pass on SPOJ. Some optimizations are shown below and indicated as (1), (2)
// and (3); to get AC you need (1) plus at least (2) or (3). You can also
// optimize a bit more by flattening the adjacency lists (not shown).
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <vector>
using namespace std;
void do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    vector<vector<int>> adj(V);
    for (int i = 0; i < E; i++) {
        int a, b; scanf("%d %d", &a, &b); --a; --b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    int bestv = -1;
    int bestl = 1e6;
    vector<int> istree(V, false);
    for (int v = 0; v < V; v++) {
        if (istree[v]) continue;
        vector<int> level(V, -1);
        vector<int> parent(V, -1);
        queue<int> Q; Q.push(v); level[v] = 0;
        bool maybe_has_cycle = false;
        while (!Q.empty()) {
            const int u = Q.front(); Q.pop();
            for (const auto w : adj[u]) {
                if (w == parent[u]) continue;
                if (level[w] == -1) {
                    level[w] = level[u] + 1;
                    parent[w] = u;
                    if (2*level[w] > bestl) {
                        // optimization (3): continuing the BFS from this vertex
                        // can't improve over the current shortest even cycle
                        // (but we still need to mark this vertex as possibly
                        // having a cycle in its connected component, because
                        // there could be a short cycle in the component that
                        // doesn't contain `v`)
                        maybe_has_cycle = true;
                        goto end;
                    }
                    Q.push(w);
                } else if (parent[w] != u) {
                    // note: the above condition must be checked because the
                    // input has duplicate edges (alternatively, preprocess the
                    // graph to remove duplicate edges)
                    maybe_has_cycle = true;
                    if (level[w] == level[u] + 1) {
                        const int l = 2*level[w];
                        if (l < bestl) {
                            bestl = l;
                            bestv = v;
                        }
                        goto end;
                    }
                }
            }
        }
end:
        if (!maybe_has_cycle) {
            // optimization (1): if there are no cycles reachable from this
            // connected component, skip BFSing from all vertices in this
            // connected component
            for (int u = 0; u < V; u++) {
                if (level[u] >= 0) istree[u] = true;
            }
        }
        // optimization (2): the minimum even cycle can't be improved beyond 4
        if (bestl == 4) break;
    }
    if (bestv == -1) { puts("-1"); return; }
    // easiest way to reconstruct the optimal cycle is to just redo the BFS in
    // *precisely* the same way, starting from `bestv`
    vector<int> level(V, -1);
    vector<int> parent(V, -1);
    queue<int> Q; Q.push(bestv); level[bestv] = 0;
    for (;;) {
        int u = Q.front(); Q.pop();
        for (auto w : adj[u]) {
            if (w == parent[u]) continue;
            if (level[w] == -1) {
                level[w] = level[u] + 1;
                parent[w] = u;
                Q.push(w);
            } else if (parent[w] != u && level[w] == level[u] + 1) {
                printf("%d\n", 2*level[w]);
                vector<int> path;
                path.push_back(w);
                do {
                    w = parent[w];
                    path.push_back(w);
                } while (w != bestv);
                reverse(path.begin(), path.end());
                path.push_back(u);
                do {
                    u = parent[u];
                    path.push_back(u);
                } while (u != bestv);
                for (int i = 0; i < path.size(); i++) {
                    if (i > 0) putchar(' ');
                    printf("%d", path[i] + 1);
                }
                putchar('\n');
                return;
            }
        }
    }
}
int main() {
    int t; scanf("%d", &t); while (t--) do_testcase();
}
