// 2026-04-24
// binary search + tree DP
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void dfs(const vector<vector<pair<int, int>>>& adj, int p, int u,
         vector<int>& ht, vector<vector<pair<int, int>>>& children) {
    ht[u] = 0;
    for (const auto edge : adj[u]) {
        if (edge.first == p) continue;
        children[u].push_back(edge);
        dfs(adj, u, edge.first, ht, children);
        ht[u] = max(ht[u], edge.second + ht[edge.first]);
    }
}
// Return value:
//   first:  min len for a path that falls entirely within this subtree and
//           satisfies the distance constraint `maxd` assuming only this subtree
//           exists (where the root `u` is treated as if it has a phantom node
//           attached by an edge with weight `pd`, but the path is not allowed
//           to contain the phantom node)
//   second: same except that the phantom node is not present and the path must
//           have `u` at one end.
// Note that `p` is the parent node (we skip it when descending).
vector<pair<int, int>> scratch;
pair<int, int> rec(const vector<vector<pair<int, int>>>& children,
                   const vector<int>& ht,
                   int maxd,
                   int u,
                   int p,
                   int pd) {
    pair<int, int> result;
    result.first = result.second = 1e9 + 1;
    int d1 = 0, d2 = 0;
    for (const auto edge : children[u]) {
        if (edge.first == p) continue;
        if (edge.second + ht[edge.first] > d1) {
            d2 = d1;
            d1 = edge.second + ht[edge.first];
        } else if (edge.second + ht[edge.first] > d2) {
            d2 = edge.second + ht[edge.first];
        }
    }
    // always consider the case where we use this node only
    if (d1 <= maxd) result.second = 0;
    if (d1 + pd <= maxd) result.first = 0;
    for (const auto edge : children[u]) {
        if (edge.first == p) continue;
        int otherd = d1;
        if (d1 == edge.second + ht[edge.first]) {
            otherd = d2;
        }
        const int newpd = max(otherd, pd) + edge.second;
        scratch[edge.first] = rec(children, ht, maxd, edge.first, u, newpd);
        result.first = min(result.first, scratch[edge.first].first);
        // can we attach this node to one end of a path?
        if (otherd <= maxd) {
            result.second = min(result.second,
                                scratch[edge.first].second + edge.second);
        }
    }
    // attach up to two paths to the current node
    if (pd <= maxd) {
        int nreq = 0;
        int req[2];
        int reql = 0;
        for (const auto edge : children[u]) {
            if (edge.second + ht[edge.first] > maxd) {
                if (nreq < 2) {
                    req[nreq] = edge.first;
                    reql += edge.second + scratch[edge.first].second;
                }
                ++nreq;
            }
        }
        if (nreq <= 2) {
            result.first = min(result.first, reql);
        }
    }
    return result;
}
void do_testcase() {
    int V, S; scanf("%d %d", &V, &S);
    vector<vector<pair<int, int>>> adj(V);
    int r = 0;
    for (int i = 0; i < V - 1; i++) {
        int u, v, w; scanf("%d %d %d", &u, &v, &w); --u; --v;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
        r += w;
    }
    vector<int> ht(V);
    vector<vector<pair<int, int>>> children(V);
    dfs(adj, -1, 0, ht, children);
    int l = 0;
    scratch.resize(V);
    while (l < r) {
        int m = (l + r) / 2;
        if (rec(children, ht, m, 0, -1, 0).first <= S) r = m; else l = m + 1;
    }
    printf("%d\n", l);
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
