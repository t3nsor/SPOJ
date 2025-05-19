// 2025-05-18
// The property of being special, as described in the problem statement, is
// equivalent to consisting of a path plus an arbitrary number of leaf nodes
// attached to each node in the path. For a given path, the total number of
// nodes in the special tree (once all adjacent leaf nodes have been tacked
// on) equals 2 + sum(deg[v] - 1) where the sum is taken over all nodes `v` in
// the path. So we have reduced the original problem to that of finding the
// heaviest path in the tree where each node is weighted by its degree minus 1.
// This can be approached similarly to the classical problem of finding the
// longest path in a tree, weighted or unweighted (provided all weights are
// non-negative): pick an arbitrary node, use DFS to find the furthest node from
// it, then use DFS to find the furthest node from *that* node. The proof is
// left as an exercise for the reader. The time limit is strict, so we use fast
// input and a flattened representation of the graph. A comment from 2013
// suggests that the edges must be printed in the order in which they appear in
// the input, so we do that, too.
#include <stdio.h>
#include <vector>
using namespace std;
int get() {
    int result = 0;
    char c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    do {
        result = 10 * result + (c - '0');
        c = getchar_unlocked();
    } while (c > 32);
    return result;
}
vector<int> adj;
vector<int> deg;
vector<int> start;
pair<int, int> dfs1(int p, int v) {
    auto result = make_pair(v, deg[v] - 1);
    for (int i = 0; i < deg[v]; i++) {
        const int w = adj[i + start[v]];
        if (w == p) continue;
        auto result2 = dfs1(v, w);
        if (deg[v] - 1 + result2.second > result.second) {
            result.first = result2.first;
            result.second = deg[v] - 1 + result2.second;
        }
    }
    return result;
}
void dfs2(int p, int u, int t, vector<int>& used) {
    if (u == t) {
        used[u] = 1;
        return;
    }
    for (int i = 0; i < deg[u]; i++) {
        const int w = adj[i + start[u]];
        if (w == p) continue;
        dfs2(u, w, t, used);
        if (used[w]) {
            used[u] = 1;
            return;
        }
    }
}
int main() {
    const int V = get();
    adj.resize(2*V - 2);
    deg.resize(V);
    start.resize(V);
    vector<pair<int, int>> edges;
    for (int i = 0; i < V - 1; i++) {
        int u = get(), v = get(); --u; --v;
        edges.emplace_back(u, v);
        ++deg[u];
        ++deg[v];
    }
    for (int i = 1; i < V; i++) {
        start[i] = start[i - 1] + deg[i - 1];
    }
    for (const auto edge : edges) {
        adj[start[edge.first]++] = edge.second;
        adj[start[edge.second]++] = edge.first;
    }
    for (int i = 0; i < V; i++) {
        start[i] -= deg[i];
    }
    const int u = dfs1(-1, 0).first;
    const int v = dfs1(-1, u).first;  // u -> v is the heaviest path
    vector<int> used(V);
    dfs2(-1, u, v, used);
    vector<pair<int, int>> result;
    for (const auto edge : edges) {
        if (used[edge.first] || used[edge.second]) {
            result.push_back(edge);
        }
    }
    printf("%d\n", (int)result.size() + 1);
    for (const auto edge : result) {
        printf("%d %d\n", edge.first + 1, edge.second + 1);
    }
}
