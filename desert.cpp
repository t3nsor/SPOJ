// 2026-06-22
// A comment on the problem GETBACK says that this problem (DESERT) is a harder
// version, but it doesn't seem to actually be harder, despite the fact that it
// involves two resources instead of one.  The key is that the need to consume
// water essentially just limits how much food can be brought from one node to
// another, and has no other relevance besides that.
//
// As in GETBACK, we assume without proof that you should travel to the
// destination in a "linear" fashion where you go back and forth along an edge
// as many times as necessary to move a certain amount of food from one end of
// that edge to the other, before proceeding to repeat this process with a new
// edge incident upon the destination end of the previous step.

#include <math.h>
#include <stdio.h>
#include <vector>
using namespace std;

void do_testcase(int cs, int V, int C) {
    vector<int> X(V), Y(V);
    for (int i = 0; i < V; i++) scanf("%d %d", &X[i], &Y[i]);
    vector<double> dist(V, 1e9);  // number of units of food needed at vertex
    vector<int> vis(V, 0);
    dist[V - 1] = 0;
    while (!vis[0]) {
        int u = -1, d = 1e9;
        for (int i = 0; i < V; i++) {
            if (!vis[i] && dist[i] < d) {
                u = i;
                d = dist[i];
            }
        }
        if (u == -1) break;
        vis[u] = 1;
        for (int v = 0; v < V; v++) {
            if (vis[v]) continue;
            const double d = hypot(X[u] - X[v], Y[u] - Y[v]);
            // can we do it in one trip?
            if (dist[u] + 2*d <= C) {
                dist[v] = min(dist[v], dist[u] + d);
            } else if (3*d < C) {
                // Each round trip wastes 2d in food.  The final trip wastes d.
                const int rt = ceil((dist[u] - (C - 2*d)) / (C - 3*d));
                dist[v] = min(dist[v], dist[u] + 2*d*rt + d);
            }
        }
    }
    printf("Trial %d: ", cs);
    if (vis[0]) {
        printf("%d units of food\n", int(ceil(dist[0])));
    } else {
        puts("Impossible");
    }
    putchar('\n');
}

int main() {
    for (int i = 1;; i++) {
        int V, C; scanf("%d %d", &V, &C);
        if (V == 0) break;
        do_testcase(i, V, C);
    }
}
