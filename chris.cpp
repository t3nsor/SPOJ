// 2025-04-25
// The idea is probably pretty obvious: let `u` be some node, let `a` be the
// longest path starting from `u`, let `b` be the longest path starting from
// `u` that has no edges in common with `a` (i.e. uses a different second
// node), and `c` the longest path starting from `u` that has no edges in
// common with `b` and `c`. If Blue Mary starts at the end of `c`, she will go
// to `u`, then to the end of `b`, then retrace her steps to `u` before going
// to the end of `a`, giving a total time of l(a) + 2l(b) + l(c). So finding the
// answer is trivial once we know, for every *directed* edge u -> v in the tree,
// the length of the longest path that starts with that edge.
//
// To calculate this, we DFS twice. In the first iteration we find the top 3
// paths that go *down* from the arbitrarily selected root (node 0). In the
// second, *before* we visit a node's children, we update the best paths for the
// node by including the path from the node to its parent. If the best path from
// the parent goes through the current node, we have to use the second best path
// from the parent instead in order to do this calculation. Pretty simple.
//
// Comments say we can get SIGSEGV from stack overflow, but that was probably
// before the memory limit was increased to 1.5GiB.
#include <array>
#include <stack>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void update(pair<long long, int> top[3],
            pair<long long, int> v) {
    if (v.first > top[2].first) {
        top[2] = v;
        if (top[2].first > top[1].first) {
            swap(top[2], top[1]);
            if (top[1].first > top[0].first) {
                swap(top[1], top[0]);
            }
        }
    }
}
void dfs1(const vector<vector<pair<int, int>>>& adj,
          int p, int u,
          vector<array<pair<long long, int>, 3>>& topdepth) {
    // depth1[u].second = -1;
    for (const auto& edge : adj[u]) {
        const int v = edge.first;
        const int l = edge.second;
        if (v != p) {
            dfs1(adj, u, v, topdepth);
            const auto d = make_pair(l + topdepth[v][0].first, v);
            update(topdepth[u].data(), d);
        }
    }
}
// update with parent edges
void dfs2(const vector<vector<pair<int, int>>>& adj,
          int p, int u, int l,
          vector<array<pair<long long, int>, 3>>& topdepth) {
    if (p != -1) {
        auto d = topdepth[p][0].second == u ? topdepth[p][1] : topdepth[p][0];
        d.first += l;
        update(topdepth[u].data(), d);
    }
    for (const auto& edge : adj[u]) {
        if (edge.first != p) dfs2(adj, u, edge.first, edge.second, topdepth);
    }
}
void do_testcase() {
    int V; scanf("%d %*d", &V);
    vector<vector<pair<int, int>>> adj(V);
    for (int i = 0; i < V - 1; i++) {
        int x, y, l; scanf("%d %d %d", &x, &y, &l); --x; --y;
        adj[x].emplace_back(y, l);
        adj[y].emplace_back(x, l);
    }
    vector<array<pair<long long, int>, 3>> topdepth(V);
    dfs1(adj, -1, 0, topdepth);
    dfs2(adj, -1, 0, 0, topdepth);
    long long best = 0;
    for (int i = 0; i < V; i++) {
        best = max(best, topdepth[i][0].first
                         + 2*topdepth[i][1].first
                         + topdepth[i][2].first);
    }
    printf("%lld\n", best);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
