// 2026-09-15
// This problem can be solved using the same ideas as QTREE4; however, one
// drastic simplification is that we only care about the shallowest node in
// each component (relative to the root); we don't need to keep track of the
// shallowest node descended from *each child* of the root of the component.
// This is because all edge lengths are nonnegative and the problem asks for the
// closest white node; the minimum will always be found when the current
// centroid is the actual LCA of the query node and its closest white node, as
// when we move to the parent centroid we can only overestimate the distance by
// double-counting the edges from that node to the LCA.  A second significant
// optimization is that we can use segment trees instead of priority queues: for
// each centroid the segment tree maintains the minimum value of an array where
// `a[i]` is `i` if there is a white node with depth `i`, and +∞ otherwise.  (We
// could have used segtrees for QTREE4 as well, but the top-level priority queue
// couldn't be replaced by a segtree because there are O(V^2) possible values.)

#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;

const int INF = 5e8;

struct ST {
    int N;
    vector<int> data;

    ST() {}

    void init(int N) { this->N = N; data.assign(4*N, INF); }

    int get_min() { return data[1]; }

    void upd(int node, int l, int r, int i, int v) {
        if (r == l + 1) { data[node] = v; return; }
        const int m = (l + r) / 2;
        if (i < m) {
            upd(2*node, l, m, i, v);
        } else {
            upd(2*node + 1, m, r, i, v);
        }
        data[node] = min(data[2*node], data[2*node + 1]);
    }

    void upd(int d, int v) {
        upd(1, 0, N, d, v);
    }
};

struct Edge {
    int v;
    int r;  // index of reverse edge
};

vector<vector<Edge>> adj;

/// Delete all of `u`'s incoming edges.
void remove_node(int u) {
    for (const auto& e : adj[u]) {
        const auto v = e.v;
        if (e.r != adj[v].size() - 1) {
            const int w = adj[v].back().v;
            adj[w][adj[v].back().r].r = e.r;
            swap(adj[v][e.r], adj[v].back());
        }
        adj[v].pop_back();
    }
}

vector<int> sz;
vector<char> iswhite;
int nw;
vector<int> pc;  // parent centroid

/// `CentroidData` stores all data about one particular centroid.
struct CentroidData {
    vector<int> a;  // number of descendants with each depth
    ST segtree;
};
vector<CentroidData> cdata;

/// `cidx[u]` tells us how to update all the segtrees to which `u` belongs.  The
/// first entry in `cidx[u]` is for the root centroid and the last is for `u`
/// itself.  Each value in `cidx[u]` gives the distance of `u` from the
/// corresponding centroid.
vector<vector<int>> cidx;

/// Calculate subtree sizes.
void dfs1(int p, int u) {
    sz[u] = 1;
    for (const auto& e : adj[u]) {
        if (e.v != p) { dfs1(u, e.v); sz[u] += sz[e.v]; }
    }
}

/// Generate `cdata` and `cidx`.
void dfs2(int r, int p, int u, int d) {
    cidx[u].push_back(d);
    if (cdata[r].a.size() < d + 1) cdata[r].a.resize(d + 1);
    for (int i = 0; i < adj[u].size(); i++) {
        const auto& edge = adj[u][i];
        if (edge.v == p) continue;
        dfs2(r, u, edge.v, d + 1);
    }
}

/// Construct centroid tree.  Sizes are destroyed (we don't need them anymore
/// after this step).  Each node `u`'s adjacency list has `u`'s parent centroid
/// (if any) removed, so that what remains is a list of only the nodes adjacent 
/// to `u` in `T(u)`.
void rec(int p, int u) {
    const int total_size = sz[u];
    int c = u;
    bool fail = true;
    while (fail) {
        fail = false;
        for (const auto& e : adj[c]) {
            if (sz[e.v] > total_size / 2) {
                fail = true;
                sz[c] -= sz[e.v];
                sz[e.v] = total_size;
                c = e.v;
                break;
            }
        }
    }
    pc[c] = p;
    dfs2(c, -1, c, 0);
    auto& cd = cdata[c];
    const int N = cd.a.size();
    cd.segtree.init(N);
    remove_node(c);
    for (const auto& e : adj[c]) rec(c, e.v);
}

int main() {
    int V; scanf("%d", &V);
    iswhite.assign(V, 0);
    adj.resize(V);
    sz.resize(V);
    nw = 0;
    for (int i = 0; i < V - 1; i++) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        adj[u].push_back(Edge{v, (int)adj[v].size()});
        adj[v].push_back(Edge{u, (int)adj[u].size() - 1});
    }
    dfs1(-1, 0);
    pc.resize(V);
    cidx.resize(V);
    cdata.resize(V);
    rec(-1, 0);
    int Q; scanf("%d", &Q);
    while (Q--) {
        int kind, u; scanf("%d %d", &kind, &u); --u;
        if (kind == 1) {
            if (nw == 0) {
                puts("-1");
            } else {
                int result = INF;
                for (int c = u, i = cidx[u].size() - 1;
                     i >= 0;
                     i--, c = pc[c]) {
                    result = min(result,
                                 cidx[u][i] + cdata[c].segtree.get_min());
                }
                printf("%d\n", result);
            }
        } else {
            if (iswhite[u]) --nw; else ++nw;
            iswhite[u] ^= 1;
            for (int c = u, i = cidx[u].size() - 1; i >= 0; i--, c = pc[c]) {
                auto& cd = cdata[c];
                const auto d = cidx[u][i];
                if (iswhite[u]) {
                    if (0 == cd.a[d]++) cd.segtree.upd(d, d);
                } else {
                    if (0 == --cd.a[d]) cd.segtree.upd(d, INF);
                }
            }
        }
    }
}
