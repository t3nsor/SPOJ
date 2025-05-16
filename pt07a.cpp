// 2025-05-15
// This is a well-known game called "Green Hackenbush". The solution is known
// for general graphs; the fact that we are given a tree makes it a little bit
// easier. Still, while a bright competitor might discover the colon principle
// during a live contest, it's much less likely that they'd discover the
// fusion principle, which is also required for the solution. Apparently, the
// Play on Trees contest allowed contestants to use the Internet, otherwise
// this problem would have been overly difficult.
//
// The official solution says that we have to use strongly connected
// components to find all the cycles in order to apply the fusion principle
// (considering all nodes on the ground to be equivalent).  I'm not sure what
// they meant by that, considering that the graph is undirected. Actually, the
// fact that the original graph is a tree makes it really easy to predict the
// result after contracting all the cycles: every edge that belongs to a path
// in the tree between two nodes that are on the ground is on a cycle, and after
// all of those edges have been contracted, the result is a single node on the
// ground that has M self-loops, where M is the number of cycle edges.
// Equivalently, M is 1 less than the number of vertices that lie on any path
// that begins and ends on the ground (if there is only one ground node, then
// M is trivially 0). A self-loop is equivalent to an edge to a leaf node,
// since you can always cut it without affecting the rest of the graph. So we
// DFS once to compute M and then a second time to apply the colon principle
// to all the offshoots from cycle nodes.
//
// The time limit is very strict, so this implementation "flattens" the
// adjacency list instead of using a vector of vectors, and also keeps a bunch
// of vectors around as global variables that we must remember to reinitialize
// on each test case (not sure how much the latter affects the runtime).
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
vector<int> adj;
vector<int> deg;
vector<int> ofs;
vector<int> ground;
vector<int> loop;
void dfs1(int p, int v) {
    loop[v] = ground[v];
    for (int i = 0; i < deg[v]; i++) {
        const int w = adj[i + ofs[v]];
        if (w != p) {
            dfs1(v, w);
            loop[v] |= loop[w];
        }
    }
}
unsigned dfs2(int p, int v) {
    unsigned ret = 0;
    for (int i = 0; i < deg[v]; i++) {
        const int w = adj[i + ofs[v]];
        if (w != p && !loop[w]) {
            ret ^= 1 + dfs2(v, w);
        }
    }
    return ret;
}
void do_testcase() {
    int V; scanf("%d", &V);
    int source = -1;
    ground.assign(V, 0);
    for (int i = 0; i < V; i++) {
        scanf("%d", &ground[i]);
        if (ground[i]) source = i;
    }
    if (source < 0) throw;  // invalid input?
    vector<pair<int, int>> edges(V - 1);
    deg.assign(V, 0);
    for (int i = 0; i < V - 1; i++) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        edges[i] = make_pair(u, v);
        ++deg[u];
        ++deg[v];
    }
    ofs.assign(V, 0);
    ofs[0] = 0;
    for (int i = 1; i < V; i++) {
        ofs[i] = deg[i - 1] + ofs[i - 1];
    }
    vector<int> cur = ofs;
    adj.resize(2*V - 2);
    for (const auto e : edges) {
        const int u = e.first, v = e.second;
        adj[cur[u]++] = v;
        adj[cur[v]++] = u;
    }
    loop.assign(V, 0);
    dfs1(-1, source);
    unsigned nimber = 1;
    for (int i = 0; i < V; i++) {
        if (loop[i]) {
            nimber ^= 1;
            nimber ^= dfs2(-1, i);  // returns nimber
        }
    }
    printf("%d\n", bool(nimber));
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
