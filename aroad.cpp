// 2024-04-03
// First we find all connected components of the graph with vertex 0 removed. A
// spanning tree for the entire graph must use at least one edge from vertex 0
// to each such connected component. If insufficient edges exist from vertex 0
// or if `d` is less than `ncc` (the number of connected components), we print
// NONE and finish. Otherwise, we start by using the cheapest edge from vertex 0
// to each connected component. This gives us the best possible spanning tree
// that uses `ncc` edges from vertex 0.
//
// While the degree of vertex 0 is less than `d`, we greedily update the
// spanning tree by selecting an edge from vertex 0 that isn't in the tree yet,
// and deleting the most expensive edge on the cycle formed by adding that edge.
// At each stage we pick the edge that gives us the greatest reduction in total
// cost. Since the bounds are small, we don't need link/cut trees or other
// complicated data structures. We just treat the spanning tree as always being
// rooted at vertex 0 and keep track of parent pointers.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
const int INF = 1e9;
void dfs_label(vector<int>& cc, int u, int label,
               const vector<vector<int>>& adj) {
    cc[u] = label;
    for (int v = 1; v < adj[u].size(); v++) {
        if (adj[u][v] < INF && cc[v] == -1) {
            dfs_label(cc, v, label, adj);
        }
    }
}
struct Edge {
    int cost;
    int u;
    int v;
};
bool operator<(Edge e1, Edge e2) {
    return e1.cost < e2.cost;
}
int kruskal(vector<Edge>&& edges,
            vector<vector<int>>& tree,
            const vector<int>& cc,
            const vector<int>& cheapest_neighbor) {
    const int V = tree.size();
    vector<int> id(V);
    for (int i = 0; i < V; i++) id[i] = i;
    sort(edges.begin(), edges.end());
    int have = 0;
    int cost = 0;
    for (auto e : edges) {
        if (e.v == 0) swap(e.u, e.v);
        if (e.u == 0 && e.v != cheapest_neighbor[cc[e.v]]) continue;
        int uid = e.u, vid = e.v;
        while (id[uid] != uid) {
            uid = id[uid] = id[id[uid]];
        }
        while (id[vid] != vid) {
            vid = id[vid] = id[id[vid]];
        }
        if (uid == vid) continue;
        id[uid] = vid;
        tree[e.u][e.v] = tree[e.v][e.u] = 1;
        ++have;
        cost += e.cost;
        if (have == V - 1) break;
    }
    return cost;
}
void dfs_root(int cur_parent, int u, vector<int>& parent,
              const vector<vector<int>>& tree) {
    parent[u] = cur_parent;
    for (int v = 0; v < tree.size(); v++) {
        if (v == cur_parent || !tree[u][v]) continue;
        dfs_root(u, v, parent, tree);
    }
}
void do_testcase() {
    int V, E, maxdeg; scanf("%d %d %d", &V, &E, &maxdeg);
    vector<vector<int>> adj(V, vector<int>(V, INF));
    vector<Edge> edges;
    while (E--) {
        Edge e; scanf("%d %d %d", &e.u, &e.v, &e.cost); --e.u; --e.v;
        edges.push_back(e);
        adj[e.u][e.v] = adj[e.v][e.u] = min(adj[e.u][e.v], e.cost);
    }
    // find connected components without node 0
    vector<int> cc(V, -1);
    int ncc = 0;
    for (int i = 1; i < V; i++) {
        if (cc[i] == -1) {
            dfs_label(cc, i, ncc, adj);
            ++ncc;
        }
    }
    if (maxdeg < ncc) {
        // we need at least one edge to each connected component
        puts("NONE");
        return;
    }
    // find cheapest edge from node 0 to each cc
    vector<int> cheapest_neighbor(ncc, -1);
    for (int i = 1; i < V; i++) {
        if (adj[0][i] == INF) continue;
        if (cheapest_neighbor[cc[i]] == -1 ||
            adj[0][i] < adj[0][cheapest_neighbor[cc[i]]]) {
            cheapest_neighbor[cc[i]] = i;
        }
    }
    for (int i = 0; i < ncc; i++) {
        if (cheapest_neighbor[i] == -1) {
            // again, we need at least one edge to each connected component
            puts("NONE");
            return;
        }
    }
    // build MST where an edge from vertex 0 is allowed to be used only if it's
    // the cheapest edge from vertex 0 to the other vertex's cc
    vector<vector<int>> orig_mst(V, vector<int>(V, 0));
    int cost = kruskal(move(edges), orig_mst, cc, cheapest_neighbor);
    // root the MST at vertex 0
    vector<int> parent(V);
    dfs_root(-1, 0, parent, orig_mst);
    // keep track of which edges from vertex 0 have been used so far
    vector<int> used(V, 0);
    for (int i = 0; i < ncc; i++) used[cheapest_neighbor[i]] = 1;
    for (int deg = ncc + 1; deg <= maxdeg; ++deg) {
        int best_neighbor;
        int best_breakpoint;
        int best_reduction = 0;
        for (int u = 1; u < V; u++) {
            if (used[u]) continue;
            // find path from `u` to the root
            int highest_cost = -1, breakpoint;
            int uu = u;
            while (parent[uu] != 0) {
                if (adj[uu][parent[uu]] > highest_cost) {
                    highest_cost = adj[uu][parent[uu]];
                    breakpoint = parent[uu];
                }
                uu = parent[uu];
            }
            if (highest_cost > adj[0][u]) {
                if (highest_cost - adj[0][u] > best_reduction) {
                    best_reduction = highest_cost - adj[0][u];
                    best_neighbor = u;
                    best_breakpoint = breakpoint;
                }
            }
        }
        if (best_reduction == 0) {
            // no further improvement possible
            break;
        }
        cost -= best_reduction;
        used[best_neighbor] = 1;
        // reparent everything on the near side toward the new edge
        int prev = 0;
        int u = best_neighbor;
        do {
            const int nextu = parent[u];
            parent[u] = prev;
            prev = u;
            u = nextu;
        } while (u != best_breakpoint);
    }
    printf("%d\n", cost);
}
int main() {
    int c; scanf("%d", &c);
    while (c--) do_testcase();
}
