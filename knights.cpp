// 2026-10-10
// This problem looks easy (599 accepted users) but I think it's not actually
// that easy.  Well, obviously it's easy to see that it comes down to building a
// graph where there's an edge between two nodes if they represent knights who
// *don't* hate each other, and then determining which nodes are part of an odd
// cycle, but it wasn't that obvious to me how to actually do it.  The solution
// is to decompose the graph into biconnected components and if a biconnected
// component fails to be bipartite, then every node in the biconnected component
// lies on an odd cycle.  (The proof is left as an exercise for the reader.  I
// think I know how to prove it, but my proof is ugly.)
//
// BCC code adapted from stopcity.cpp.  2D vectors and dynamic allocation
// avoided, since the input can be very large.
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <utility>
using namespace std;
int V;
int deg[1000];
int adj[1000][1000];
bool keep[1000];
bool hate[1000000];
int pre[1000];
int next_pre;
pair<int, int> stk[1000000];
int stksize;

int bccsize;
int bccnodes[1000];
int bccdeg[1000];
int bccadj[1000][2000];

int side[1000];

bool is_bipartite(int u, int s) {
    if (side[u] == -1) {
        side[u] = s;
        for (int i = 0; i < bccdeg[u]; i++) {
            if (!is_bipartite(bccadj[u][i], !s)) return false;
        }
        return true;
    } else {
        return side[u] == s;
    }
}

int dfs(int p, int u) {
    pre[u] = next_pre++;
    int ret = pre[u];
    for (int i = 0; i < deg[u]; i++) {
        const auto v = adj[u][i];
        if (v == p) continue;
        if (pre[v] >= 0) {
            if (pre[v] < pre[u]) {
                ret = min(ret, pre[v]);
                stk[stksize++] = make_pair(u, i);
            }
        } else {
            stk[stksize++] = make_pair(u, i);
            const auto child_ret = dfs(u, v);
            ret = min(ret, child_ret);
            if (p < 0 || (p >= 0 && child_ret >= pre[u])) {
                bccsize = 0;
                pair<int, int> P;
                do {
                    P = stk[--stksize];
                    const auto w = P.first;
                    const auto z = adj[w][P.second];
                    if (bccdeg[w] < 0) {
                        bccdeg[w] = 0;
                        bccnodes[bccsize++] = w;
                        side[w] = -1;
                    }
                    if (bccdeg[z] < 0) {
                        bccdeg[z] = 0;
                        bccnodes[bccsize++] = z;
                        side[z] = -1;
                    }
                    bccadj[w][bccdeg[w]++] = z;
                    bccadj[z][bccdeg[z]++] = w;
                } while (P != make_pair(u, i));
                const bool b = is_bipartite(u, 0);
                while (bccsize) {
                    const int w = bccnodes[--bccsize];
                    bccdeg[w] = -1;
                    if (!b) keep[w] = true;
                }
            }
        }
    }
    return ret;
}
int do_testcase(int E) {
    memset(deg, 0, V*sizeof(int));
    memset(keep, 0, V);
    memset(hate, 0, V*V);
    memset(pre, -1, V*sizeof(int));
    next_pre = 0;
    memset(bccdeg, -1, V*sizeof(int));
    for (int i = 0; i < E; i++) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        if (u < v) swap(u, v);
        hate[u+V*v] = true;
    }
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < i; j++) {
            if (!hate[i+V*j]) {
                adj[i][deg[i]++] = j;
                adj[j][deg[j]++] = i;
            }
        }
    }
    for (int i = 0; i < V; i++) {
        if (pre[i] < 0) dfs(-1, i);
    }
    int expel = 0;
    for (int i = 0; i < V; i++) if (!keep[i]) ++expel;
    return expel;
}
int main() {
    for (;;) {
        int E; scanf("%d %d", &V, &E);
        if (V == 0) break;
        printf("%d\n", do_testcase(E));
    }
}
