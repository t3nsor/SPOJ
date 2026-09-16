// 2026-09-12
// This is a classic centroid decomposition problem.  For a given node u, let
// T(u) denote u's component (that is, the connected component to which u
// belongs immediately after its parent centroid, if any, is deleted).  We
// associate with u the following data:
// * The distances of all nodes in T(u) from u (does not change)
// * For each child v of u in T(u), the set of all white nodes descended from v
//   in T(u), sorted by distance from u
// * The set S of children v themselves, sorted by the max value in v's set plus
//   the edge length from u to v.  Thus, the sum of the largest two values in
//   S, or the largest value if u is white, or 0, is the max distance between
//   white nodes in T(u) such that the path passes through u (call it D(u)).
// When changing a node color we have to update the data for the node itself,
// its parent centroid, and so on up to the root, i.e., O(log V) nodes total,
// where each update takes O(log V) time.  This gives O((Q + V)log^2 V) time and
// O(V log V) space.  We also have an overall set of all nodes in the entire
// tree, ordered by their D-values.
//
// Implementing this using multisets has too large a constant factor, so I
// switched the implementation to using heaps.  As we have to perform frequent
// "increase key" and "decrease key" operations, I implemented these by hand.
// (An alternative approach is to add a new element to the heap when a key needs
// to be increased, but lazily remove elements whose keys have been decreased.
// This is more complicated and probably not faster, so I'm too lazy to
// implement it and check.)  Also the constant factor of this solution could
// definitely be improved by reducing memory fragmentation but I'm too lazy for
// that as well.
//
// I believe an O(V) space algorithm is possible using heavy-light decomposition
// (HLD), but it doesn't improve the time bound and it seems much more annoying
// because I couldn't imagine a way to do it that doesn't involve maintaining
// one auxiliary segment tree per heavy path.  This may be why none of the
// solutions I found online use HLD.  (But if you ask nicely I'm sure you can
// get an LLM to generate one for you.)

#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;

const int INF = 5e8;

struct Heap {
    int N;
    int *v;  // table of priorities
    int *h;  // heap, contains values [0, n)
    int *p;  // p[i] is the location of i in the heap
    Heap() = default;
    void init(int N, int* v, int* h, int *p) {
        this->N = N; this->v = v; this->h = h; this->p = p;
        for (int i = 0; i < N; i++) { h[i] = i; }
        make_heap(h, h + N, [v](int i, int j) { return v[i] < v[j]; });
        for (int i = 0; i < N; i++) p[h[i]] = i;
    }
    int top() const {
        return v[h[0]];
    }
    int top2sum() const {
        int t1 = v[h[0]], t2 = -INF;
        if (N >= 3) {
            t2 = max(v[h[1]], v[h[2]]);
        } else if (N == 2) {
            t2 = v[h[1]];
        }
        return t1 + t2;
    }

    /// exchange the elements at positions lhs, rhs in the heap, then set lhs to
    /// rhs
    void exch(int& lhs, int rhs) {
        p[h[lhs]] = rhs;
        p[h[rhs]] = lhs;
        swap(h[lhs], h[rhs]);
        lhs = rhs;
    }

    /// set v[i] to x and update the heap accordingly
    void set(int i, int x) {
        int cur = p[i];
        if (x >= v[i]) {
            // bubble up
            while (cur > 0 && x > v[h[(cur - 1)/ 2]]) exch(cur, (cur - 1) / 2);
        } else {
            // bubble down
            while ((2*cur + 1 < N && x < v[h[2*cur + 1]]) ||
                   (2*cur + 2 < N && x < v[h[2*cur + 2]])) {
                if (2*cur + 1 == N - 1 ||
                    v[h[2*cur + 1]] >= v[h[2*cur + 2]]) {
                    exch(cur, 2*cur + 1);
                } else {
                    exch(cur, 2*cur + 2);
                }
            }
        }
        v[i] = x;
    }
};

