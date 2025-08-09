// 2025-08-08
// This problem can be solved using the centroid decomposition technique. We
// find the centroid of the tree, find the best valid path that passes through
// that node, and then remove the centroid and repeat for each resulting
// subtree.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
int V, K;
vector<int> is_crowded;
vector<int> tsize;
// adjacency list entry
struct Entry {
    int v;
    int w;
    // In order to efficiently remove nodes from the tree, we need to be able to
    // easily look up, for a given edge (from u to v) where to find its reverse
    // edge in v's adjacency list.
    int ridx;
};
vector<vector<Entry>> adj;
void remove_node(int u) {
    for (const auto& e : adj[u])  {
        const int ridx = e.ridx;
        // To remove an adjacency list entry, swap it with the last entry in the
        // list and then remove the last entry. This requires fixing up the
        // `ridx` value of the reverse edge, of course.
        if (ridx < adj[e.v].size() - 1) {
            auto& vlast = adj[e.v].back();
            adj[vlast.v][vlast.ridx].ridx = ridx;
            swap(adj[e.v][ridx], vlast);
        }
        adj[e.v].pop_back();
    }
}
void dfs1(int parent, int u) {
    tsize[u] = 1;
    for (const auto& e : adj[u]) {
        if (e.v == parent) continue;
        dfs1(u, e.v);
        tsize[u] += tsize[e.v];
    }
}
void dfs2(int parent, int u, int w, int nc, vector<int>& paths) {
    if (paths.size() <= nc) {
        paths.push_back(-2e9);
    }
    paths[nc] = max(paths[nc], w);
    for (const auto& e : adj[u]) {
        if (e.v == parent) continue;
        dfs2(u, e.v, w + e.w, nc + is_crowded[e.v], paths);
    }
}
// A "hall of fame" with 2 players. The `update` method adds a new score `val`
// for player `i`.
struct Best2 {
    int i1 = -1;
    int val1 = -2e9;
    int i2 = -1;
    int val2 = -2e9;
    void update(int i, int val) {
        if (i == i1) {
            if (val > val1) val1 = val;
        } else if (i == i2) {
            if (val > val1) {
                swap(i1, i2);
                val2 = val1;
                val1 = val;
            } else if (val > val2) val2 = val;
        } else {
            if (val > val1) {
                val2 = val1;
                i2 = i1;
                val1 = val;
                i1 = i;
            } else if (val > val2) {
                i2 = i;
                val2 = val;
            }
        }
    }
};
int rec(int cur) {
    // find centroid
    const int total_size = tsize[cur];
    int parent = -1;
    vector<int> children;
retry:
    children.clear();
    for (const auto& e : adj[cur]) {
        children.push_back(e.v);
        if (e.v != parent && tsize[e.v] > total_size / 2) {
            tsize[cur] -= tsize[e.v];
            tsize[e.v] = total_size;
            parent = cur;
            cur = e.v;
            goto retry;
        }
    }

    // Find best path through centroid.
    //
    // First, for each child `v` of the centroid, we DFS from `v` to generate
    // an array A[v] such that A[v][c] is the best path value for a path that
    // starts at the centroid, goes through `v`, and visits exactly `c` crowded
    // places (not including the centroid itself).
    //
    // Once we have A, we can combine two paths to obtain a path with total
    // value A[u][i] + A[v][j], where u != v, and i + j is the total number of
    // crowded places in the path, not including the centroid.  This represents
    // a path where u and v are the neighbours of the centroid.  We want to
    // maximize that expression subject to i + j <= K - is_crowded[centroid].
    //
    // To do all this in linear time we first generate the "transpose" T, where
    // "transpose" T, where T[c] is a list of all `v` such that A[v][c] > -inf.
    // For each element `u` in T[i], we want to find the best value of A[v][j]
    // where v != u and i + j <= K - is_crowded[centroid].  By iterating over
    // `i` from largest to smallest, every time we decrement `i`, another column
    // of A may become eligible to participate in the maximum. Using the set
    // values in that column (which are found using T) we update the running
    // best 2 values (where the second best must use a different first node than
    // the best).  For each `u` in T[i] we check whether it can be combined with
    // the best (because the first node of the best is not `u`).  If not then we
    // combine it with the second best.
    int result = -2e9;
    vector<vector<int>> A;
    vector<vector<int>> T;
    int maxc = 0;
    for (const auto& e : adj[cur]) {
        vector<int> a = {0};
        dfs2(cur, e.v, e.w, is_crowded[e.v], a);
        for (int i = 0; i < a.size(); i++) {
            if (T.size() < i + 1) {
                T.emplace_back();
            }
            T[i].push_back(A.size());
        }
        A.push_back(move(a));
    }
    const int limit = K - is_crowded[cur];
    if (limit >= 0) result = 0;
    Best2 B; int maxj_included = 0;
    for (int i = T.size() - 1; i >= 0; i--) {
        if (i > limit) continue;
        const int maxj = min(limit - i, (int)T.size() - 1);
        while (maxj_included < maxj) {
            ++maxj_included;
            for (const int k : T[maxj_included]) {
                B.update(k, A[k][maxj_included]);
            }
        }
        for (const int k : T[i]) {
            int tot = A[k][i];
            const int term = (B.i1 != k ? B.val1 : B.val2);
            if (term > 0) tot += term;
            result = max(result, tot);
        }
    }

    // remove centroid for recursion
    remove_node(cur);
    for (int v : children) {
        result = max(result, rec(v));
    }
    return result;
}
int main() {
    int M; scanf("%d %d %d", &V, &K, &M);
    if (K == 0 && M == V) throw;
    adj.resize(V);
    is_crowded.resize(V);
    while (M--) {
        int v; scanf("%d", &v); is_crowded[v - 1] = 1;
    }
    for (int i = 0; i < V - 1; i++) {
        int u, v, w; scanf("%d %d %d", &u, &v, &w); --u; --v;
        const int uidx = adj[u].size();
        const int vidx = adj[v].size();
        adj[u].push_back(Entry{v, w, vidx});
        adj[v].push_back(Entry{u, w, uidx});
    }
    tsize.resize(V);
    dfs1(-1, 0);
    printf("%d\n", rec(0));
}
