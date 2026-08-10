// 2026-08-09
// Not too hard tree DP, not sure why someone gave it a "hard" concept rating
// and it has so few accepted users.  Our algorithm has two stages.  In the
// first stage, for each node `u` we find the best "closed" and "open" paths
// (see below for explanation) assuming that the subtree rooted at `u` is the
// entire graph (where we arbitrarily assume the entire tree is rooted at node
// 0).  To determine the actual value of a closed path in the subtree rooted at
// `u`, we re-root the tree at the parent of `u`.  This is done in the second
// stage (a second DFS that treats each node in the tree as the root in turn).
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
using LL = long long;
// A "hall of fame" with 2 players. The `update` method adds a new score `val`
// for player `i`.  Smaller scores are better.
struct Best2 {
    int i1 = -1;
    LL val1 = 1e12;
    int i2 = -1;
    LL val2 = 1e12;
    void update(int i, int val) {
        if (i == i1) {
            if (val < val1) val1 = val;
        } else if (i == i2) {
            if (val < val1) {
                swap(i1, i2);
                val2 = val1;
                val1 = val;
            } else if (val < val2) val2 = val;
        } else {
            if (val < val1) {
                val2 = val1;
                i2 = i1;
                val1 = val;
                i1 = i;
            } else if (val < val2) {
                i2 = i;
                val2 = val;
            }
        }
    }
};
int V, L;
vector<vector<int>> adj;
vector<int> tsize;
vector<LL> total_depth;
// Let `T` be the subtree rooted at `i`.  `closed[i]` is defined to be the
// minimum possible sum of distances from nodes in `T` to a path lying within
// `T` containing up to `L` hospitals (nodes) including `i`.  For a given `j`,
// `open[i][j]` is defined to be the minimum possible sum of distances from
// nodes in `T` to a path lying in `T` having at most `i` hospitals (nodes) and
// one of its endpoints being `i`.
vector<LL> closed;
vector<vector<LL>> open;
void rec1(int p, int u) {
    for (const int v : adj[u]) {
        if (v == p) continue;
        rec1(u, v);
        tsize[u] += tsize[v];
        total_depth[u] += total_depth[v] + tsize[v];
    }
    closed[u] = total_depth[u];
    open[u].push_back(1e12);
    open[u].push_back(total_depth[u]);
    // handle open paths with one endpoint at `u`
    for (const int v : adj[u]) {
        if (v == p) continue;
        for (int i = 1; i < open[v].size(); i++) {
            if (i + 1 > L) break;
            if (open[u].size() <= i + 1) open[u].push_back(1e12);
            open[u][i + 1] = min(open[u][i + 1],
                                 open[v][i] + total_depth[u]
                                 - total_depth[v] - tsize[v]);
        }
    }
    for (int i = 1; i < open[u].size(); i++) {
        open[u][i] = min(open[u][i], open[u][i - 1]);
        // every open path is also a closed path
        closed[u] = min(closed[u], open[u][i]);
    }

    // handle closed cases that use this root and 2 subtrees
    for (int i = 1; i <= (L - 1) / 2; i++) {
        const int l1 = i, l2 = L - 1 - i;
        // find the best 2 open paths for `l1` and `l2`
        Best2 b1, b2;
        for (const int v : adj[u]) {
            if (v == p) continue;
            const auto& co = open[v];
            b1.update(v, co[min(l1, (int)co.size() - 1)]
                         - tsize[v] - total_depth[v]);
            b2.update(v, co[min(l2, (int)co.size() - 1)]
                         - tsize[v] - total_depth[v]);
        }
        if (b1.i1 != b2.i1) {
            closed[u] = min(closed[u], total_depth[u] + b1.val1 + b2.val1);
        } else {
            closed[u] = min(closed[u], total_depth[u] + min(b1.val1 + b2.val2,
                                                            b2.val1 + b1.val2));
        }
    }
    // free up some memory before we return
    for (const int v : adj[u]) if (v != p) open[v].clear();
}

LL rec2(int p, int u) {
    LL result = 1e12;
    for (const int v : adj[u]) {
        if (v == p) continue;
        result = min(result, closed[v] + total_depth[u] - total_depth[v]
                             + tsize[u] - 2*tsize[v]);
        // This re-rooting operation destroys the original information about
        // node `v`.  We won't need it anyway, so we don't bother restoring it.
        total_depth[v] = total_depth[u] + tsize[u] - 2*tsize[v];
        tsize[v] = tsize[u];
        result = min(result, rec2(u, v));
    }
    return result;
}

void do_testcase() {
    scanf("%d %d", &V, &L);
    L++;  // we define `L` as number of nodes in the path, not length
    adj.clear(); adj.resize(V);
    for (int i = 0; i < V - 1; i++) {
        int u, v; scanf("%d %d", &u, &v);
        adj[u].push_back(v); adj[v].push_back(u);
    }
    tsize.assign(V, 1);
    total_depth.assign(V, 0);
    closed.resize(V);
    open.clear(); open.resize(V);
    rec1(-1, 0);
    printf("%lld\n", min(closed[0], rec2(-1, 0)));
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
