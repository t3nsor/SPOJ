// 2024-01-13
// This problem is a variant of "shortest simple cycle in an undirected graph
// containing a given node", which is discussed here:
// https://codeforces.com/blog/entry/44244?#comment-288324
// This version of the problem is actually easier, however, because in this case
// we don't care whether the cycle is simple, we just want its winding number to
// be nonzero---which also rules out trivial cases like taking one flight and
// then immediately going back along the same flight to the starting point. All
// we have to do is BFS from the starting vertex and record the number of
// flights and net arc travelled to get to each vertex (i.e. total clockwise
// travel minus total counterclockwise travel) and then for each non-tree edge
// (u, v) in the graph, check whether it satisfies the nonzero winding number
// condition.
#include <limits.h>
#include <queue>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
int MOD(int x) { return ((x % 360) + 360) % 360; }
int arc(int l1, int l2) {
    const int d = MOD(l2 - l1);
    return d < 180 ? d : d - 360;
}
void do_testcase() {
    int N, M; scanf("%d %d", &N, &M);
    vector<int> loc(N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &loc[i]);
    }
    vector<vector<int>> adj(N);
    for (int i = 0; i < M; i++) {
        int u, v; scanf("%d %d", &u, &v);
        adj[u-1].push_back(v-1);
        adj[v-1].push_back(u-1);
    }
    vector<int> angle(N);
    vector<int> parent(N);
    vector<int> dist(N, -1);
    queue<pair<int, int>> Q;  // (node, parent)
    Q.emplace(0, -1);
    while (!Q.empty()) {
        const auto P = Q.front();
        const auto u = P.first;
        Q.pop();
        if (dist[u] >= 0) continue;
        parent[u] = P.second;
        if (parent[u] == -1) {
            dist[u] = 0;
            angle[u] = 0;
        } else {
            dist[u] = dist[parent[u]] + 1;
            angle[u] = angle[parent[u]] + arc(loc[parent[u]], loc[u]);
        }
        for (const auto v : adj[u]) {
            Q.emplace(v, u);
        }
    }
    int result = INT_MAX;
    for (int u = 0; u < N; u++) {
        for (int v : adj[u]) {
            if (dist[u] == -1 || dist[v] == -1) continue;
            // check for non-tree edge
            if (parent[u] == v || parent[v] == u) continue;
            if (angle[u] + arc(loc[u], loc[v]) - angle[v] != 0) {
                result = min(result, dist[u] + dist[v] + 1);
            }
        }
    }
    printf("%d\n", result == INT_MAX ? -1 : result);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