struct Edge {
    int v;
    int w;
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
vector<int> D;  // best distance for each centroid (for a path that uses this
                // node and is contained within its component)
vector<int> Dh;  // heap array for `D`
vector<int> Dp;  // position array for heap for `D`
Heap H;  // heap of best distances per centroid
vector<int> pc;  // parent centroid

/// `ChildData` stores all data about one particular child of one particular
/// centroid.
struct ChildData {
    vector<int> d;  // distance of descendants from a centroid
    vector<int> v;  // value array for heap (equal to d for white nodes or -INF
                    // for black nodes)
    vector<int> h;
    vector<int> p;
    Heap H;
};

vector<vector<ChildData>> child_data;

/// `cidx[u]` tells us how to update all the heaps to which `u` belongs.  The
/// first entry in `cidx[u]` is for the root centroid and the last is for `u`
/// itself.  The first member of each pair is the child index and the second is
/// the index within the `ChildData` object.
vector<vector<pair<int, int>>> cidx;

struct CentroidData {
    vector<int> v;  // value array for heap (deepest white node per child)
    vector<int> h;
    vector<int> p;
    Heap H;
};
vector<CentroidData> centroid_data;

/// Calculate subtree sizes.
void dfs1(int p, int u) {
    sz[u] = 1;
    for (const auto& e : adj[u]) {
        if (e.v != p) { dfs1(u, e.v); sz[u] += sz[e.v]; }
    }
}

/// Generate `child_data` and `cidx`.
void dfs2(int r, int p, int u, int child, int d) {
    if (u == r) {
        cidx[u].emplace_back(-1, -1);
    } else {
        cidx[u].emplace_back(child, child_data[r][child].d.size());
        child_data[r][child].d.push_back(d);
    }
    for (int i = 0; i < adj[u].size(); i++) {
        const auto& edge = adj[u][i];
        if (edge.v == p) continue;
        int child2 = child;
        if (u == r) {
            child_data[r].emplace_back();
            child2 = child_data[r].size() - 1;
        }
        dfs2(r, u, edge.v, child2, d + edge.w);
    }
}

/// Return the `D` value for a given node (or 0 if there are no white nodes in
/// its component).
int calcD(int c) {
    int result = 0;
    const int nchild = adj[c].size();
    const auto& cd = centroid_data[c];
    if (nchild >= 1) {
        if (iswhite[c]) result = max(result, cd.H.top());
        if (nchild >= 2) result = max(result, cd.H.top2sum());
    }
    return result;
}

/// Construct centroid tree.  Sizes are destroyed (we don't need them anymore
/// after this step).  Each node `u`'s adjacency list has `u`'s parent centroid
/// (if any) removed, so that what remains is a list of only the nodes adjacent 
/// to `u` in `T(u)`.  Initial values of `D` are also calculated.
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
    dfs2(c, -1, c, -1, 0);
    for (auto& cd : child_data[c]) {
        int ndesc = cd.d.size();
        for (int i = 0; i < ndesc; i++) {
            cd.v.push_back(cd.d[i]);
        }
        cd.h.resize(ndesc); cd.p.resize(ndesc);
        cd.H.init(ndesc, cd.v.data(), cd.h.data(), cd.p.data());
    }
    auto& cd = centroid_data[c];
    const int nchild = adj[c].size();
    for (int i = 0; i < nchild; i++) {
        cd.v.push_back(child_data[c][i].H.top());
    }
    cd.h.resize(nchild); cd.p.resize(nchild);
    cd.H.init(nchild, cd.v.data(), cd.h.data(), cd.p.data());
    D[c] = calcD(c);
    remove_node(c);
    for (const auto& e : adj[c]) rec(c, e.v);
}

int main() {
    int V; scanf("%d", &V);
    iswhite.assign(V, 1);
    adj.resize(V);
    sz.resize(V);
    nw = V;
    for (int i = 0; i < V - 1; i++) {
        int u, v, d; scanf("%d %d %d", &u, &v, &d); --u; --v;
        adj[u].push_back(Edge{v, d, (int)adj[v].size()});
        adj[v].push_back(Edge{u, d, (int)adj[u].size() - 1});
    }
    dfs1(-1, 0);
    pc.resize(V);
    child_data.resize(V);
    cidx.resize(V);
    centroid_data.resize(V);
    D.resize(V);
    rec(-1, 0);
    Dh.resize(V); Dp.resize(V); H.init(V, D.data(), Dh.data(), Dp.data());
    int Q; scanf("%d", &Q);
    while (Q--) {
        int c; do { c = getchar_unlocked(); } while (c <= 32);
        if (c == 'A') {
            if (nw == 0) puts("They have disappeared.");
            else printf("%d\n", H.top());
            continue;
        }
        int u; scanf("%d", &u); --u;
        if (iswhite[u]) --nw; else ++nw;
        iswhite[u] ^= 1;
        for (int c = u, i = cidx[u].size() - 1; i >= 0; i--, c = pc[c]) {
            const auto& p = cidx[u][i];
            if (p.first >= 0) {
                auto& cd1 = child_data[c][p.first];
                cd1.H.set(p.second, iswhite[u] ? cd1.d[p.second] : -INF);
                auto& cd2 = centroid_data[c];
                cd2.H.set(p.first, cd1.H.top());
            }
            H.set(c, calcD(c));
        }
    }
}
